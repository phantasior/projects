using System;
using Itmo.ObjectOrientedProgramming.Lab2.Entities.Сomponents;
using Itmo.ObjectOrientedProgramming.Lab2.Exceptions;
using Itmo.ObjectOrientedProgramming.Lab2.Models;

namespace Itmo.ObjectOrientedProgramming.Lab2.Services.Validators;

public class RamValidator : IComputerValidator
{
    public Result<BuildStatus, string> Validate(ComputerBuilder builder)
    {
        builder = builder ?? throw new ArgumentNullException(nameof(builder));
        builder.MotherBoard = builder.MotherBoard ?? throw new ArgumentNullException(nameof(builder));
        foreach (RamStick ramStick in builder.RamSticks)
        {
            if (builder.MotherBoard.DdrVersion != ramStick.DdrVersion)
            {
                return $"RAM supports {ramStick.DdrVersion} but motherboard has {builder.MotherBoard.DdrVersion}";
            }
        }

        return BuildStatus.Success;
    }
}