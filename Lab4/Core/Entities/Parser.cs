using System;
using Itmo.ObjectOrientedProgramming.Lab4.Core.Entities.ParserChainLink;
using Itmo.ObjectOrientedProgramming.Lab4.Core.Models;

namespace Itmo.ObjectOrientedProgramming.Lab4.Core.Entities;

public class Parser
{
    private readonly IParserChainLink _chainLink;

    public Parser(IParserChainLink chainLink)
    {
        _chainLink = chainLink;
    }

    public ParseResult Parse(string raw)
    {
        raw = raw ?? throw new ArgumentNullException(nameof(raw));
        string[] data = raw.Split(" ", StringSplitOptions.RemoveEmptyEntries | StringSplitOptions.TrimEntries);
        var iterator = new Iterator<string>(data);

        return _chainLink.TryParse(iterator);
    }
}