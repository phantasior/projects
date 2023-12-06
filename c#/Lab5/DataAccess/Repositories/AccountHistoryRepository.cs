using Abstractions.Repositories;
using DataAccess.Postgres.ConnectionProvider;
using Models.History;
using Npgsql;

namespace DataAccess.Repositories;

public class AccountHistoryRepository : IAccountHistoryRepository
{
    private readonly IPostgresConnectionProvider _provider;

    public AccountHistoryRepository(IPostgresConnectionProvider provider)
    {
        _provider = provider;
    }

    public bool AddRecord(long accountId, long amount)
    {
        const string sql =
            """
                insert into history
                values (DEFAULT, :accountId, :amount, current_timestamp)
            """;

        NpgsqlConnection connection = _provider
            .GetConnectionAsync(default)
            .GetAwaiter()
            .GetResult();

        using var command = new NpgsqlCommand(sql, connection);
        command.Parameters.Add(new NpgsqlParameter<long>("accountId", accountId));
        command.Parameters.Add(new NpgsqlParameter<long>("amount", amount));

        int result = command.ExecuteNonQueryAsync().GetAwaiter().GetResult();
        return result == 1;
    }

    public IEnumerable<AccountHistoryItem> ShowHistoryByAccount(long accountId)
    {
        const string sql =
            """
                select account_id, amount, timestamp 
                from history
                where account_id = :accountId;
            """;

        NpgsqlConnection connection = _provider
            .GetConnectionAsync(default)
            .GetAwaiter()
            .GetResult();

        using var command = new NpgsqlCommand(sql, connection);
        command.Parameters.Add(new NpgsqlParameter<long>("accountId", accountId));

        using NpgsqlDataReader reader = command.ExecuteReader();

        while (reader.ReadAsync().GetAwaiter().GetResult())
        {
            yield return new AccountHistoryItem(
                reader.GetInt64(0),
                reader.GetInt64(1),
                reader.GetDateTime(2));
        }
    }
}