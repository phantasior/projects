#pragma warning disable CA1034
namespace Models.Results;

public abstract record GetBalanceResult
{
    private GetBalanceResult() { }

    public sealed record Success(long Value) : GetBalanceResult;

    public sealed record NotFound : GetBalanceResult;
}
#pragma warning restore CA1034