using System;
using Itmo.ObjectOrientedProgramming.Lab4.Core.Models;

#pragma warning disable CA1034

namespace Itmo.ObjectOrientedProgramming.Lab4.Core.Entities.Commands;

public class RenameCommand : ICommand
{
    private readonly string _path;
    private readonly string _name;

    private RenameCommand(string path, string name)
    {
        _path = path;
        _name = name;
    }

    public static Builder GetBuilder => new Builder();

    public void Execute(IFileSystemManager fileSystem)
    {
        fileSystem?.Rename(_path, _name);
    }

    public class Builder : IBuilder
    {
        private string? _path;
        private string? _name;

        public Builder WithPath(string path)
        {
            _path = path;
            return this;
        }

        public Builder WithName(string name)
        {
            _name = name;
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

            if (_name is null)
                return new BuildResult.MissedRequiredArgument("name");

            return new BuildResult.Success();
        }

        public ICommand Build()
        {
            ArgumentNullException.ThrowIfNull(_path);
            ArgumentNullException.ThrowIfNull(_name);
            return new RenameCommand(_path, _name);
        }
    }
}

#pragma warning restore CA1034