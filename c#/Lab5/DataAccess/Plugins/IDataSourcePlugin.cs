using Npgsql;

namespace DataAccess.Plugins;

public interface IDataSourcePlugin
{
    void Configure(NpgsqlDataSourceBuilder builder);
}