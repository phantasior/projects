namespace Itmo.ObjectOrientedProgramming.Lab1.Entities.Deflectors.DeflectorDecorators;

public abstract class DeflectorDecorator : DeflectorBase
{
    protected DeflectorDecorator(DeflectorBase deflector)
    {
        Deflector = deflector;
    }

    protected DeflectorBase Deflector { get; }
}