using Itmo.ObjectOrientedProgramming.Lab2.Models;

namespace Itmo.ObjectOrientedProgramming.Lab2.Entities.Сomponents.DataStorage;

public class SsdStorage : DataStorageBase
{
    public SsdStorage(
        string name,
        int capacity,
        int powerSupply,
        SsdConnectionType connectionType,
        int maxSpeed)
        : base(name, capacity, powerSupply)
    {
        ConnectionType = connectionType;
        MaxSpeed = maxSpeed;
    }

    public SsdConnectionType ConnectionType { get; }

    public int MaxSpeed { get; }
}