using System.Collections.Generic;
using System.Linq;
using Itmo.ObjectOrientedProgramming.Lab2.Models.СpuModels;

namespace Itmo.ObjectOrientedProgramming.Lab2.Entities.Сomponents;

public class Cpu : ComponentBase
{
    private List<int> _supportedFrequency;

    public Cpu(
        int coreFrequency,
        int coreAmount,
        Sockets socket,
        bool hasVideoCore,
        int tpd,
        int power,
        IEnumerable<int> supportedFrequency,
        string name)
    {
        CoreFrequency = coreFrequency;
        CoreAmount = coreAmount;
        Socket = socket;
        HasVideoCore = hasVideoCore;
        Tpd = tpd;
        Power = power;
        _supportedFrequency = supportedFrequency.ToList();
        Name = name;
    }

    public override string Name { get; }

    public int CoreFrequency { get; }

    public int CoreAmount { get; }

    public Sockets Socket { get; }

    public bool HasVideoCore { get; }

    public IReadOnlyCollection<int> SupportedFrequency => _supportedFrequency.AsReadOnly();

    public int Tpd { get; }

    public int Power { get; }
}