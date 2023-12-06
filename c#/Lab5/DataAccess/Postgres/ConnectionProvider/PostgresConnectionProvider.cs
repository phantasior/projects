using DataAccess.Plugins;
using Npgsql;

namespace DataAccess.Postgres.ConnectionProvider;

public class PostgresConnectionProvider : IPostgresConnectionProvider
{
    private readonly Lazy<Task<NpgsqlConnection>> _connection;

    public PostgresConnectionProvider(
        PostgresConnectionString connectionString,
        IEnumerable<IDataSourcePlugin> plugins)
    {
        _connection = new Lazy<Task<NpgsqlConnection>>(async () =>
        {
            var builder = new NpgsqlDataSourceBuilder(connectionString.Value);
            foreach (IDataSourcePlugin plugin in plugins)
                plugin.Configure(builder);

            return await builder.Build().OpenConnectionAsync().ConfigureAwait(false);
        });
    }

    public async Task<NpgsqlConnection> GetConnectionAsync(CancellationToken cancellationToken)
    {
        return await _connection.Value.ConfigureAwait(false);
    }
}