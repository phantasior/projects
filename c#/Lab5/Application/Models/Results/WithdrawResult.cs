#pragma warning disable CA1034
namespace Models.Results;

public abstract record WithdrawResult
{
    private WithdrawResult() { }

    public sealed record Success : WithdrawResult;

    public sealed record NotEnoughMoney : WithdrawResult;

    public sealed record AccountNotFound : WithdrawResult;

    public sealed record StrangerAccount : WithdrawResult;
}
#pragma warning restore CA1034