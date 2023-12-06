using System;
using System.Collections.Generic;
using Itmo.ObjectOrientedProgramming.Lab2.Entities;
using Itmo.ObjectOrientedProgramming.Lab2.Entities.Сomponents;
using Itmo.ObjectOrientedProgramming.Lab2.Entities.Сomponents.DataStorage;
using Itmo.ObjectOrientedProgramming.Lab2.Exceptions;
using Itmo.ObjectOrientedProgramming.Lab2.Models;

namespace Itmo.ObjectOrientedProgramming.Lab2.Services;

public class ComputerDirector
{
    private readonly ComputerBuilder _builder;

    private readonly ComponentsFactory _factory;

    public ComputerDirector(ComponentsFactory factory, IEnumerable<IComputerValidator> validators)
    {
        _factory = factory;
        _builder = new ComputerBuilder(validators);
    }

    public Result<Computer, string> BuildComputer(Specification specification)
    {
        specification = specification ?? throw new ArgumentNullException(nameof(specification));

        // MotherBoard
        MotherBoard? motherBoard = _factory.GetMotherBoardByName(specification.MotherBoard);
        if (motherBoard is null)
        {
            return $"We don't have motherboard with name '{specification.MotherBoard}'";
        }

        _builder.SetMotherBoard(motherBoard);

        // Cpu
        Cpu? cpu = _factory.GetCpuByName(specification.CpuName);
        if (cpu is null)
        {
            return $"We don't have CPU with name '{specification.CpuName}'";
        }

        _builder.SetCpu(cpu);

        // RAM
        foreach (string ramStickName in specification.RamSticks)
        {
            RamStick? ramStick = _factory.GetRamStickByName(ramStickName);
            if (ramStick is null)
            {
                return $"We don't have ram stick with name '{ramStickName}'";
            }

            _builder.SetRamStick(ramStick);
        }

        // GPU
        GraphicCard? gpu = _factory.GetGraphicCardByName(specification.GraphicCard);
        if (gpu is null)
        {
            return $"We don't have GPU with name '{specification.GraphicCard}'";
        }

        _builder.SetGraphicCard(gpu);

        // HDD/SDD
        DataStorageBase? dataStorage = _factory.GetDataStorageByName(specification.DataStorage);
        if (dataStorage is null)
        {
            return $"We don't have data storage (ssd/hdd) with name '{specification.DataStorage}'";
        }

        _builder.SetDataStorage(dataStorage);

        // CPU cooling system
        CpuCoolingSystem? cpuCoolingSystem = _factory.GetCpuCoolingSystemByName(specification.CpuCoolingSystem);
        if (cpuCoolingSystem is null)
        {
            return $"We don't have CPU cooling system with name '{specification.CpuCoolingSystem}'";
        }

        _builder.SetCpuCoolingSystem(cpuCoolingSystem);

        // Computer case
        ComputerCase? computerCase = _factory.GetComputerCaseByName(specification.ComputerCase);
        if (computerCase is null)
        {
            return $"We don't have computer case with name '{specification.ComputerCase}'";
        }

        _builder.SetComputerCase(computerCase);

        // Power supply
        PowerSupply? powerSupply = _factory.GetPowerSupplyByName(specification.PowerSupply);
        if (powerSupply is null)
        {
            return $"We don't have power supply with name '{specification.PowerSupply}'";
        }

        _builder.SetPowerSupply(powerSupply);

        // Bios
        Bios? bios = null;
        if (specification.Bios is not null)
        {
            bios = _factory.GetBiosByName(specification.Bios);

            if (bios is null)
            {
                return $"We don't have bios with name '{specification.Bios}'";
            }
        }

        _builder.SetBios(bios);

        // WIFI adapter
        WifiAdapter? wifiAdapter = null;
        if (specification.WifiAdapter is not null)
        {
            wifiAdapter = _factory.GetWifiAdapterByName(specification.WifiAdapter);

            if (wifiAdapter is null)
            {
                return $"We don't have Wi-Fi adapter with name '{specification.WifiAdapter}'";
            }
        }

        _builder.SetWifiAdapter(wifiAdapter);

        return _builder.Build();
    }
}