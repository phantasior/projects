using System;
using Itmo.ObjectOrientedProgramming.Lab1.Models;

namespace Itmo.ObjectOrientedProgramming.Lab1.Entities.Engines.ImpulseEngineBase;

public abstract class ImpulseEngineBase : IEngine
{
    private readonly int _fuelAmountToStart;

    private readonly int _fuelConsumption;

    private readonly int _velocity;

    protected ImpulseEngineBase(int fuelConsumption, int velocity, int fuelAmountToStart)
    {
        _fuelConsumption = fuelConsumption;
        _fuelAmountToStart = fuelAmountToStart;
        _velocity = velocity;
    }

    public abstract bool CanMoveInPath(Path path);

    public virtual int GetFuelToCross(Path path)
    {
        path = path ?? throw new ArgumentNullException(nameof(path));
        return (_fuelConsumption * path.Distance) + _fuelAmountToStart;
    }

    public virtual int GetDurationToCross(Path path)
    {
        path = path ?? throw new ArgumentNullException(nameof(path));
        return _velocity * path.Distance;
    }
}