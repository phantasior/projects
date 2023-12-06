using System;
using Itmo.ObjectOrientedProgramming.Lab1.Entities.Deflectors;
using Itmo.ObjectOrientedProgramming.Lab1.Entities.Engines;
using Itmo.ObjectOrientedProgramming.Lab1.Entities.Obstacles;
using Itmo.ObjectOrientedProgramming.Lab1.Models;

namespace Itmo.ObjectOrientedProgramming.Lab1.Entities.Ships.DualEngineShips;

public abstract class DualEngineShipBase : ShipBase
{
    protected DualEngineShipBase(
        IEngine firstEngine,
        IEngine secondEngine,
        int hitPoints,
        int mass,
        DeflectorBase? deflector = null)
        : base(firstEngine, hitPoints, mass, deflector)
    {
        SecondEngine = secondEngine ?? throw new ArgumentNullException(nameof(secondEngine));
    }

    private IEngine SecondEngine { get; }

    public override void CrossPath(Path path)
    {
        path = path ?? throw new ArgumentNullException(nameof(path));

        foreach (ObstacleBase obstacle in path.Environment.Obstacles)
        {
            TakeDamage(obstacle);

            if (Report.Status != TravelStatus.Success)
            {
                return;
            }
        }

        if (!Engine.CanMoveInPath(path) && !SecondEngine.CanMoveInPath(path))
        {
            Report.Status = TravelStatus.Lost;
            return;
        }

        Report.Status = TravelStatus.Success;

        Report.UsedFuel += Engine.GetFuelToCross(path);
        Report.Duration += Engine.GetDurationToCross(path);

        Report.UsedFuel += SecondEngine.GetFuelToCross(path);
        Report.Duration += SecondEngine.GetDurationToCross(path);
    }
}