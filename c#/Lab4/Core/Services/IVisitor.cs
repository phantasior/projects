using Itmo.ObjectOrientedProgramming.Lab4.Core.Entities.FileSystemUnits;

namespace Itmo.ObjectOrientedProgramming.Lab4.Core.Services;

public interface IVisitor
{
    string Result { get; }
}

public interface IVisitor<in T>
    where T : IFileSystemUnit
{
    void Visit(T component);
}