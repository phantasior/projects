using System;
using Itmo.ObjectOrientedProgramming.Lab1.Entities.Obstacles;
using Itmo.ObjectOrientedProgramming.Lab1.Models;

namespace Itmo.ObjectOrientedProgramming.Lab1.Entities.Deflectors.DeflectorDecorators;

public class PhotonicDeflector : DeflectorDecorator
{
    private int _hitsCanTake = 3;

    public PhotonicDeflector(DeflectorBase deflector)
        : base(deflector)
    {
    }

    public override CrashResult TakeDamage(Explosion explosion)
    {
        explosion = explosion ?? throw new ArgumentNullException(nameof(explosion));
        if (_hitsCanTake == 0)
        {
            return CrashResult.CrewDeath;
        }

        --_hitsCanTake;
        explosion.Destroy();
        return Deflector.TakeDamage(explosion);
    }

    public override CrashResult TakeDamage(Meteor meteor)
    {
        return Deflector.TakeDamage(meteor);
    }

    public override CrashResult TakeDamage(SpaceWhale whale)
    {
        return Deflector.TakeDamage(whale);
    }
}