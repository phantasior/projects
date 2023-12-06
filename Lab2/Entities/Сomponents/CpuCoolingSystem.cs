using System.Collections.Generic;
using System.Linq;
using Itmo.ObjectOrientedProgramming.Lab2.Models.СpuModels;

namespace Itmo.ObjectOrientedProgramming.Lab2.Entities.Сomponents;

public class CpuCoolingSystem : ComponentBase
{
    private readonly List<Sockets> _supportedSockets;

    public CpuCoolingSystem(
        string name,
        int width,
        int height,
        int maxTdp,
        ICollection<Sockets> supportedSockets)
    {
        Name = name;
        Width = width;
        Height = height;
        MaxTdp = maxTdp;
        _supportedSockets = supportedSockets.ToList();
    }

    public override string Name { get; }

    public int Width { get; }

    public int Height { get; }

    public int MaxTdp { get; }

    public IReadOnlyCollection<Sockets> SupportedSockets => _supportedSockets.AsReadOnly();
}