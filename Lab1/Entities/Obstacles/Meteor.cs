using Itmo.ObjectOrientedProgramming.Lab1.Entities.Deflectors;
using Itmo.ObjectOrientedProgramming.Lab1.Models.DefaultProps;

namespace Itmo.ObjectOrientedProgramming.Lab1.Entities.Obstacles;

public class Meteor : ObstacleBase
{
    public Meteor()
        : base(DefaultObstaclesProps.MeteorHitPoints)
    {
    }

    public override void CollideWith(DeflectorBase deflector)
    {
        deflector?.TakeDamage(this);
    }
}