using Itmo.ObjectOrientedProgramming.Lab1.Entities.Deflectors;
using Itmo.ObjectOrientedProgramming.Lab1.Entities.Deflectors.DeflectorDecorators;
using Itmo.ObjectOrientedProgramming.Lab1.Entities.Engines.ImpulseEngineBase;
using Itmo.ObjectOrientedProgramming.Lab1.Models.DefaultProps;

namespace Itmo.ObjectOrientedProgramming.Lab1.Entities.Ships;

public class Meredian : ShipBase
{
    public Meredian(bool isPhotonic = false)
        : base(
            new EClassImpulseEngine(),
            DefaultShipProps.MeredianHitPoints,
            DefaultShipProps.MeredianMass,
            isPhotonic ? new PhotonicDeflector(new AntiNitrinoDeflector(new Class2Deflectors())) : new AntiNitrinoDeflector(new Class2Deflectors()))
    {
    }
}