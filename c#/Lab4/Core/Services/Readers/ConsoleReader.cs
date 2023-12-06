using System;

namespace Itmo.ObjectOrientedProgramming.Lab4.Core.Services.Readers;

public class ConsoleReader : IReader<string>
{
    public string Read()
    {
        return Console.ReadLine() ?? string.Empty;
    }
}