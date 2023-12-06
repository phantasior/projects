#pragma warning disable CA1034

using System;
using Itmo.ObjectOrientedProgramming.Lab4.Core.Models;

namespace Itmo.ObjectOrientedProgramming.Lab4.Core.Entities.Commands;

public class ListCommand : ICommand
{
    private readonly int _depth;

    private ListCommand(int? depth)
    {
        _depth = depth ?? 1;
    }

    public static Builder GetBuilder => new Builder();

    public void Execute(IFileSystemManager fileSystem)
    {
        fileSystem?.TreeList(_depth);
    }

    public class Builder : IBuilder
    {
        private int? _depth;

        public IBuilder WithArgument(string argument, string value)
        {
            if (argument == "-d")
            {
                if (!int.TryParse(value, out int depth))
                {
                    throw new ArgumentException("depth should be int type");
                }

                _depth = depth;
            }

            return this;
        }

        public IBuilder WithFlag(string flag)
        {
            return this;
        }

        public BuildResult Validate()
        {
            return new BuildResult.Success();
        }

        public ICommand Build()
        {
            return new ListCommand(_depth);
        }
    }
}

#pragma warning restore CA1034