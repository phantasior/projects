namespace Itmo.ObjectOrientedProgramming.Lab2.Entities.Сomponents;

public class GraphicCard : ComponentBase
{
    public GraphicCard(
        string name,
        int height,
        int width,
        int memoryAmount,
        int pcieVersion,
        int chipFrequency,
        int powerConsumption)
    {
        Name = name;
        Height = height;
        Width = width;
        MemoryAmount = memoryAmount;
        PcieVersion = pcieVersion;
        ChipFrequency = chipFrequency;
        PowerConsumption = powerConsumption;
    }

    public override string Name { get; }

    public int Height { get; }

    public int Width { get; }

    public int MemoryAmount { get; }

    public int PcieVersion { get; }

    public int ChipFrequency { get; }

    public int PowerConsumption { get; }
}