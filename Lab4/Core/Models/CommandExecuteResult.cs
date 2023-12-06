#pragma warning disable CA1034

namespace Itmo.ObjectOrientedProgramming.Lab4.Core.Models;

public abstract record CommandExecuteResult
{
    private CommandExecuteResult() { }

    public sealed record SuccessWith(string Result) : CommandExecuteResult;

    public sealed record ErrorResult(string ErrorMessage) : CommandExecuteResult;
}

#pragma warning restore CA1034