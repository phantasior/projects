using System;
using Itmo.ObjectOrientedProgramming.Lab3.Entities.Messages;

namespace Itmo.ObjectOrientedProgramming.Lab3.Entities.Recipients;

public class PriorityRecipient : IRecipient
{
    private readonly IRecipient _recipient;

    public PriorityRecipient(IRecipient recipient, int priority)
    {
        _recipient = recipient;
        Priority = priority;
    }

    public int Priority { get; }

    public void ReceiveMessage(IMessage message)
    {
        message = message ?? throw new ArgumentNullException(nameof(message));
        if (message.Priority <= Priority)
        {
            _recipient.ReceiveMessage(message);
        }
    }
}