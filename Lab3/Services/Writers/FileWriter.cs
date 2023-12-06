using System.IO;

namespace Itmo.ObjectOrientedProgramming.Lab3.Services.Writers;

public class FileWriter : IWriter
{
    private readonly StreamWriter _writer;

    public FileWriter(StreamWriter writer)
    {
        _writer = writer;
    }

    public void Write(string text)
    {
        _writer.WriteLine(text);
    }
}