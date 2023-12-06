using Itmo.ObjectOrientedProgramming.Lab4.Core.Entities.Commands;

namespace Itmo.ObjectOrientedProgramming.Lab4.Core.Entities;

public interface IFileSystemManager
{
    public void Connect(string address, string mode);

    public void Disconnect();

    public void TreeList(int depth = 1);

    public void Show(string path, string mode);

    public void TreeGoTo(string path);

    public void Move(string sourcePath, string destinationPath);

    public void Copy(string sourcePath, string destinationPath);

    public void Delete(string path);

    public void Rename(string path, string name);

    public void Execute(ICommand command);
}