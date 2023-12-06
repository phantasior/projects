using Itmo.ObjectOrientedProgramming.Lab1.Entities.Obstacles;
using Itmo.ObjectOrientedProgramming.Lab1.Models;

namespace Itmo.ObjectOrientedProgramming.Lab1.Entities.Deflectors;

public class NullDeflector : DeflectorBase
{
    public NullDeflector()
        : base(0)
    {
    }

    public override CrashResult TakeDamage(Meteor meteor)
    {
        return CrashResult.Success;
    }

    public override CrashResult TakeDamage(SpaceWhale whale)
    {
        return CrashResult.Destroy;
    }
}