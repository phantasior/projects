using Itmo.ObjectOrientedProgramming.Lab2.Models;
using Itmo.ObjectOrientedProgramming.Lab2.Models.MotherBoardModels;
using Itmo.ObjectOrientedProgramming.Lab2.Models.СpuModels;

namespace Itmo.ObjectOrientedProgramming.Lab2.Entities.Сomponents;

public class MotherBoard : ComponentBase
{
    public MotherBoard(
        string name,
        Sockets socket,
        int pcieLinesAmount,
        int sataPortsAmount,
        int cpuSlotAmount,
        MotherBoardFormFactor motherBoardFormFactor,
        DdrStandard ddrVersion,
        ChipSet chipSet,
        bool hasInternalWifi)
    {
        Name = name;
        Socket = socket;
        PcieLinesAmount = pcieLinesAmount;
        SataPortsAmount = sataPortsAmount;
        CpuSlotAmount = cpuSlotAmount;
        MotherBoardFormFactor = motherBoardFormFactor;
        DdrVersion = ddrVersion;
        ChipSet = chipSet;
        HasInternalWifi = hasInternalWifi;
    }

    public override string Name { get; }

    public Sockets Socket { get; }

    public int PcieLinesAmount { get; } // for gpu,  wifi, etc

    public int SataPortsAmount { get; } // for hdd/ssd

    public int CpuSlotAmount { get; } // understandable

    public MotherBoardFormFactor MotherBoardFormFactor { get; }

    public DdrStandard DdrVersion { get; }

    public ChipSet ChipSet { get; }

    public bool HasInternalWifi { get; }
}