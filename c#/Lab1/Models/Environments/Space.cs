using System;
using System.Collections.ObjectModel;
using Itmo.ObjectOrientedProgramming.Lab1.Entities.Obstacles;

namespace Itmo.ObjectOrientedProgramming.Lab1.Models.Environments;

public class Space : IEnvironment
{
    public Space(params Meteor[] meteors)
    {
        Obstacles = new Collection<ObstacleBase>();
        meteors = meteors ?? throw new ArgumentNullException(nameof(meteors));

        foreach (Meteor meteor in meteors)
        {
            Obstacles.Add(meteor);
        }
    }

    public Collection<ObstacleBase> Obstacles { get; }
}