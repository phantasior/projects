using System;
using Itmo.ObjectOrientedProgramming.Lab4.Core.Models;

namespace Itmo.ObjectOrientedProgramming.Lab4.Core.Entities.ParserChainLink;

public abstract class ParserChainLinkBase : IParserChainLink
{
    protected IParserChainLink? Next { get; private set; }

    public IParserChainLink AddNext(IParserChainLink chainLink)
    {
        if (Next is null)
        {
            Next = chainLink;
        }
        else
        {
            Next.AddNext(chainLink);
        }

        return this;
    }

    public ParseResult ParserHelper(Iterator<string> iterator, IBuilder builder)
    {
        ArgumentParserHelper(iterator, builder);

        ArgumentNullException.ThrowIfNull(builder);
        BuildResult result = builder.Validate();
        if (result is BuildResult.MissedRequiredArgument fail)
            return new ParseResult.MissedRequiredArgument(fail.Argument);

        return new ParseResult.Success(builder.Build());
    }

    public void ArgumentParserHelper(Iterator<string> iterator, IBuilder builder)
    {
        ArgumentNullException.ThrowIfNull(iterator);
        ArgumentNullException.ThrowIfNull(builder);
        while (iterator.HasNext())
        {
            string argument = iterator.Next();
            if (!iterator.HasNext() || iterator.Current().StartsWith("-", StringComparison.Ordinal))
            {
                builder.WithFlag(argument);
                continue;
            }

            builder.WithArgument(argument, iterator.Current());
        }
    }

    public abstract ParseResult TryParse(Iterator<string> iterator);
}