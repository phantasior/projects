using System;
using Itmo.ObjectOrientedProgramming.Lab4.Core.Entities.Commands;
using Itmo.ObjectOrientedProgramming.Lab4.Core.Models;

namespace Itmo.ObjectOrientedProgramming.Lab4.Core.Entities.ParserChainLink;

public class ParseGoToChainLink : ParserChainLinkBase
{
    public override ParseResult TryParse(Iterator<string> iterator)
    {
        ArgumentNullException.ThrowIfNull(iterator);
        GoToCommand.Builder builder = GoToCommand.GetBuilder;

        if (iterator.Current() != "tree" || !iterator.HasNext() || iterator.CheckNext() != "goto")
            return Next?.TryParse(iterator) ?? new ParseResult.NotSuitableCommand(iterator.Current());

        iterator.Skip(2);

        if (!iterator.HasNext())
            return new ParseResult.MissedRequiredArgument("path");

        builder.WithPath(iterator.Next());

        return ParserHelper(iterator, builder);
    }
}