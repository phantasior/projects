using System.Collections.Generic;
using System.Linq;

namespace Itmo.ObjectOrientedProgramming.Lab2.Entities.Сomponents;

public class Bios : ComponentBase
{
    private readonly List<Cpu> _supportedCpu;

    public Bios(string name, int type, int version, IEnumerable<Cpu> supportedCpu)
    {
        Name = name;
        Type = type;
        Version = version;
        _supportedCpu = supportedCpu.ToList();
    }

    public override string Name { get; }

    public int Type { get; }

    public int Version { get; }

    public IReadOnlyCollection<Cpu> SupportedCpu => _supportedCpu.AsReadOnly();
}