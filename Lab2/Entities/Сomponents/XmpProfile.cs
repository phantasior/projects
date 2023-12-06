namespace Itmo.ObjectOrientedProgramming.Lab2.Entities.Сomponents;

public class XmpProfile : ComponentBase
{
    public XmpProfile(string timings, int voltage, int frequency, string name)
    {
        Timings = timings;
        Voltage = voltage;
        Frequency = frequency;
        Name = name;
    }

    public override string Name { get; }

    public string Timings { get; }

    public int Voltage { get; }

    public int Frequency { get; }
}