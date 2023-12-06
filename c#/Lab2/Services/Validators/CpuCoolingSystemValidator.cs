using System;
using Itmo.ObjectOrientedProgramming.Lab2.Exceptions;
using Itmo.ObjectOrientedProgramming.Lab2.Models;

namespace Itmo.ObjectOrientedProgramming.Lab2.Services.Validators;

public class CpuCoolingSystemValidator : IComputerValidator
{
    public Result<BuildStatus, string> Validate(ComputerBuilder builder)
    {
        builder = builder ?? throw new ArgumentNullException(nameof(builder));
        builder.Cpu = builder.Cpu ?? throw new ArgumentNullException(nameof(builder));
        if (builder.CpuCoolingSystem?.MaxTdp < builder.Cpu.Tpd)
        {
            builder.Warnings.Add("Possible malfunctions because of weak CPU cooling system");
            return BuildStatus.Warning;
        }

        return BuildStatus.Success;
    }
}