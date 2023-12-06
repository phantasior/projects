using Itmo.ObjectOrientedProgramming.Lab3.Services;

namespace Itmo.ObjectOrientedProgramming.Lab3.Entities.Messages;

public class Message : IMessage
{
    public Message(string header, string body, int priority)
    {
        Header = header;
        Body = body;
        Priority = priority;
        Id = Uid.GetId;
    }

    public string Header { get; }

    public string Body { get; }

    public int Priority { get; }

    public int Id { get; }

    public override bool Equals(object? obj)
    {
        if (obj is not IMessage item)
        {
            return false;
        }

        return Id == item.Id;
    }

    public override int GetHashCode()
    {
        return base.GetHashCode();
    }
}