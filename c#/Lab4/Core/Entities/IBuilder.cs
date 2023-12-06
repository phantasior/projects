using Itmo.ObjectOrientedProgramming.Lab4.Core.Entities.Commands;
using Itmo.ObjectOrientedProgramming.Lab4.Core.Models;

namespace Itmo.ObjectOrientedProgramming.Lab4.Core.Entities;

public interface IBuilder
{
    ICommand Build();

    IBuilder WithFlag(string flag);

    IBuilder WithArgument(string argument, string value);

    BuildResult Validate();
}