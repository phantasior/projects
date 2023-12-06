using Application.Accounts;
using Application.History;
using Application.Users;
using Contracts.Accounts;
using Contracts.Context;
using Contracts.History;
using Contracts.Users;
using Microsoft.Extensions.DependencyInjection;

namespace Application.Extensions;

public static class ServiceCollectionExtensions
{
    public static IServiceCollection AddApplication(this IServiceCollection collection)
    {
        collection.AddScoped<IUserService, UserService>();
        collection.AddScoped<IHistoryService, HistoryService>();
        collection.AddScoped<IAccountService, AccountService>();
        collection.AddScoped<IContext, ProgramContext>();

        return collection;
    }
}