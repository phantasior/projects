using System;
using System.Collections.Generic;
using System.IO;
using Itmo.ObjectOrientedProgramming.Lab4.Core.Entities.Commands;
using Itmo.ObjectOrientedProgramming.Lab4.Core.Entities.FileSystems;
using Itmo.ObjectOrientedProgramming.Lab4.Core.Entities.FileSystemUnits;
using Itmo.ObjectOrientedProgramming.Lab4.Core.Services;
using Itmo.ObjectOrientedProgramming.Lab4.Core.Services.Writers;

namespace Itmo.ObjectOrientedProgramming.Lab4.Core.Entities;

public class FileSystemManager : IFileSystemManager
{
    private readonly Dictionary<string, Func<IFileSystem>> _availableFileSystems;
    private readonly Dictionary<string, Func<IWriter>> _availableShowModes;
    private readonly IWriter _writer;
    private string? _workingDirectory;
    private IFileSystem? _implementation;

    public FileSystemManager(
        Dictionary<string, Func<IFileSystem>> availableFileSystems,
        Dictionary<string, Func<IWriter>> availableShowModes,
        IWriter writer)
    {
        _availableFileSystems = availableFileSystems;
        _availableShowModes = availableShowModes;
        _writer = writer;
    }

    public void Connect(string address, string mode)
    {
        if (!Directory.Exists(address))
        {
            _writer.Write("No such directory");
            return;
        }

        if (!_availableFileSystems.ContainsKey(mode))
        {
            _writer.Write("No such mode available");
            return;
        }

        _workingDirectory = address;
        _implementation = _availableFileSystems[mode]();
    }

    public void Disconnect()
    {
        _workingDirectory = null;
        _implementation = null;
    }

    public void TreeList(int depth)
    {
        if (_workingDirectory is null || _implementation is null)
        {
            _writer.Write("You should connect before use tree list");
            return;
        }

        try
        {
            CoreDirectory directory = _implementation.TreeList(_workingDirectory, depth);
            var visitor = new FileSystemVisitor();
            directory.Accept(visitor);
            _writer.Write(visitor.Result);
        }
        catch (IOException exception)
        {
            _writer.Write(exception.Message);
        }
    }

    public void Show(string path, string mode)
    {
        if (!_availableShowModes.ContainsKey(mode))
        {
            _writer.Write("There is no such mode to show");
            return;
        }

        if (_implementation is null)
        {
            _writer.Write("You should connect before use it");
            return;
        }

        IWriter writer = _availableShowModes[mode]();

        try
        {
            string? result = _implementation.Show(NormalizePath(path));
            writer.Write(result);
        }
        catch (IOException exception)
        {
            Console.Write(exception.Message);
        }
    }

    public void TreeGoTo(string path)
    {
        if (_workingDirectory is null)
        {
            _writer.Write("You should connect before use tree list");
            return;
        }

        _workingDirectory = NormalizePath(path);
    }

    public void Move(string sourcePath, string destinationPath)
    {
        if (_implementation is null)
        {
            _writer.Write("You should connect before use tree list");
            return;
        }

        try
        {
            _implementation.Move(NormalizePath(sourcePath), NormalizePath(destinationPath));
        }
        catch (IOException exception)
        {
            _writer.Write(exception.Message);
        }
    }

    public void Copy(string sourcePath, string destinationPath)
    {
        if (_implementation is null)
        {
            _writer.Write("You should connect before use tree list");
            return;
        }

        try
        {
            _implementation.Copy(NormalizePath(sourcePath), NormalizePath(destinationPath));
        }
        catch (IOException exception)
        {
            _writer.Write(exception.Message);
        }
    }

    public void Delete(string path)
    {
        if (_implementation is null)
        {
            _writer.Write("You should connect before use tree list");
            return;
        }

        try
        {
            _implementation.Delete(NormalizePath(path));
        }
        catch (IOException exception)
        {
            _writer.Write(exception.Message);
        }
    }

    public void Rename(string path, string name)
    {
        if (_implementation is null)
        {
            _writer.Write("You should connect before use tree list");
            return;
        }

        _implementation.Rename(NormalizePath(path), name);
    }

    public void Execute(ICommand command)
    {
        ArgumentNullException.ThrowIfNull(command);
        command.Execute(this);
    }

    public void PrintDirectory()
    {
        if (_workingDirectory is not null)
            Console.Write(_workingDirectory);

        Console.Write("> ");
    }

    private string NormalizePath(string path)
    {
        if (_workingDirectory is null)
            return path;

        return Path.IsPathRooted(path) ? path : Path.Combine(_workingDirectory, path);
    }
}