using Itmo.ObjectOrientedProgramming.Lab1.Entities.Deflectors;
using Itmo.ObjectOrientedProgramming.Lab1.Models.DefaultProps;

namespace Itmo.ObjectOrientedProgramming.Lab1.Entities.Obstacles;

public class Explosion : ObstacleBase
{
    public Explosion()
        : base(DefaultObstaclesProps.ExplosionHitPoints)
    {
    }

    public override void CollideWith(DeflectorBase deflector)
    {
        deflector?.TakeDamage(this);
    }
}