using Application.Extensions;
using Console.Extensions;
using Console.Scenarios;
using DataAccess.Extensions;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.DependencyInjection;

static string GetPathToConfig(string configName)
{
    string currentDirectory = Environment.CurrentDirectory;
    string? workingDirectory = Directory.GetParent(currentDirectory)?.Parent?.Parent?.FullName;
    ArgumentNullException.ThrowIfNull(workingDirectory);
    return Path.Combine(workingDirectory, configName);
}

IConfigurationRoot config = new ConfigurationBuilder()
    .AddJsonFile(GetPathToConfig("config.json"), false, true)
    .Build();

IServiceCollection collection = new ServiceCollection();
collection.AddApplication()
    .AddInfrastructureDataAccess(configuration =>
    {
        ArgumentNullException.ThrowIfNull(config["port"]);
        if (!int.TryParse(config["port"], out int port))
            throw new ArgumentException("port must has integer type");

        configuration.Port = port;
        configuration.Host = config["host"] ?? throw new ArgumentNullException(nameof(configuration));
        configuration.Username = config["username"] ?? throw new ArgumentNullException(nameof(configuration));
        configuration.Password = config["password"] ?? throw new ArgumentNullException(nameof(configuration));
        configuration.Database = config["database"] ?? throw new ArgumentNullException(nameof(configuration));
        configuration.SslMode = config["sslMode"] ?? throw new ArgumentNullException(nameof(configuration));
    })
    .AddFluentMigrations()
    .AddConsoleLayer();

ServiceProvider provider = collection.BuildServiceProvider();
using IServiceScope scope = provider.CreateScope();
scope.AddInfrastructureDataAccess();

ScenarioRunner? runner = scope.ServiceProvider.GetService<ScenarioRunner>();
ArgumentNullException.ThrowIfNull(runner);

while (true)
{
    runner.Run();
    System.Console.Clear();
}