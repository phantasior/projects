using System;
using System.Linq;
using Itmo.ObjectOrientedProgramming.Lab2.Exceptions;
using Itmo.ObjectOrientedProgramming.Lab2.Models;

namespace Itmo.ObjectOrientedProgramming.Lab2.Services.Validators;

public class CpuValidator : IComputerValidator
{
    public Result<BuildStatus, string> Validate(ComputerBuilder builder)
    {
        builder = builder ?? throw new ArgumentNullException(nameof(builder));
        builder.Cpu = builder.Cpu ?? throw new ArgumentNullException(nameof(builder));
        builder.MotherBoard = builder.MotherBoard ?? throw new ArgumentNullException(nameof(builder));
        if (builder.Cpu.Socket != builder.MotherBoard.Socket)
        {
            return "CPU and motherboard have different sockets";
        }

        if (builder.Bios is not null && builder.Bios.SupportedCpu.Contains(builder.Cpu))
        {
            return "CPU not supported by BIOS";
        }

        return BuildStatus.Success;
    }
}