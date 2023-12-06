using Abstractions.Repositories;
using DataAccess.PasswordHandlers;
using DataAccess.Postgres.ConnectionProvider;
using Models.Accounts;
using Npgsql;

namespace DataAccess.Repositories;

public class AccountRepository : IAccountRepository
{
    private readonly IPostgresConnectionProvider _provider;

    public AccountRepository(IPostgresConnectionProvider provider)
    {
        _provider = provider;
    }

    public IEnumerable<Account> FindAccountsByUserId(long usedId)
    {
        const string sql =
            """
                select id, pin, user_id, balance 
                from accounts
                where user_id = :userId;
            """;

        NpgsqlConnection connection = _provider
            .GetConnectionAsync(default)
            .GetAwaiter()
            .GetResult();

        using var command = new NpgsqlCommand(sql, connection);
        command.Parameters.Add(new NpgsqlParameter<long>("userId", usedId));

        using NpgsqlDataReader reader = command.ExecuteReader();

        while (reader.ReadAsync().GetAwaiter().GetResult())
        {
            yield return new Account(
                reader.GetInt64(0), // id
                reader.GetString(1), // hashedPin
                reader.GetInt64(2), // userId
                reader.GetInt64(3)); // balance
        }
    }

    public Account? FindAccountById(long accountId)
    {
        const string sql =
            """
                select id, pin, user_id, balance 
                from accounts
                where id = :accountId;
            """;

        NpgsqlConnection connection = _provider
            .GetConnectionAsync(default)
            .GetAwaiter()
            .GetResult();

        using var command = new NpgsqlCommand(sql, connection);
        command.Parameters.Add(new NpgsqlParameter<long>("accountId", accountId));

        using NpgsqlDataReader reader = command.ExecuteReader();

        reader.ReadAsync().GetAwaiter().GetResult();
        return new Account(
            reader.GetInt64(0), // id
            reader.GetString(1), // hashedPin
            reader.GetInt64(2), // userId
            reader.GetInt64(3)); // balance
    }

    public bool AddAccount(long userId, int pin)
    {
        const string sql =
            """
                insert into accounts 
                values (DEFAULT, :hashedPin, :userId, 0);
            """;

        NpgsqlConnection connection = _provider
            .GetConnectionAsync(default)
            .GetAwaiter()
            .GetResult();

        using var command = new NpgsqlCommand(sql, connection);
        command.Parameters.Add(new NpgsqlParameter<string>("hashedPin", PasswordHandler.GenerateHash(pin)));
        command.Parameters.Add(new NpgsqlParameter<long>("userId", userId));

        int rowsAffected = command.ExecuteNonQueryAsync().GetAwaiter().GetResult();
        return rowsAffected == 1;
    }

    public bool UpdateAccountBalance(long accountId, long newBalance)
    {
        const string sql =
            """
                update accounts 
                set balance = :newBalance
                where id = :accountId;
            """;

        NpgsqlConnection connection = _provider
            .GetConnectionAsync(default)
            .GetAwaiter()
            .GetResult();

        using var command = new NpgsqlCommand(sql, connection);
        command.Parameters.Add(new NpgsqlParameter<long>("newBalance", newBalance));
        command.Parameters.Add(new NpgsqlParameter<long>("accountId", accountId));

        int rowsAffected = command.ExecuteNonQueryAsync().GetAwaiter().GetResult();
        return rowsAffected == 1;
    }

    public IEnumerable<AccountOperation> GetAccountHistory(long accountId)
    {
        const string sql =
            """
                select account_id, amount from history
                where account_id = :accountId;
            """;

        NpgsqlConnection connection = _provider
            .GetConnectionAsync(default)
            .GetAwaiter()
            .GetResult();

        using var command = new NpgsqlCommand(sql, connection);
        command.Parameters.Add(new NpgsqlParameter<long>("accountId", accountId));

        using NpgsqlDataReader reader = command.ExecuteReader();
        if (reader.Read() is false)
            yield break;

        while (reader.ReadAsync().GetAwaiter().GetResult())
        {
            yield return new AccountOperation(
                reader.GetInt64(0),
                reader.GetInt64(1));
        }
    }
}