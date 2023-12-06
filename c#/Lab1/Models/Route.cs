using System.Collections.Generic;

namespace Itmo.ObjectOrientedProgramming.Lab1.Models;

public class Route
{
    public Route(params Path[] paths)
    {
        Paths = new List<Path>(paths);
    }

    public IList<Path> Paths { get; }
}