using System.Collections.Generic;
using System.Linq;
using Itmo.ObjectOrientedProgramming.Lab2.Entities;
using Itmo.ObjectOrientedProgramming.Lab2.Entities.Сomponents;
using Itmo.ObjectOrientedProgramming.Lab2.Entities.Сomponents.DataStorage;
using Itmo.ObjectOrientedProgramming.Lab2.Exceptions;
using Itmo.ObjectOrientedProgramming.Lab2.Models;

namespace Itmo.ObjectOrientedProgramming.Lab2.Services;

public class ComputerBuilder : IComputerBuilder
{
    private readonly IList<IComputerValidator> _validators;

    public ComputerBuilder(IEnumerable<IComputerValidator> validators)
    {
        _validators = validators.ToList();
    }

    public Cpu? Cpu { get; set; }

    public MotherBoard? MotherBoard { get; set; }

    public IList<RamStick> RamSticks { get; } = new List<RamStick>();

    public GraphicCard? GraphicCard { get; set; }

    public DataStorageBase? DataStorage { get; set; }

    public CpuCoolingSystem? CpuCoolingSystem { get; set; }

    public PowerSupply? PowerSupply { get; set; }

    public ComputerCase? ComputerCase { get; set; }

    public Bios? Bios { get; set; }

    public WifiAdapter? WifiAdapter { get; set; }

    public IList<string> Warnings { get; } = new List<string>();

    public IComputerBuilder Reset()
    {
        Cpu = null;
        MotherBoard = null;
        RamSticks.Clear();
        GraphicCard = null;
        DataStorage = null;
        CpuCoolingSystem = null;
        PowerSupply = null;
        ComputerCase = null;
        Bios = null;
        WifiAdapter = null;

        return this;
    }

    public IComputerBuilder SetCpu(Cpu cpu)
    {
        Cpu = cpu;
        return this;
    }

    public IComputerBuilder SetMotherBoard(MotherBoard motherBoard)
    {
        MotherBoard = motherBoard;
        return this;
    }

    public IComputerBuilder SetRamStick(RamStick ramStick)
    {
        RamSticks.Add(ramStick);
        return this;
    }

    public IComputerBuilder SetGraphicCard(GraphicCard graphicCard)
    {
        GraphicCard = graphicCard;
        return this;
    }

    public IComputerBuilder SetDataStorage(DataStorageBase dataStorage)
    {
        DataStorage = dataStorage;
        return this;
    }

    public IComputerBuilder SetCpuCoolingSystem(CpuCoolingSystem cpuCoolingSystem)
    {
        CpuCoolingSystem = cpuCoolingSystem;
        return this;
    }

    public IComputerBuilder SetPowerSupply(PowerSupply powerSupply)
    {
        PowerSupply = powerSupply;
        return this;
    }

    public IComputerBuilder SetComputerCase(ComputerCase computerCase)
    {
        ComputerCase = computerCase;
        return this;
    }

    public IComputerBuilder SetBios(Bios? bios)
    {
        Bios = bios;
        return this;
    }

    public IComputerBuilder SetWifiAdapter(WifiAdapter? wifiAdapter)
    {
        WifiAdapter = wifiAdapter;
        return this;
    }

    public Result<Computer, string> Build()
    {
        if (Cpu is null) return "You must set up CPU";
        if (MotherBoard is null) return "You must set up motherboard";
        if (GraphicCard is null) return "You must set up GPU";
        if (DataStorage is null) return "You must set up data storage (ssd/hdd)";
        if (CpuCoolingSystem is null) return "You must set up CPU cooling system";
        if (PowerSupply is null) return "You must set up power supply";
        if (ComputerCase is null) return "You must set up computer case";

        foreach (IComputerValidator validator in _validators)
        {
            Result<BuildStatus, string> validateResult = validator.Validate(this);
            if (!validateResult.HasValue)
            {
                return validateResult.Error();
            }
        }

        return new Computer(
            cpu: Cpu,
            motherBoard: MotherBoard,
            graphicCard: GraphicCard,
            dataStorage: DataStorage,
            cpuCoolingSystem: CpuCoolingSystem,
            powerSupply: PowerSupply,
            computerCase: ComputerCase,
            ramSticks: RamSticks,
            bios: Bios,
            wifiAdapter: WifiAdapter,
            warnings: Warnings);
    }
}