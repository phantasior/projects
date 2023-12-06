#pragma warning disable CA1034
namespace Models.Results;

public abstract record SignUpResult
{
    private SignUpResult() { }

    public sealed record Success : SignUpResult;

    public sealed record Failure : SignUpResult;
}
#pragma warning restore CA1034