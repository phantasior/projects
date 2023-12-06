using System;
using Itmo.ObjectOrientedProgramming.Lab2.Exceptions;
using Itmo.ObjectOrientedProgramming.Lab2.Models;

namespace Itmo.ObjectOrientedProgramming.Lab2.Services.Validators;

public class WifiAdapterValidator : IComputerValidator
{
    public Result<BuildStatus, string> Validate(ComputerBuilder builder)
    {
        builder = builder ?? throw new ArgumentNullException(nameof(builder));
        builder.MotherBoard = builder.MotherBoard ?? throw new ArgumentNullException(nameof(builder));

        if (builder.WifiAdapter is not null && builder.MotherBoard.HasInternalWifi)
        {
            return "You can't setup wifi adapter because your motherboard has internal one";
        }

        return BuildStatus.Success;
    }
}