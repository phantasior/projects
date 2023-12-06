using System;
using Itmo.ObjectOrientedProgramming.Lab4.Core.Entities.Commands;
using Itmo.ObjectOrientedProgramming.Lab4.Core.Models;

namespace Itmo.ObjectOrientedProgramming.Lab4.Core.Entities.ParserChainLink;

public class ParseCopyChainLink : ParserChainLinkBase
{
    public override ParseResult TryParse(Iterator<string> iterator)
    {
        ArgumentNullException.ThrowIfNull(iterator);
        CopyCommand.Builder builder = CopyCommand.GetBuilder;

        if (iterator.Current() != "file" || !iterator.HasNext() || iterator.CheckNext() != "copy")
            return Next?.TryParse(iterator) ?? new ParseResult.NotSuitableCommand(iterator.Current());

        iterator.Skip(2);

        if (!iterator.HasNext())
            return new ParseResult.MissedRequiredArgument("source");

        builder.WithSource(iterator.Next());

        if (!iterator.HasNext())
            return new ParseResult.MissedRequiredArgument("destination");

        builder.WithDestination(iterator.Next());

        return ParserHelper(iterator, builder);
    }
}