using System;
using Itmo.ObjectOrientedProgramming.Lab4.Core.Entities.Commands;
using Itmo.ObjectOrientedProgramming.Lab4.Core.Models;

namespace Itmo.ObjectOrientedProgramming.Lab4.Core.Entities.ParserChainLink;

public class ParseDisconnectChainLink : ParserChainLinkBase
{
    public override ParseResult TryParse(Iterator<string> iterator)
    {
        ArgumentNullException.ThrowIfNull(iterator);
        DisconnectCommand.Builder builder = DisconnectCommand.GetBuilder;

        if (iterator.Current() != "disconnect")
            return Next?.TryParse(iterator) ?? new ParseResult.NotSuitableCommand(iterator.Current());

        iterator.Skip(1);

        return ParserHelper(iterator, builder);
    }
}