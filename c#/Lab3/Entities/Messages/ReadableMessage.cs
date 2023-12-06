using System.ComponentModel.Design;

namespace Itmo.ObjectOrientedProgramming.Lab3.Entities.Messages;

public class ReadableMessage : IReadableMessage
{
    private readonly IMessage _message;

    public ReadableMessage(IMessage message)
    {
        _message = message;
    }

    public string Header => _message.Header;

    public string Body => _message.Body;

    public int Priority => _message.Priority;

    public int Id => _message.Id;

    public bool WasRead { get; private set; }

    public void Read()
    {
        if (WasRead)
        {
            throw new CheckoutException("You can't read message two times");
        }

        WasRead = true;
    }

    public override bool Equals(object? obj)
    {
        return _message.Equals(obj);
    }

    public override int GetHashCode()
    {
        return _message.GetHashCode();
    }
}