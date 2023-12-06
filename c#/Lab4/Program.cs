using System;
using System.Collections.Generic;
using Itmo.ObjectOrientedProgramming.Lab4.Core.Entities;
using Itmo.ObjectOrientedProgramming.Lab4.Core.Entities.FileSystems;
using Itmo.ObjectOrientedProgramming.Lab4.Core.Entities.ParserChainLink;
using Itmo.ObjectOrientedProgramming.Lab4.Core.Models;
using Itmo.ObjectOrientedProgramming.Lab4.Core.Services.Readers;
using Itmo.ObjectOrientedProgramming.Lab4.Core.Services.Writers;

namespace Itmo.ObjectOrientedProgramming.Lab4;

public static class Program
{
    public static void Main()
    {
        Console.WriteLine("[App started!]");

        IParserChainLink chain = new ParseConnectChainLink()
            .AddNext(new ParseShowChainLink())
            .AddNext(new ParseRenameCommand())
            .AddNext(new ParseCopyChainLink())
            .AddNext(new ParseDeleteChainLink())
            .AddNext(new ParseDisconnectChainLink())
            .AddNext(new ParseListChainLink())
            .AddNext(new ParseMoveChainLink())
            .AddNext(new ParseGoToChainLink());

        var parser = new Parser(chain);
        var writer = new ConsoleWriter();
        var reader = new ConsoleReader();

        var availableModes = new Dictionary<string, Func<IFileSystem>>() { { "local", () => new LocalFileSystem() } };
        var availableShowModes = new Dictionary<string, Func<IWriter>>() { { "console", () => new ConsoleWriter() } };

        var fileSystem = new FileSystemManager(availableModes, availableShowModes, writer);

        while (true)
        {
            fileSystem.PrintDirectory();
            string row = reader.Read();
            if (row == "exit")
                break;

            ParseResult result = parser.Parse(row);
            if (result is ParseResult.Success success)
            {
                fileSystem.Execute(success.Command);
                continue;
            }

            writer.Write(HandleError(result));
        }
    }

    private static string HandleError(ParseResult result)
    {
        return result switch
        {
            ParseResult.Success => string.Empty,
            ParseResult.MissedRequiredArgument err => $"Missed required argument: {err.MissedArgument}",
            ParseResult.NotSuitableCommand err => $"Not suitable command: {err.Command}",
            _ => "Unknown error",
        };
    }
}