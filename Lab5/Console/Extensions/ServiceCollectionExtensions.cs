using Console.Scenarios;
using Console.Scenarios.AccountsList;
using Console.Scenarios.CreateAccount;
using Console.Scenarios.Deposit;
using Console.Scenarios.History;
using Console.Scenarios.Login;
using Console.Scenarios.Logout;
using Console.Scenarios.Signup;
using Console.Scenarios.WithdrawMoney;
using Microsoft.Extensions.DependencyInjection;

namespace Console.Extensions;

public static class ServiceCollectionExtensions
{
    public static IServiceCollection AddConsoleLayer(this IServiceCollection collection)
    {
        collection.AddScoped<ScenarioRunner>();
        collection.AddScoped<IScenarioProvider, CreateAccountProvider>();
        collection.AddScoped<IScenarioProvider, GetAccountsListScenarioProvider>();
        collection.AddScoped<IScenarioProvider, HistoryScenarioProvider>();
        collection.AddScoped<IScenarioProvider, DepositScenarioProvider>();
        collection.AddScoped<IScenarioProvider, WithdrawScenarioProvider>();
        collection.AddScoped<IScenarioProvider, LoginScenarioProvider>();
        collection.AddScoped<IScenarioProvider, SignUpScenarioProvider>();
        collection.AddScoped<IScenarioProvider, LogoutScenarioProvider>();
        return collection;
    }
}