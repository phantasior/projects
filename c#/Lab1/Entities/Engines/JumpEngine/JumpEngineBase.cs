using System;
using Itmo.ObjectOrientedProgramming.Lab1.Models;

namespace Itmo.ObjectOrientedProgramming.Lab1.Entities.Engines.JumpEngine;

public abstract class JumpEngineBase : IEngine
{
    private readonly int _velocity;

    protected JumpEngineBase(int fuelConsumption, int velocity)
    {
        FuelConsumption = fuelConsumption;
        _velocity = velocity;
    }

    public abstract int DrivingReserve { get; }

    protected int FuelConsumption { get; }

    public abstract bool CanMoveInPath(Path path);

    public abstract int GetFuelToCross(Path path);

    public virtual int GetDurationToCross(Path path)
    {
        path = path ?? throw new ArgumentNullException(nameof(path));
        return _velocity * path.Distance;
    }
}