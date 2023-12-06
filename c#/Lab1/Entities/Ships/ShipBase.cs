using System;
using System.ComponentModel.DataAnnotations;
using Itmo.ObjectOrientedProgramming.Lab1.Entities.Deflectors;
using Itmo.ObjectOrientedProgramming.Lab1.Entities.Engines;
using Itmo.ObjectOrientedProgramming.Lab1.Entities.Obstacles;
using Itmo.ObjectOrientedProgramming.Lab1.Models;

namespace Itmo.ObjectOrientedProgramming.Lab1.Entities.Ships;

public abstract class ShipBase
{
    protected ShipBase(
        IEngine engine,
        int hitPoints,
        int mass,
        DeflectorBase? deflector = null)
    {
        Engine = engine ?? throw new ArgumentNullException(nameof(engine));
        HitPoints = hitPoints;
        Mass = mass;
        Deflector = deflector ?? new NullDeflector();
    }

    public IEngine Engine { get; }

    public DeflectorBase Deflector { get; }

    public ShipReport Report { get; protected set; } = new();

    [Range(1, 100, ErrorMessage = "Value for {0} must be between {1} and {2}.")]
    private int HitPoints { get; set; }

    [Range(1, int.MaxValue, ErrorMessage = "Value for {0} must be between {1} and {2}.")]
    private int Mass { get; set; }

    public virtual void CrossPath(Path path)
    {
        path = path ?? throw new ArgumentNullException(nameof(path));

        foreach (ObstacleBase obstacle in path.Environment.Obstacles)
        {
            TakeDamage(obstacle);

            if (Report.Status != TravelStatus.Success)
            {
                return;
            }
        }

        if (!Engine.CanMoveInPath(path))
        {
            Report.Status = TravelStatus.Lost;
            return;
        }

        Report.Status = TravelStatus.Success;
        Report.UsedFuel += Engine.GetFuelToCross(path);
        Report.Duration += Engine.GetDurationToCross(path);
    }

    protected void TakeDamage(ObstacleBase obstacle)
    {
        obstacle = obstacle ?? throw new ArgumentNullException(nameof(obstacle));

        obstacle.CollideWith(Deflector);
        HitPoints -= obstacle.HitPoints;

        if (!obstacle.IsAlive)
        {
            return;
        }

        Report.Status = obstacle switch
        {
            Meteor => TravelStatus.Destroy,
            Explosion => TravelStatus.CrewDeath,
            SpaceWhale => !Deflector.IsAlive ? TravelStatus.Destroy : TravelStatus.Success,
            _ => Report.Status,
        };
    }
}