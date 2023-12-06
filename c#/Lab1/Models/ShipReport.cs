namespace Itmo.ObjectOrientedProgramming.Lab1.Models;

public class ShipReport
{
    public int UsedFuel { get; set; }

    public int Duration { get; set; }

    public TravelStatus Status { get; set; } = TravelStatus.Success;
}