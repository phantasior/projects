using Itmo.ObjectOrientedProgramming.Lab1.Entities.Deflectors;
using Itmo.ObjectOrientedProgramming.Lab1.Entities.Deflectors.DeflectorDecorators;
using Itmo.ObjectOrientedProgramming.Lab1.Entities.Engines.ImpulseEngineBase;
using Itmo.ObjectOrientedProgramming.Lab1.Models.DefaultProps;

namespace Itmo.ObjectOrientedProgramming.Lab1.Entities.Ships;

public class Shuttle : ShipBase
{
    public Shuttle(bool isPhotonic = false)
        : base(
            new CClassImpulseEngine(),
            DefaultShipProps.ShuttleHitPoints,
            DefaultShipProps.ShuttleMass,
            isPhotonic ? new AntiNitrinoDeflector(new NullDeflector()) : null)
    {
    }
}