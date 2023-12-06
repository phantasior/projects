using System.Collections.Generic;
using System.Linq;

namespace Itmo.ObjectOrientedProgramming.Lab2.Models;

public class ChipSet
{
    private readonly List<int> _supportedFrequency;

    public ChipSet(IEnumerable<int> supportedFrequency, bool hasXmpSupport)
    {
        _supportedFrequency = supportedFrequency.ToList();
        HasXmpSupport = hasXmpSupport;
    }

    public bool HasXmpSupport { get; }

    public IReadOnlyCollection<int> SupportedFrequency => _supportedFrequency.AsReadOnly();
}