using Itmo.ObjectOrientedProgramming.Lab4.Core.Entities.FileSystemUnits;

namespace Itmo.ObjectOrientedProgramming.Lab4.Core.Entities.FileSystems;

public interface IFileSystem
{
    public CoreDirectory TreeList(string path, int depth = 1);

    public string Show(string path);

    void Move(string sourcePath, string destinationPath);

    void Copy(string sourcePath, string destinationPath);

    void Delete(string path);

    void Rename(string path, string name);
}