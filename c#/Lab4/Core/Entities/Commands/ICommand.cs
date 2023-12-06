namespace Itmo.ObjectOrientedProgramming.Lab4.Core.Entities.Commands;

public interface ICommand
{
    void Execute(IFileSystemManager fileSystem);
}