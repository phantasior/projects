using System;

namespace Itmo.ObjectOrientedProgramming.Lab3.Services.Writers;

public class ConsoleWriter : IWriter
{
    public void Write(string text)
    {
        Console.WriteLine(text);
    }
}