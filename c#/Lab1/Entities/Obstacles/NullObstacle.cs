using Itmo.ObjectOrientedProgramming.Lab1.Entities.Deflectors;

namespace Itmo.ObjectOrientedProgramming.Lab1.Entities.Obstacles;

public class NullObstacle : ObstacleBase
{
    public NullObstacle()
        : base(0)
    {
    }

    public override void TakeDamage(int hitPoints)
    {
    }

    public override void CollideWith(DeflectorBase deflector)
    {
        deflector?.TakeDamage(this);
    }
}