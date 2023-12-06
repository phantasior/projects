using System;
using Itmo.ObjectOrientedProgramming.Lab4.Core.Models;

#pragma warning disable CA1034

namespace Itmo.ObjectOrientedProgramming.Lab4.Core.Entities.Commands;

public class MoveCommand : ICommand
{
    private readonly string _source;
    private readonly string _destination;

    private MoveCommand(string source, string destination)
    {
        _source = source;
        _destination = destination;
    }

    public static Builder GetBuilder => new Builder();

    public void Execute(IFileSystemManager fileSystem)
    {
        fileSystem?.Move(_source, _destination);
    }

    public class Builder : IBuilder
    {
        private string? _source;
        private string? _destination;

        public Builder WithSource(string source)
        {
            _source = source;
            return this;
        }

        public Builder WithDestination(string destination)
        {
            _destination = destination;
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
            if (_source is null)
                return new BuildResult.MissedRequiredArgument("source");

            if (_destination is null)
                return new BuildResult.MissedRequiredArgument("destination");

            return new BuildResult.Success();
        }

        public ICommand Build()
        {
            ArgumentNullException.ThrowIfNull(_source);
            ArgumentNullException.ThrowIfNull(_destination);
            return new MoveCommand(_source, _destination);
        }
    }
}

#pragma warning restore CA1034