using System.Collections.Generic;
using Itmo.ObjectOrientedProgramming.Lab1.Entities.Ships;
using Itmo.ObjectOrientedProgramming.Lab1.Models;

namespace Itmo.ObjectOrientedProgramming.Lab1.Services;

public interface ITravelRunner
{
    ShipBase GetBestShip(Route route, IReadOnlyCollection<ShipBase> ships);

    void Run(Route route, ShipBase ship);
}