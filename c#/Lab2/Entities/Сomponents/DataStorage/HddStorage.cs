namespace Itmo.ObjectOrientedProgramming.Lab2.Entities.Сomponents.DataStorage;

public class HddStorage : DataStorageBase
{
    public HddStorage(
        string name,
        int capacity,
        int powerSupply,
        int shindleSpeed)
        : base(name, capacity, powerSupply)
    {
        ShindleSpeed = shindleSpeed;
    }

    public int ShindleSpeed { get; }
}