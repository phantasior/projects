using Itmo.ObjectOrientedProgramming.Lab4.Core.Models;

namespace Itmo.ObjectOrientedProgramming.Lab4.Core.Entities.ParserChainLink;

public interface IParserChainLink
{
    IParserChainLink AddNext(IParserChainLink chainLink);

    ParseResult TryParse(Iterator<string> iterator);

    ParseResult ParserHelper(Iterator<string> iterator, IBuilder builder);

    void ArgumentParserHelper(Iterator<string> iterator, IBuilder builder);
}