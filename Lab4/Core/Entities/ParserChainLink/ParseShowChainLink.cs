using System;
using Itmo.ObjectOrientedProgramming.Lab4.Core.Entities.Commands;
using Itmo.ObjectOrientedProgramming.Lab4.Core.Models;

namespace Itmo.ObjectOrientedProgramming.Lab4.Core.Entities.ParserChainLink;

public class ParseShowChainLink : ParserChainLinkBase
{
    public override ParseResult TryParse(Iterator<string> iterator)
    {
        ArgumentNullException.ThrowIfNull(iterator);
        ShowCommand.Builder builder = ShowCommand.GetBuilder;

        if (iterator.Current() != "file" || !iterator.HasNext() || iterator.CheckNext() != "show")
            return Next?.TryParse(iterator) ?? new ParseResult.NotSuitableCommand(iterator.Current());

        iterator.Skip(2);

        if (!iterator.HasNext())
            return new ParseResult.MissedRequiredArgument("path");

        builder.WithPath(iterator.Next());

        return ParserHelper(iterator, builder);
    }
}