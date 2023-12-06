using Itmo.ObjectOrientedProgramming.Lab1.Entities.Deflectors;

namespace Itmo.ObjectOrientedProgramming.Lab1.Entities.Obstacles;

public abstract class ObstacleBase
{
    protected ObstacleBase(int hitPoints)
    {
        HitPoints = hitPoints;
    }

    public bool IsAlive { get; private set; } = true;

    public int HitPoints { get; private set; }

    public abstract void CollideWith(DeflectorBase deflector);

    public virtual void TakeDamage(int hitPoints)
    {
        HitPoints -= hitPoints;

        if (HitPoints <= 0)
        {
            IsAlive = false;
        }
    }

    public void Destroy()
    {
        IsAlive = false;
        HitPoints = 0;
    }
}