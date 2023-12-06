using Itmo.ObjectOrientedProgramming.Lab2.Models;

namespace Itmo.ObjectOrientedProgramming.Lab2.Entities.Сomponents;

public class WifiAdapter : ComponentBase
{
    public WifiAdapter(
        string name,
        WifiStandards wifiStandard,
        bool hasBuiltInBluetooth,
        int pcieVersion,
        int powerConsumption)
    {
        Name = name;
        WifiStandard = wifiStandard;
        HasBuiltInBluetooth = hasBuiltInBluetooth;
        PcieVersion = pcieVersion;
        PowerConsumption = powerConsumption;
    }

    public override string Name { get; }

    public WifiStandards WifiStandard { get; }

    public bool HasBuiltInBluetooth { get; }

    public int PcieVersion { get; }

    public int PowerConsumption { get; }
}