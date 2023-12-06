using System;
using Itmo.ObjectOrientedProgramming.Lab4.Core.Models;

#pragma warning disable CA1034

namespace Itmo.ObjectOrientedProgramming.Lab4.Core.Entities.Commands;

public class DeleteCommand : ICommand
{
    private readonly string _path;

    private DeleteCommand(string path)
    {
        _path = path;
    }

    public static Builder GetBuilder => new Builder();

    public void Execute(IFileSystemManager fileSystem)
    {
        fileSystem?.Delete(_path);
    }

    public class Builder : IBuilder
    {
        private string? _path;

        public Builder WithPath(string path)
        {
            _path = path;
            return this;
        }

        public IBuilder WithArgument(string argument, string value)
        {
            return this;
        }

        public IBuilder WithFlag(string flag)
        {
            return this;
        }

        public BuildResult Validate()
        {
            if (_path is null)
                return new BuildResult.MissedRequiredArgument("path");

            return new BuildResult.Success();
        }

        public ICommand Build()
        {
            ArgumentNullException.ThrowIfNull(_path);
            return new DeleteCommand(_path);
        }
    }
}

#pragma warning restore CA1034