using System.Collections.ObjectModel;
using Itmo.ObjectOrientedProgramming.Lab1.Entities.Obstacles;

namespace Itmo.ObjectOrientedProgramming.Lab1.Models.Environments;

public interface IEnvironment
{
    public Collection<ObstacleBase> Obstacles { get; }
}