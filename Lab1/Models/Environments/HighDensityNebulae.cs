using System;
using System.Collections.ObjectModel;
using Itmo.ObjectOrientedProgramming.Lab1.Entities.Obstacles;

namespace Itmo.ObjectOrientedProgramming.Lab1.Models.Environments;

public class HighDensityNebulae : IEnvironment
{
    public HighDensityNebulae(params Explosion[] explosions)
    {
        Obstacles = new Collection<ObstacleBase>();
        explosions = explosions ?? throw new ArgumentNullException(nameof(explosions));

        foreach (Explosion explosion in explosions)
        {
            Obstacles.Add(explosion);
        }
    }

    public Collection<ObstacleBase> Obstacles { get; }
}