#pragma warning disable CA1034

using System;
using Itmo.ObjectOrientedProgramming.Lab4.Core.Models;

namespace Itmo.ObjectOrientedProgramming.Lab4.Core.Entities.Commands;

public class ConnectCommand : ICommand
{
    private readonly string _address;
    private readonly string _mode;

    private ConnectCommand(string address, string mode)
    {
        _address = address;
        _mode = mode;
    }

    public static Builder GetBuilder => new Builder();

    public void Execute(IFileSystemManager fileSystem)
    {
        fileSystem?.Connect(_address, _mode);
    }

    public class Builder : IBuilder
    {
        private string? _address;
        private string? _mode;

        public Builder WithAddress(string address)
        {
            _address = address;
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
            if (_address is null)
                return new BuildResult.MissedRequiredArgument("address");

            if (_mode is null)
                return new BuildResult.MissedRequiredArgument("mode");

            return new BuildResult.Success();
        }

        public ICommand Build()
        {
            ArgumentNullException.ThrowIfNull(_address);
            ArgumentNullException.ThrowIfNull(_mode);
            return new ConnectCommand(_address, _mode);
        }
    }
}

#pragma warning restore CA1034