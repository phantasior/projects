using System;

namespace Itmo.ObjectOrientedProgramming.Lab4.Core.Services.Writers;

public class ConsoleWriter : IWriter
{
    public void Write(string text)
    {
        Console.WriteLine(text);
    }
}