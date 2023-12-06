using System.Collections.Generic;
using Itmo.ObjectOrientedProgramming.Lab4.Core.Services;

namespace Itmo.ObjectOrientedProgramming.Lab4.Core.Entities.FileSystemUnits;

public class CoreDirectory : IFileSystemUnit
{
    public CoreDirectory(string path, IEnumerable<IFileSystemUnit>? content = null)
    {
        Path = path;
        Content = content ?? new List<IFileSystemUnit>();
    }

    public string Path { get; }

    public IEnumerable<IFileSystemUnit> Content { get; }

    public void Accept(IVisitor visitor)
    {
        if (visitor is IVisitor<CoreDirectory> v)
        {
            v.Visit(this);
        }
    }
}