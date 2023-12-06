using Abstractions.Repositories;
using DataAccess.Migrations;
using DataAccess.Plugins;
using DataAccess.Postgres;
using DataAccess.Postgres.ConnectionProvider;
using DataAccess.Repositories;
using FluentMigrator.Runner;
using Microsoft.Extensions.DependencyInjection;

namespace DataAccess.Extensions;

public static class ServiceCollectionExtensions
{
    public static IServiceCollection AddInfrastructureDataAccess(
        this IServiceCollection collection,
        Action<PostgresConfiguration> configurationSetter)
    {
        ArgumentNullException.ThrowIfNull(configurationSetter);

        var configuration = new PostgresConfiguration();
        configurationSetter(configuration);

        collection.AddSingleton(configuration);
        collection.AddSingleton<PostgresConnectionString>();

        collection.AddScoped<IUserRepository, UserRepository>();
        collection.AddScoped<IAccountRepository, AccountRepository>();
        collection.AddScoped<IAccountHistoryRepository, AccountHistoryRepository>();

        collection.AddScoped<IPostgresConnectionProvider, PostgresConnectionProvider>();
        collection.AddSingleton<IDataSourcePlugin, MappingPlugins>();
        return collection;
    }

    public static IServiceCollection AddFluentMigrations(this IServiceCollection collection)
    {
        collection.AddFluentMigratorCore()
            .ConfigureRunner(rb => rb
                .AddPostgres()
                .WithGlobalConnectionString(
                    collection.BuildServiceProvider().GetService<PostgresConnectionString>()?.Value ??
                    throw new ArgumentNullException(nameof(rb)))
                .ScanIn(typeof(InitialMigration).Assembly).For.Migrations())
            .AddLogging(lb => lb.AddFluentMigratorConsole());
        return collection;
    }
}