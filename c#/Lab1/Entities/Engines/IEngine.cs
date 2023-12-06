using Itmo.ObjectOrientedProgramming.Lab1.Models;

namespace Itmo.ObjectOrientedProgramming.Lab1.Entities.Engines;

public interface IEngine
{
    bool CanMoveInPath(Path path);

    int GetFuelToCross(Path path);

    int GetDurationToCross(Path path);
}