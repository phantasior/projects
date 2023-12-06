using Itmo.ObjectOrientedProgramming.Lab1.Entities.Obstacles;
using Itmo.ObjectOrientedProgramming.Lab1.Models;

namespace Itmo.ObjectOrientedProgramming.Lab1.Entities.Deflectors.DeflectorDecorators;

public class AntiNitrinoDeflector : DeflectorDecorator
{
    public AntiNitrinoDeflector(DeflectorBase deflector)
        : base(deflector)
    {
    }

    public override CrashResult TakeDamage(Explosion explosion)
    {
        return Deflector.TakeDamage(explosion);
    }

    public override CrashResult TakeDamage(Meteor meteor)
    {
        return Deflector.TakeDamage(meteor);
    }

    public override CrashResult TakeDamage(SpaceWhale whale)
    {
        whale?.Destroy();
        return CrashResult.Success;
    }
}