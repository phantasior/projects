using System;
using System.Collections.Generic;
using System.Linq;
using Itmo.ObjectOrientedProgramming.Lab3.Entities.Messages;

namespace Itmo.ObjectOrientedProgramming.Lab3.Entities;

public class User
{
    private readonly List<IReadableMessage> _messages;

    public User(IReadOnlyCollection<IReadableMessage>? messages = null)
    {
        _messages = messages?.ToList() ?? new List<IReadableMessage>();
    }

    public IReadOnlyCollection<IReadableMessage> Messages => _messages.AsReadOnly();

    public void ReceiveMessage(IReadableMessage message)
    {
        message = message ?? throw new ArgumentNullException(nameof(message));
        _messages.Add(message);
    }

    public void ReadMessage(IMessage message)
    {
        _messages.FirstOrDefault(myMessage => myMessage.Equals(message))?.Read();
    }
}