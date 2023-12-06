using FluentMigrator.Runner;
using Microsoft.Extensions.DependencyInjection;

namespace DataAccess.Extensions;

public static class ServiceScopeExtensions
{
    public static IServiceScope AddInfrastructureDataAccess(this IServiceScope scope)
    {
        ArgumentNullException.ThrowIfNull(scope);
        scope.ServiceProvider.GetRequiredService<IMigrationRunner>().MigrateUp();
        return scope;
    }
}