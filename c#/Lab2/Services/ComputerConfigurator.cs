using System;
using System.Collections.Generic;
using Itmo.ObjectOrientedProgramming.Lab2.Entities;
using Itmo.ObjectOrientedProgramming.Lab2.Exceptions;
using Itmo.ObjectOrientedProgramming.Lab2.Models;
using Itmo.ObjectOrientedProgramming.Lab2.Services.Validators;

namespace Itmo.ObjectOrientedProgramming.Lab2.Services;

public class ComputerConfigurator
{
    private readonly ComponentsFactory _factory;

    private readonly IList<IComputerValidator> _validators;

    public ComputerConfigurator(ComponentsFactory factory, IList<IComputerValidator>? validators = null)
    {
        _factory = factory;
        _validators = validators ?? new List<IComputerValidator>()
        {
            new ComputerCaseValidator(),
            new CpuCoolingSystemValidator(),
            new CpuValidator(),
            new RamValidator(),
            new ValidatePowerSupply(),
            new WifiAdapterValidator(),
            new XmpValidator(),
        };
    }

    public IReadOnlyCollection<IComputerValidator> Validators => _validators.AsReadOnly();

    public ComputerBuilder BuildComputerBasedOnAnother(Computer computer)
    {
        computer = computer ?? throw new ArgumentNullException(nameof(computer));
        var builder = new ComputerBuilder(Validators);
        computer.Build(builder);

        return builder;
    }

    public Result<Computer, string> BuildComputer(Specification specification)
    {
        var director = new ComputerDirector(_factory, Validators);
        return director.BuildComputer(specification);
    }
}