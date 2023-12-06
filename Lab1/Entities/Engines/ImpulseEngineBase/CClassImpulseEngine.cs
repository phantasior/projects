using System;
using Itmo.ObjectOrientedProgramming.Lab1.Models;
using Itmo.ObjectOrientedProgramming.Lab1.Models.DefaultProps;
using Itmo.ObjectOrientedProgramming.Lab1.Models.Environments;

namespace Itmo.ObjectOrientedProgramming.Lab1.Entities.Engines.ImpulseEngineBase;

public class CClassImpulseEngine : ImpulseEngineBase
{
    public CClassImpulseEngine()
        : base(
            DefaultEngineProps.CClassEngineFuelConsumption,
            DefaultEngineProps.CClassEngineVelocity,
            DefaultEngineProps.CClassEngineFuelAmountToStart)
    {
    }

    public override bool CanMoveInPath(Path path)
    {
        path = path ?? throw new ArgumentNullException(nameof(path));

        return path.Environment switch
        {
            Space => true,
            HighDensityNebulae => false,
            NitrinoNebulae => false,
            _ => false,
        };
    }
}