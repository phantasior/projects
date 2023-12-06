using System.Collections.Generic;

namespace Itmo.ObjectOrientedProgramming.Lab2.Models;

public class Specification
{
    public Specification(
        string cpuName,
        string motherBoard,
        IList<string> ramSticks,
        string graphicCard,
        string dataStorage,
        string cpuCoolingSystem,
        string powerSupply,
        string computerCase,
        string? xmpProfile = null,
        string? wifiAdapter = null,
        string? bios = null)
    {
        CpuName = cpuName;
        MotherBoard = motherBoard;
        RamSticks = ramSticks;
        GraphicCard = graphicCard;
        DataStorage = dataStorage;
        CpuCoolingSystem = cpuCoolingSystem;
        PowerSupply = powerSupply;
        ComputerCase = computerCase;
        XmpProfile = xmpProfile;
        WifiAdapter = wifiAdapter;
        Bios = bios;
    }

    public string CpuName { get; }

    public string MotherBoard { get; }

    public IList<string> RamSticks { get; }

    public string GraphicCard { get; }

    public string DataStorage { get; }

    public string CpuCoolingSystem { get; }

    public string PowerSupply { get; }

    public string ComputerCase { get; }

    public string? Bios { get; }

    public string? WifiAdapter { get; }

    public string? XmpProfile { get; }
}