#pragma warning disable CA1034

namespace Itmo.ObjectOrientedProgramming.Lab4.Core.Models;

public abstract record BuildResult
{
    private BuildResult() { }

    public sealed record Success() : BuildResult;

    public sealed record MissedRequiredArgument(string Argument) : BuildResult;
}

#pragma warning restore CA1034