#pragma warning disable CA1034
using Itmo.ObjectOrientedProgramming.Lab4.Core.Entities.Commands;

namespace Itmo.ObjectOrientedProgramming.Lab4.Core.Models;

public abstract record ParseResult
{
    private ParseResult() { }

    public sealed record Success(ICommand Command) : ParseResult;

    public sealed record MissedRequiredArgument(string MissedArgument) : ParseResult;

    public sealed record NotSuitableCommand(string Command) : ParseResult;
}

#pragma warning restore CA1034