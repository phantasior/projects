using Itmo.ObjectOrientedProgramming.Lab4.Core.Services;

namespace Itmo.ObjectOrientedProgramming.Lab4.Core.Entities.FileSystemUnits;

public interface IFileSystemUnit
{
    string Path { get; }

    void Accept(IVisitor visitor);
}