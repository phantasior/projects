using System;
using System.Collections.Generic;
using System.Linq;
using Itmo.ObjectOrientedProgramming.Lab1.Entities.Ships;
using Itmo.ObjectOrientedProgramming.Lab1.Models;

namespace Itmo.ObjectOrientedProgramming.Lab1.Services;

public class TravelRunner : ITravelRunner
{
    public ShipBase GetBestShip(Route route, IReadOnlyCollection<ShipBase> ships)
    {
        route = route ?? throw new ArgumentNullException(nameof(route));
        ships = ships ?? throw new ArgumentNullException(nameof(ships));
        foreach (ShipBase ship in ships)
        {
            Run(route, ship);
        }

        return ships
            .OrderBy(ship => ship.Report.Status != TravelStatus.Success)
            .ThenBy(ship => ship.Report.UsedFuel + ship.Report.Duration)
            .First();
    }

    public void Run(Route route, ShipBase ship)
    {
        route = route ?? throw new ArgumentNullException(nameof(route));
        ship = ship ?? throw new ArgumentNullException(nameof(ship));

        foreach (Path path in route.Paths)
        {
            ship.CrossPath(path);
            if (ship.Report.Status is not TravelStatus.Success)
            {
                return;
            }
        }
    }
}