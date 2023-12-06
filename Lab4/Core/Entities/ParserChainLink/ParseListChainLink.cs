using System;
using Itmo.ObjectOrientedProgramming.Lab4.Core.Entities.Commands;
using Itmo.ObjectOrientedProgramming.Lab4.Core.Models;

namespace Itmo.ObjectOrientedProgramming.Lab4.Core.Entities.ParserChainLink;

public class ParseListChainLink : ParserChainLinkBase
{
    public override ParseResult TryParse(Iterator<string> iterator)
    {
        ArgumentNullException.ThrowIfNull(iterator);
        ListCommand.Builder builder = ListCommand.GetBuilder;

        if (iterator.Current() != "tree" || !iterator.HasNext() || iterator.CheckNext() != "list")
            return Next?.TryParse(iterator) ?? new ParseResult.NotSuitableCommand(iterator.Current());

        iterator.Skip(2);

        return ParserHelper(iterator, builder);
    }
}