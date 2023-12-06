using Itmo.ObjectOrientedProgramming.Lab1.Entities.Deflectors;
using Itmo.ObjectOrientedProgramming.Lab1.Models.DefaultProps;

namespace Itmo.ObjectOrientedProgramming.Lab1.Entities.Obstacles;

public class SpaceWhale : ObstacleBase
{
    public SpaceWhale()
        : base(DefaultObstaclesProps.SpaceWhaleHitPoints)
    {
    }

    public override void CollideWith(DeflectorBase deflector)
    {
        deflector?.TakeDamage(this);
    }
}