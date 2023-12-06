using System;
using Itmo.ObjectOrientedProgramming.Lab1.Models.Environments;

namespace Itmo.ObjectOrientedProgramming.Lab1.Models;

public class Path
{
    public Path(IEnvironment environment, int distance)
    {
        Environment = environment ?? throw new ArgumentNullException(nameof(environment));
        Distance = distance;
    }

    public int Distance { get; }

    public IEnvironment Environment { get; }
}