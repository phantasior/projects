using System;
using System.Collections.ObjectModel;
using Itmo.ObjectOrientedProgramming.Lab1.Entities.Obstacles;

namespace Itmo.ObjectOrientedProgramming.Lab1.Models.Environments;

public class NitrinoNebulae : IEnvironment
{
    public NitrinoNebulae(params SpaceWhale[] whales)
    {
        Obstacles = new Collection<ObstacleBase>();
        whales = whales ?? throw new ArgumentNullException(nameof(whales));

        foreach (SpaceWhale whale in whales)
        {
            Obstacles.Add(whale);
        }
    }

    public Collection<ObstacleBase> Obstacles { get; }
}