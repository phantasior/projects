#pragma warning disable CA1034

using System;
using Itmo.ObjectOrientedProgramming.Lab4.Core.Models;

namespace Itmo.ObjectOrientedProgramming.Lab4.Core.Entities.Commands;

public class ShowCommand : ICommand
{
    private readonly string _path;
    private readonly string _mode;

    private ShowCommand(string path, string mode = "console")
    {
        _path = path;
        _mode = mode;
    }

    public static Builder GetBuilder => new Builder();

    public void Execute(IFileSystemManager fileSystem)
    {
        fileSystem?.Show(_path, _mode);
    }

    public class Builder : IBuilder
    {
        private string? _path;
        private string? _mode;

        public Builder WithPath(string path)
        {
            _path = path;
            return this;
        }

        public IBuilder WithArgument(string argument, string value)
        {
            if (argument == "-m")
            {
                _mode = value;
            }

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

            if (_mode is null)
                return new BuildResult.MissedRequiredArgument("mode");

            return new BuildResult.Success();
        }

        public ICommand Build()
        {
            ArgumentNullException.ThrowIfNull(_path);
            ArgumentNullException.ThrowIfNull(_mode);
            return new ShowCommand(_path, _mode);
        }
    }
}

#pragma warning restore CA1034