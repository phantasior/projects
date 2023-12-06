using Itmo.ObjectOrientedProgramming.Lab1.Entities.Deflectors;
using Itmo.ObjectOrientedProgramming.Lab1.Entities.Deflectors.DeflectorDecorators;
using Itmo.ObjectOrientedProgramming.Lab1.Entities.Engines.ImpulseEngineBase;
using Itmo.ObjectOrientedProgramming.Lab1.Entities.Engines.JumpEngine;
using Itmo.ObjectOrientedProgramming.Lab1.Models.DefaultProps;

namespace Itmo.ObjectOrientedProgramming.Lab1.Entities.Ships.DualEngineShips;

public class Vaklas : DualEngineShipBase
{
    public Vaklas(bool isPhotonic = false)
        : base(
            new EClassImpulseEngine(),
            new GammaEngine(),
            DefaultShipProps.VaklasHitPoints,
            DefaultShipProps.VaklasMass,
            isPhotonic ? new PhotonicDeflector(new Class1Deflector()) : new Class1Deflector())
    {
    }
}