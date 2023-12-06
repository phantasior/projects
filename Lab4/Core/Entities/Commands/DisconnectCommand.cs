using Itmo.ObjectOrientedProgramming.Lab4.Core.Models;

#pragma warning disable CA1034

namespace Itmo.ObjectOrientedProgramming.Lab4.Core.Entities.Commands;

public class DisconnectCommand : ICommand
{
    private DisconnectCommand() { }

    public static Builder GetBuilder => new Builder();

    public void Execute(IFileSystemManager fileSystem)
    {
        fileSystem?.Disconnect();
    }

    public class Builder : IBuilder
    {
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
            return new BuildResult.Success();
        }

        public ICommand Build()
        {
            return new DisconnectCommand();
        }
    }
}

#pragma warning restore CA1034