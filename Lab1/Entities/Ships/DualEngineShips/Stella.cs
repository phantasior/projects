using Itmo.ObjectOrientedProgramming.Lab1.Entities.Deflectors;
using Itmo.ObjectOrientedProgramming.Lab1.Entities.Deflectors.DeflectorDecorators;
using Itmo.ObjectOrientedProgramming.Lab1.Entities.Engines.ImpulseEngineBase;
using Itmo.ObjectOrientedProgramming.Lab1.Entities.Engines.JumpEngine;
using Itmo.ObjectOrientedProgramming.Lab1.Models.DefaultProps;

namespace Itmo.ObjectOrientedProgramming.Lab1.Entities.Ships.DualEngineShips;

public class Stella : DualEngineShipBase
{
    public Stella(bool isPhotonic = false)
        : base(
            new CClassImpulseEngine(),
            new OmegaEngine(),
            DefaultShipProps.StellaHitPoints,
            DefaultShipProps.StellaMass,
            isPhotonic ? new PhotonicDeflector(new Class1Deflector()) : new Class1Deflector())
    {
    }
}