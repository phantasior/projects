// for test naming

#pragma warning disable CA1707

using System;
using System.Collections.Generic;
using Itmo.ObjectOrientedProgramming.Lab4.Core.Entities;
using Itmo.ObjectOrientedProgramming.Lab4.Core.Entities.Commands;
using Itmo.ObjectOrientedProgramming.Lab4.Core.Entities.FileSystems;
using Itmo.ObjectOrientedProgramming.Lab4.Core.Entities.ParserChainLink;
using Itmo.ObjectOrientedProgramming.Lab4.Core.Models;
using Itmo.ObjectOrientedProgramming.Lab4.Core.Services.Writers;
using NSubstitute;
using Xunit;

namespace Itmo.ObjectOrientedProgramming.Lab4.Tests;

public class FileSystemTests
{
    private readonly Parser _parser;
    private readonly Dictionary<string, Func<IWriter>> _availableShowModes;

    public FileSystemTests()
    {
        IParserChainLink chain = new ParseConnectChainLink()
            .AddNext(new ParseDisconnectChainLink())
            .AddNext(new ParseDeleteChainLink())
            .AddNext(new ParseShowChainLink())
            .AddNext(new ParseRenameCommand())
            .AddNext(new ParseCopyChainLink())
            .AddNext(new ParseListChainLink())
            .AddNext(new ParseMoveChainLink())
            .AddNext(new ParseGoToChainLink());

        _parser = new Parser(chain);
        _availableShowModes = new Dictionary<string, Func<IWriter>>()
        {
            { "console", () => new ConsoleWriter() },
        };
    }

    [Theory]
    [InlineData("connect ./ -m local", "file move ./ ./", "././", "././")]
    public void FileSystemMove_ShouldBeCorrect_Default(string connect, string list, string from, string to)
    {
        IWriter writer = Substitute.For<IWriter>();
        IFileSystem mockedFileSystem = Substitute.For<IFileSystem>();
        var mockedLocalFileSystemMode = new Dictionary<string, Func<IFileSystem>>()
            { { "local", () => mockedFileSystem } };

        ParseResult connectResult = _parser.Parse(connect);
        ParseResult.Success connectSuccess =
            connectResult as ParseResult.Success ?? throw new ArgumentNullException(nameof(connect));

        ParseResult moveResult = _parser.Parse(list);
        ParseResult.Success moveSuccess =
            moveResult as ParseResult.Success ?? throw new ArgumentNullException(nameof(connect));

        IFileSystemManager fileSystem = new FileSystemManager(mockedLocalFileSystemMode, _availableShowModes, writer);

        fileSystem.Execute(connectSuccess.Command);
        fileSystem.Execute(moveSuccess.Command);

        mockedFileSystem.Received().Move(from, to);
    }

    [Theory]
    [InlineData("connect ./ -m local", "file show ./hi/text.txt -m console", "././hi/text.txt")]
    public void FileShow_ShouldBeCorrect_Default(string connect, string show, string expectedPath)
    {
        IWriter writer = Substitute.For<IWriter>();
        IFileSystem mockedFileSystem = Substitute.For<IFileSystem>();
        var mockedLocalFileSystemMode = new Dictionary<string, Func<IFileSystem>>()
            { { "local", () => mockedFileSystem } };
        ParseResult connectResult = _parser.Parse(connect);
        ParseResult.Success connectSuccess =
            connectResult as ParseResult.Success ?? throw new ArgumentNullException(nameof(connect));

        ParseResult showResult = _parser.Parse(show);
        ParseResult.Success showSuccess =
            showResult as ParseResult.Success ?? throw new ArgumentNullException(nameof(connect));

        IFileSystemManager fileSystem = new FileSystemManager(mockedLocalFileSystemMode, _availableShowModes, writer);

        fileSystem.Execute(connectSuccess.Command);
        fileSystem.Execute(showSuccess.Command);

        mockedFileSystem.Received().Show(expectedPath);
    }

    [Theory]
    [InlineData("connect /home/src/ -m local", typeof(ConnectCommand))]
    [InlineData("connect   /home/    -m    local", typeof(ConnectCommand))]
    [InlineData("disconnect", typeof(DisconnectCommand))]
    [InlineData("tree list -d 3", typeof(ListCommand))]
    [InlineData("tree list", typeof(ListCommand))]
    [InlineData("file show ./path/to/file -m console", typeof(ShowCommand))]
    [InlineData("file move ./123 ../", typeof(MoveCommand))]
    [InlineData("file copy ./123 ../", typeof(CopyCommand))]
    [InlineData("file delete ../ somefile.txt", typeof(DeleteCommand))]
    [InlineData("file rename ./123 ../", typeof(RenameCommand))]
    [InlineData("tree goto ./123", typeof(GoToCommand))]
    public void Parse_ShouldBeCorrect_Default(string raw, Type type)
    {
        ParseResult result = _parser.Parse(raw);
        var success = result as ParseResult.Success;

        Assert.IsType<ParseResult.Success>(result);
        Assert.IsType(type, success?.Command);
    }

    [Theory]
    [InlineData("connect ./path")]
    [InlineData("tree goto")]
    [InlineData("file show")]
    [InlineData("file move")]
    [InlineData("file move ./path")]
    [InlineData("file copy")]
    [InlineData("file copy ./path")]
    [InlineData("file delete")]
    [InlineData("file rename")]
    [InlineData("file rename ./path")]
    public void Parse_ShouldHasError_WithoutRequiredArgument(string raw)
    {
        ParseResult result = _parser.Parse(raw);

        Assert.IsType<ParseResult.MissedRequiredArgument>(result);
    }

    [Theory]
    [InlineData("text")]
    [InlineData("longer text")]
    [InlineData("file shoow")]
    [InlineData("file copyy")]
    [InlineData("filee copy")]
    public void Parse_ShouldHasError_UnknownCommand(string raw)
    {
        ParseResult result = _parser.Parse(raw);

        Assert.IsType<ParseResult.NotSuitableCommand>(result);
    }
}

#pragma warning restore CA1707