namespace Itmo.ObjectOrientedProgramming.Lab2.Entities.Сomponents;

public class ComputerCase : ComponentBase
{
    public ComputerCase(string name, int maxGpuWidth, int maxGpuHeight, int width, int height)
    {
        Name = name;
        MaxGpuWidth = maxGpuWidth;
        MaxGpuHeight = maxGpuHeight;
        Width = width;
        Height = height;
    }

    public override string Name { get; }

    public int MaxGpuWidth { get; }

    public int MaxGpuHeight { get; }

    public int Width { get; }

    public int Height { get; }
}