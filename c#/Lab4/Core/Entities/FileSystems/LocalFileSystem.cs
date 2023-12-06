using System.IO;
using System.Linq;
using Itmo.ObjectOrientedProgramming.Lab4.Core.Entities.FileSystemUnits;
using File = System.IO.File;

namespace Itmo.ObjectOrientedProgramming.Lab4.Core.Entities.FileSystems;

public class LocalFileSystem : IFileSystem
{
    public CoreDirectory TreeList(string path, int depth = 1)
    {
        if (depth == 0)
            return new CoreDirectory(path);

        string[] files = Directory.GetFiles(path);
        var content = files.Select(
            file => new CoreFile(Path.Combine(path, file))).Cast<IFileSystemUnit>().ToList();

        string[] subdirectories = Directory.GetDirectories(path);
        content.AddRange(subdirectories.Select(
            subdirectory => TreeList(subdirectory, depth - 1)));

        return new CoreDirectory(path, content);
    }

    public string Show(string path)
    {
        return File.ReadAllText(path);
    }

    public void Move(string sourcePath, string destinationPath)
    {
        File.Move(sourcePath, destinationPath);
    }

    public void Copy(string sourcePath, string destinationPath)
    {
        File.Copy(sourcePath, destinationPath);
    }

    public void Delete(string path)
    {
        File.Delete(path);
    }

    public void Rename(string path, string name)
    {
        string? oldPath = Path.GetDirectoryName(path);
        string newPath = oldPath is null ? name : Path.Combine(oldPath, name);
        File.Move(path, newPath);
    }
}