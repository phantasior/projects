using Abstractions.Repositories;
using DataAccess.PasswordHandlers;
using DataAccess.Postgres.ConnectionProvider;
using Models.Users;
using Npgsql;

namespace DataAccess.Repositories;

public class UserRepository : IUserRepository
{
    private readonly IPostgresConnectionProvider _provider;

    public UserRepository(IPostgresConnectionProvider provider)
    {
        _provider = provider;
    }

    public User? FindUserByUsername(string username)
    {
        const string sql =
            """
                select id, password, user_name, role 
                from users
                where user_name = :username;
            """;

        NpgsqlConnection connection = _provider
            .GetConnectionAsync(default)
            .GetAwaiter()
            .GetResult();

        using var command = new NpgsqlCommand(sql, connection);
        command.Parameters.Add(new NpgsqlParameter<string>("username", username));

        using NpgsqlDataReader reader = command.ExecuteReader();
        if (reader.Read() is false)
            return null;

        return new User(
            Id: reader.GetInt64(0),
            Password: reader.GetString(1),
            Username: reader.GetString(2),
            Role: reader.GetFieldValue<UserRole>(3));
    }

    public bool AddUser(string username, string password)
    {
        const string sql =
            """
                insert into users
                values (DEFAULT, :password, :username, :userRole);
            """;

        NpgsqlConnection connection = _provider
            .GetConnectionAsync(default)
            .GetAwaiter()
            .GetResult();

        using var command = new NpgsqlCommand(sql, connection);
        command.Parameters.Add(new NpgsqlParameter<string>("username", username));
        command.Parameters.Add(new NpgsqlParameter<string>("password", PasswordHandler.GenerateHash(password)));
        command.Parameters.Add(new NpgsqlParameter<UserRole>("userRole", UserRole.User));

        int rowsAffected = command.ExecuteNonQueryAsync().GetAwaiter().GetResult();
        return rowsAffected == 1;
    }
}