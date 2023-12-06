using System.Runtime.CompilerServices;

namespace DataAccess.Postgres;

public class PostgresConfiguration
{
    public string Host { get; set; } = string.Empty;

    public int Port { get; set; }

    public string Database { get; set; } = string.Empty;

    public string Username { get; set; } = string.Empty;

    public string Password { get; set; } = string.Empty;

    public string SslMode { get; set; } = string.Empty;

    public bool Pooling { get; set; } = true;

    public string ToConnectionString()
    {
        var interpolatedStringHandler = new DefaultInterpolatedStringHandler(60, 7);
        interpolatedStringHandler.AppendLiteral("Host=");
        interpolatedStringHandler.AppendFormatted(this.Host);
        interpolatedStringHandler.AppendLiteral(";");
        interpolatedStringHandler.AppendLiteral("Port=");
        interpolatedStringHandler.AppendFormatted<int>(this.Port);
        interpolatedStringHandler.AppendLiteral(";");
        interpolatedStringHandler.AppendLiteral("Database=");
        interpolatedStringHandler.AppendFormatted(this.Database);
        interpolatedStringHandler.AppendLiteral(";");
        interpolatedStringHandler.AppendLiteral("Username=");
        interpolatedStringHandler.AppendFormatted(this.Username);
        interpolatedStringHandler.AppendLiteral(";");
        interpolatedStringHandler.AppendLiteral("Password=");
        interpolatedStringHandler.AppendFormatted(this.Password);
        interpolatedStringHandler.AppendLiteral(";");
        interpolatedStringHandler.AppendLiteral("Ssl Mode=");
        interpolatedStringHandler.AppendFormatted(this.SslMode);
        interpolatedStringHandler.AppendLiteral(";");
        interpolatedStringHandler.AppendLiteral("Pooling=");
        interpolatedStringHandler.AppendFormatted<bool>(this.Pooling);
        return interpolatedStringHandler.ToStringAndClear();
    }
}