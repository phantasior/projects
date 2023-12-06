using System;
using Itmo.ObjectOrientedProgramming.Lab1.Entities.Obstacles;
using Itmo.ObjectOrientedProgramming.Lab1.Models;

namespace Itmo.ObjectOrientedProgramming.Lab1.Entities.Deflectors;

public abstract class DeflectorBase
{
    protected DeflectorBase(int hitPoints = 0)
    {
        HitPoints = hitPoints;
    }

    public bool IsAlive => HitPoints > 0;

    private int HitPoints { get; set; }

    public virtual CrashResult TakeDamage(Explosion explosion)
    {
        return explosion?.IsAlive == true ? CrashResult.CrewDeath : CrashResult.Success;
    }

    public virtual CrashResult TakeDamage(Meteor meteor)
    {
        meteor = meteor ?? throw new ArgumentNullException(nameof(meteor));

        int damage = Math.Min(HitPoints, meteor.HitPoints);
        HitPoints -= damage;
        meteor.TakeDamage(damage);

        return CrashResult.Success;
    }

    public virtual CrashResult TakeDamage(SpaceWhale whale)
    {
        Die();
        return CrashResult.Success;
    }

    public virtual CrashResult TakeDamage(NullObstacle none)
    {
        return CrashResult.Success;
    }

    private void Die() => HitPoints = 0;
}