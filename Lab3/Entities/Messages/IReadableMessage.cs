namespace Itmo.ObjectOrientedProgramming.Lab3.Entities.Messages;

public interface IReadableMessage : IMessage
{
    public bool WasRead { get; }

    public void Read();
}