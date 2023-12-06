using System;
using Itmo.ObjectOrientedProgramming.Lab4.Core.Entities.Commands;
using Itmo.ObjectOrientedProgramming.Lab4.Core.Models;

namespace Itmo.ObjectOrientedProgramming.Lab4.Core.Entities.ParserChainLink;

public class ParseMoveChainLink : ParserChainLinkBase
{
    public override ParseResult TryParse(Iterator<string> iterator)
    {
        ArgumentNullException.ThrowIfNull(iterator);
        MoveCommand.Builder builder = MoveCommand.GetBuilder;

        if (iterator.Current() != "file" || !iterator.HasNext() || iterator.CheckNext() != "move")
            return Next?.TryParse(iterator) ?? new ParseResult.NotSuitableCommand(iterator.Current());

        iterator.Skip(2);

        if (!iterator.HasNext())
            return new ParseResult.MissedRequiredArgument("source");

        builder.WithSource(iterator.Next());

        if (!iterator.HasNext())
            return new ParseResult.MissedRequiredArgument("destination");

        builder.WithDestination(iterator.Next());

        return new ParseResult.Success(builder.Build());
    }
}