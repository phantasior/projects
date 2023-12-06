using System;
using Itmo.ObjectOrientedProgramming.Lab1.Models;
using Itmo.ObjectOrientedProgramming.Lab1.Models.DefaultProps;
using Itmo.ObjectOrientedProgramming.Lab1.Models.Environments;

namespace Itmo.ObjectOrientedProgramming.Lab1.Entities.Engines.ImpulseEngineBase;

public class EClassImpulseEngine : ImpulseEngineBase
{
    public EClassImpulseEngine()
        : base(
            DefaultEngineProps.EClassEngineFuelConsumption,
            DefaultEngineProps.EClassEngineVelocity,
            DefaultEngineProps.EClassEngineFuelAmountToStart)
    {
    }

    public override bool CanMoveInPath(Path path)
    {
        path = path ?? throw new ArgumentNullException(nameof(path));

        return path.Environment switch
        {
            Space => true,
            NitrinoNebulae => true,
            HighDensityNebulae => false,
            _ => false,
        };
    }
}