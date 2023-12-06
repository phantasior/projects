using Npgsql;

namespace DataAccess.Postgres.ConnectionProvider;

public interface IPostgresConnectionProvider
{
    Task<NpgsqlConnection> GetConnectionAsync(CancellationToken cancellationToken);
}