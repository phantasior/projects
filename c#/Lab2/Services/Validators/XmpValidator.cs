using System;
using Itmo.ObjectOrientedProgramming.Lab2.Exceptions;
using Itmo.ObjectOrientedProgramming.Lab2.Models;

namespace Itmo.ObjectOrientedProgramming.Lab2.Services.Validators;

public class XmpValidator : IComputerValidator
{
    public Result<BuildStatus, string> Validate(ComputerBuilder builder)
    {
        builder = builder ?? throw new ArgumentNullException(nameof(builder));
        builder.MotherBoard = builder.MotherBoard ?? throw new ArgumentNullException(nameof(builder));
        if (!builder.MotherBoard.ChipSet.HasXmpSupport)
        {
            return "Your motherboard doesn't support XMP";
        }

        return BuildStatus.Success;
    }
}