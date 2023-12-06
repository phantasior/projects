using System;
using System.IO;
using System.Linq;
using System.Text;
using Itmo.ObjectOrientedProgramming.Lab4.Core.Entities.FileSystemUnits;
using Itmo.ObjectOrientedProgramming.Lab4.Core.Models;

namespace Itmo.ObjectOrientedProgramming.Lab4.Core.Services;

public class FileSystemVisitor : IVisitor, IVisitor<CoreDirectory>, IVisitor<CoreFile>
{
    private readonly StringBuilder _sb = new();
    private int _depth = 1;

    public string Result => _sb.ToString();

    public void Visit(CoreDirectory component)
    {
        ArgumentNullException.ThrowIfNull(component);

        _sb.Append(Properties.MainSeparator, _depth - 1);
        _sb.Append(Properties.LastSeparator);
        _sb.Append(Path.DirectorySeparatorChar);
        _sb.Append(component.Path.Split(Path.DirectorySeparatorChar).Last());
        _sb.Append(Properties.DirectoryImage);
        _sb.Append(Path.DirectorySeparatorChar);
        _sb.Append('\n');

        _depth += Properties.SeparatorLen;

        foreach (IFileSystemUnit unit in component.Content)
        {
            unit.Accept(this);
        }

        _depth -= Properties.SeparatorLen;
    }

    public void Visit(CoreFile component)
    {
        ArgumentNullException.ThrowIfNull(component);

        _sb.Append(' ', _depth - 1);
        _sb.Append(Properties.VerticalSeparator);
        _sb.Append(Path.GetFileName(component.Path));
        _sb.Append(Properties.FileImage);
        _sb.Append('\n');
    }
}