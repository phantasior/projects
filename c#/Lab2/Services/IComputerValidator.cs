using Itmo.ObjectOrientedProgramming.Lab2.Exceptions;
using Itmo.ObjectOrientedProgramming.Lab2.Models;

namespace Itmo.ObjectOrientedProgramming.Lab2.Services;

public interface IComputerValidator
{
    Result<BuildStatus, string> Validate(ComputerBuilder builder);
}