namespace Itmo.ObjectOrientedProgramming.Lab3.Entities.Messages;

public interface IMessage
{
    string Header { get; }

    string Body { get; }

    int Priority { get; }

    int Id { get; }
}