using Itmo.ObjectOrientedProgramming.Lab4.Core.Services;

namespace Itmo.ObjectOrientedProgramming.Lab4.Core.Entities.FileSystemUnits;

public class CoreFile : IFileSystemUnit
{
    public CoreFile(string path)
    {
        Path = path;
    }

    public string Path { get; }

    public void Accept(IVisitor visitor)
    {
        if (visitor is IVisitor<CoreFile> v)
        {
            v.Visit(this);
        }
    }
}