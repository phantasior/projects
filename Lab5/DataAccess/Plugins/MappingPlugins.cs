using Models.Users;
using Npgsql;

namespace DataAccess.Plugins;

public class MappingPlugins : IDataSourcePlugin
{
    public void Configure(NpgsqlDataSourceBuilder builder)
    {
        ArgumentNullException.ThrowIfNull(builder);
        builder.MapEnum<UserRole>();
    }
}