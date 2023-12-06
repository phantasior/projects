using System.Collections.Generic;
using System.Linq;
using Itmo.ObjectOrientedProgramming.Lab2.Models;
using Itmo.ObjectOrientedProgramming.Lab2.Models.MotherBoardModels;

namespace Itmo.ObjectOrientedProgramming.Lab2.Entities.Сomponents;

public class RamStick : ComponentBase
{
    private readonly List<string> _availableProfiles;

    private readonly List<int> _supportedFrequencies;

    public RamStick(
        int memorySize,
        int power,
        string name,
        DdrStandard ddrVersion,
        RamFormFactor formFactor,
        IEnumerable<string> availableProfiles,
        IEnumerable<int> supportedFrequencies)
    {
        Name = name;
        MemorySize = memorySize;
        DdrVersion = ddrVersion;
        FormFactor = formFactor;
        Power = power;
        _supportedFrequencies = supportedFrequencies.ToList();
        _availableProfiles = availableProfiles.ToList();
    }

    public override string Name { get; }

    public int MemorySize { get; }

    public DdrStandard DdrVersion { get; }

    public RamFormFactor FormFactor { get; }

    public int Power { get; }

    public IReadOnlyCollection<int> SupportedFrequencies => _supportedFrequencies.ToList();

    public IReadOnlyCollection<string> AvailableProfiles => _availableProfiles.ToList();
}