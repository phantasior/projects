using System;
using System.Collections.Generic;
using System.Linq;
using Itmo.ObjectOrientedProgramming.Lab2.Entities.Сomponents;
using Itmo.ObjectOrientedProgramming.Lab2.Entities.Сomponents.DataStorage;
using Itmo.ObjectOrientedProgramming.Lab2.Services;

namespace Itmo.ObjectOrientedProgramming.Lab2.Entities;

public class Computer
{
    public Computer(
        Cpu cpu,
        MotherBoard motherBoard,
        GraphicCard graphicCard,
        DataStorageBase dataStorage,
        CpuCoolingSystem cpuCoolingSystem,
        PowerSupply powerSupply,
        ComputerCase computerCase,
        IList<RamStick> ramSticks,
        IEnumerable<string> warnings,
        Bios? bios = null,
        WifiAdapter? wifiAdapter = null)
    {
        Cpu = cpu;
        MotherBoard = motherBoard;
        RamSticks = ramSticks;
        Warnings = warnings;
        Bios = bios;
        WifiAdapter = wifiAdapter;
        GraphicCard = graphicCard;
        DataStorage = dataStorage;
        CpuCoolingSystem = cpuCoolingSystem;
        PowerSupply = powerSupply;
        ComputerCase = computerCase;
    }

    public Cpu Cpu { get; }

    public MotherBoard MotherBoard { get; }

    public IList<RamStick> RamSticks { get; }

    public GraphicCard GraphicCard { get; }

    public DataStorageBase DataStorage { get; }

    public CpuCoolingSystem CpuCoolingSystem { get; }

    public PowerSupply PowerSupply { get; }

    public ComputerCase ComputerCase { get; }

    public Bios? Bios { get; }

    public WifiAdapter? WifiAdapter { get; }

    public IEnumerable<string> Warnings { get; }

    public void Build(ComputerBuilder builder)
    {
        builder = builder ?? throw new ArgumentNullException(nameof(builder));
        builder.Cpu = Cpu;
        builder.MotherBoard = MotherBoard;
        builder.RamSticks.CopyTo(RamSticks.ToArray(), 0);
        builder.Warnings.CopyTo(Warnings.ToArray(), 0);
        builder.Bios = Bios;
        builder.WifiAdapter = WifiAdapter;
        builder.GraphicCard = GraphicCard;
        builder.DataStorage = DataStorage;
        builder.CpuCoolingSystem = CpuCoolingSystem;
        builder.PowerSupply = PowerSupply;
        builder.ComputerCase = ComputerCase;
    }
}