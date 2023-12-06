using System;
using Itmo.ObjectOrientedProgramming.Lab4.Core.Entities.Commands;
using Itmo.ObjectOrientedProgramming.Lab4.Core.Models;

namespace Itmo.ObjectOrientedProgramming.Lab4.Core.Entities.ParserChainLink;

public class ParseConnectChainLink : ParserChainLinkBase
{
    public override ParseResult TryParse(Iterator<string> iterator)
    {
        ArgumentNullException.ThrowIfNull(iterator);
        ConnectCommand.Builder builder = ConnectCommand.GetBuilder;

        if (iterator.Current() != "connect")
            return Next?.TryParse(iterator) ?? new ParseResult.NotSuitableCommand(iterator.Current());

        iterator.Skip(1);

        if (!iterator.HasNext())
            return new ParseResult.MissedRequiredArgument("address");

        builder.WithAddress(iterator.Next());

        return ParserHelper(iterator, builder);
    }
}