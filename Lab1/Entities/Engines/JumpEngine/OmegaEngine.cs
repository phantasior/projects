using System;
using Itmo.ObjectOrientedProgramming.Lab1.Models;
using Itmo.ObjectOrientedProgramming.Lab1.Models.DefaultProps;
using Itmo.ObjectOrientedProgramming.Lab1.Models.Environments;

namespace Itmo.ObjectOrientedProgramming.Lab1.Entities.Engines.JumpEngine;

public class OmegaEngine : JumpEngineBase
{
    public OmegaEngine()
        : base(
            DefaultEngineProps.OmegaEngineFuelConsumption,
            DefaultEngineProps.OmegaEngineVelocity)
    {
    }

    public override int DrivingReserve => DefaultEngineProps.OmegaEngineDrivingReserve;

    public override int GetFuelToCross(Path path)
    {
        path = path ?? throw new ArgumentNullException(nameof(path));
        return path.Distance * path.Distance * FuelConsumption;
    }

    public override bool CanMoveInPath(Path path)
    {
        path = path ?? throw new ArgumentNullException(nameof(path));

        return path.Environment switch
        {
            HighDensityNebulae env => DrivingReserve >= path.Distance,
            Space => false,
            NitrinoNebulae => false,
            _ => false,
        };
    }
}