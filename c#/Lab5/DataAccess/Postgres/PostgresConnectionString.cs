namespace DataAccess.Postgres;

public class PostgresConnectionString
{
    public PostgresConnectionString(PostgresConfiguration configuration)
    {
        Value = configuration?.ToConnectionString() ?? throw new ArgumentNullException(nameof(configuration));
    }

    public string Value { get; }
}