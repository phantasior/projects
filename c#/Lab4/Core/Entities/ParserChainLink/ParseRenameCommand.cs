using System;
using Itmo.ObjectOrientedProgramming.Lab4.Core.Entities.Commands;
using Itmo.ObjectOrientedProgramming.Lab4.Core.Models;

namespace Itmo.ObjectOrientedProgramming.Lab4.Core.Entities.ParserChainLink;

public class ParseRenameCommand : ParserChainLinkBase
{
    public override ParseResult TryParse(Iterator<string> iterator)
    {
        ArgumentNullException.ThrowIfNull(iterator);
        RenameCommand.Builder builder = RenameCommand.GetBuilder;

        if (iterator.Current() != "file" || !iterator.HasNext() || iterator.CheckNext() != "rename")
            return Next?.TryParse(iterator) ?? new ParseResult.NotSuitableCommand(iterator.Current());

        iterator.Skip(2);

        if (!iterator.HasNext())
            return new ParseResult.MissedRequiredArgument("path");

        builder.WithPath(iterator.Next());

        if (!iterator.HasNext())
            return new ParseResult.MissedRequiredArgument("name");

        builder.WithName(iterator.Next());

        return ParserHelper(iterator, builder);
    }
}