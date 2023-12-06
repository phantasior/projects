using Itmo.ObjectOrientedProgramming.Lab1.Entities.Deflectors;
using Itmo.ObjectOrientedProgramming.Lab1.Entities.Deflectors.DeflectorDecorators;
using Itmo.ObjectOrientedProgramming.Lab1.Entities.Engines.ImpulseEngineBase;
using Itmo.ObjectOrientedProgramming.Lab1.Entities.Engines.JumpEngine;
using Itmo.ObjectOrientedProgramming.Lab1.Models.DefaultProps;

namespace Itmo.ObjectOrientedProgramming.Lab1.Entities.Ships.DualEngineShips;

public class Avgur : DualEngineShipBase
{
    public Avgur(bool isPhotonic = false)
        : base(
            new EClassImpulseEngine(),
            new AlphaEngine(),
            DefaultShipProps.AvgurHitPoints,
            DefaultShipProps.AvgurMass,
            isPhotonic ? new PhotonicDeflector(new Class3Deflector()) : new Class3Deflector())
    {
    }
}