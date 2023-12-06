using System;
using System.Linq;
using Itmo.ObjectOrientedProgramming.Lab2.Exceptions;
using Itmo.ObjectOrientedProgramming.Lab2.Models;

namespace Itmo.ObjectOrientedProgramming.Lab2.Services.Validators;

public class ValidatePowerSupply : IComputerValidator
{
    public Result<BuildStatus, string> Validate(ComputerBuilder builder)
    {
        builder = builder ?? throw new ArgumentNullException(nameof(builder));
        builder.PowerSupply = builder.PowerSupply ?? throw new ArgumentNullException(nameof(builder));
        builder.GraphicCard = builder.GraphicCard ?? throw new ArgumentNullException(nameof(builder));
        builder.DataStorage = builder.DataStorage ?? throw new ArgumentNullException(nameof(builder));
        if (builder.PowerSupply.MaxPower <
            builder.GraphicCard.PowerConsumption + builder.DataStorage.PowerSupply +
            builder.RamSticks.Sum(stick => stick.Power) +
            (builder.WifiAdapter?.PowerConsumption ?? 0))
        {
            return "Your power supply doesn't have enough power";
        }

        return BuildStatus.Success;
    }
}