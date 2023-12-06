using System;
using Itmo.ObjectOrientedProgramming.Lab2.Exceptions;
using Itmo.ObjectOrientedProgramming.Lab2.Models;

namespace Itmo.ObjectOrientedProgramming.Lab2.Services.Validators;

public class ComputerCaseValidator : IComputerValidator
{
    public Result<BuildStatus, string> Validate(ComputerBuilder builder)
    {
        builder = builder ?? throw new ArgumentNullException(nameof(builder));
        builder.ComputerCase = builder.ComputerCase ?? throw new ArgumentNullException(nameof(builder));
        builder.GraphicCard = builder.GraphicCard ?? throw new ArgumentNullException(nameof(builder));
        if (builder.GraphicCard.Width > builder.ComputerCase.Width ||
            builder.GraphicCard.Height > builder.ComputerCase.Height)
        {
            return "Your GPU doesn't fit into computer case";
        }

        return BuildStatus.Success;
    }
}