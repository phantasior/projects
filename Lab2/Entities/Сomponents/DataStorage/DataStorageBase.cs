namespace Itmo.ObjectOrientedProgramming.Lab2.Entities.Сomponents.DataStorage;

public abstract class DataStorageBase : ComponentBase
{
    protected DataStorageBase(string name, int capacity, int powerSupply)
    {
        Name = name;
        Capacity = capacity;
        PowerSupply = powerSupply;
    }

    public override string Name { get; }

    public int Capacity { get; }

    public int PowerSupply { get; }
}