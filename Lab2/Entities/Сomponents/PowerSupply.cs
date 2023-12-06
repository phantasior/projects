namespace Itmo.ObjectOrientedProgramming.Lab2.Entities.Сomponents;

public class PowerSupply : ComponentBase
{
    public PowerSupply(string name, int maxPower)
    {
        Name = name;
        MaxPower = maxPower;
    }

    public override string Name { get; }

    public int MaxPower { get; }
}