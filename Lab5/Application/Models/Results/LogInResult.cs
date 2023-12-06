#pragma warning disable CA1034
namespace Models.Results;

public abstract record LogInResult
{
    private LogInResult() { }

    public sealed record Success : LogInResult;

    public sealed record IncorrectPassword : LogInResult;

    public sealed record NotFound : LogInResult;
}
#pragma warning restore CA1034