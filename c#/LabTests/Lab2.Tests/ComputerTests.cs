using System.Collections.Generic;
using System.Linq;
using Itmo.ObjectOrientedProgramming.Lab2.Entities;
using Itmo.ObjectOrientedProgramming.Lab2.Entities.Сomponents;
using Itmo.ObjectOrientedProgramming.Lab2.Exceptions;
using Itmo.ObjectOrientedProgramming.Lab2.Models;
using Itmo.ObjectOrientedProgramming.Lab2.Models.СpuModels;
using Itmo.ObjectOrientedProgramming.Lab2.Services;
using Xunit;

namespace Itmo.ObjectOrientedProgramming.Lab2.Tests;

public class ComputerTests
{
    private readonly ComponentsFactory _factory = new ComponentsDataBase().GetComponentsFactory;

    public static IEnumerable<object[]> CorrectSpecifications => new List<object[]>
    {
        new object[] { PredefinedSpecifications.GetFastCorrectSpecification },
    };

    public static IEnumerable<object[]> SpecificationWithLackOfPowerInPowerSupply => new List<object[]>
    {
        new object[] { PredefinedSpecifications.GetSpecificationWithLackOfPowerInPowerSupply },
    };

    public static IEnumerable<object[]> SpecificationWithCpuCoolingSystemWarning => new List<object[]>
    {
        new object[] { PredefinedSpecifications.GetCorrectSpecificationWithCpuCoolingSystemWarning },
    };

    [Theory]
    [MemberData(nameof(CorrectSpecifications))]
    public void CorrectBuildTest(Specification specification)
    {
        ComputerConfigurator configurator = new(_factory);
        Result<Computer, string> computer = configurator.BuildComputer(specification);

        Assert.True(computer.HasValue);
    }

    [Theory]
    [MemberData(nameof(CorrectSpecifications))]
    public void CorrectBuildBaseOnAnotherComputerTest(Specification specification)
    {
        ComputerConfigurator configurator = new(_factory);
        Result<Computer, string> computer = configurator.BuildComputer(specification);

        Cpu newCpu = new(
            name: "Intel Core i3-10105F CM123",
            coreFrequency: 1280,
            coreAmount: 8,
            socket: Sockets.Lga1200,
            hasVideoCore: false,
            supportedFrequency: new List<int> { 2113 },
            tpd: 13,
            power: 75);

        Result<Computer, string> other = configurator
            .BuildComputerBasedOnAnother(computer.Value())
            .SetCpu(newCpu)
            .Build();

        Assert.True(computer.HasValue);
        Assert.True(other.HasValue);
    }

    [Theory]
    [MemberData(nameof(SpecificationWithLackOfPowerInPowerSupply))]
    public void LackOfPowerInPowerSupplyTest(Specification specification)
    {
        ComputerConfigurator configurator = new(_factory);
        Result<Computer, string> computer = configurator.BuildComputer(specification);

        Assert.False(computer.HasValue);
    }

    [Theory]
    [MemberData(nameof(SpecificationWithCpuCoolingSystemWarning))]
    public void CpuCoolingSystemWarningTest(Specification specification)
    {
        ComputerConfigurator configurator = new(_factory);
        Result<Computer, string> computer = configurator.BuildComputer(specification);

        Assert.True(computer.HasValue);
        Assert.Single(computer.Value().Warnings);
        Assert.Equal("Possible malfunctions because of weak CPU cooling system", computer.Value().Warnings.First());
    }
}