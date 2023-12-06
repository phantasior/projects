using System;
using Itmo.ObjectOrientedProgramming.Lab3.Entities.Extensions;
using Itmo.ObjectOrientedProgramming.Lab3.Entities.Messages;
using Itmo.ObjectOrientedProgramming.Lab3.Entities.Messengers;

namespace Itmo.ObjectOrientedProgramming.Lab3.Entities.Recipients;

public class MessengerRecipient : IRecipient
{
    private readonly IMessenger _messenger;

    public MessengerRecipient(IMessenger messenger)
    {
        _messenger = messenger ?? throw new ArgumentNullException(nameof(messenger));
    }

    public void ReceiveMessage(IMessage message)
    {
        message = message ?? throw new ArgumentNullException(nameof(message));
        _messenger.Write(message.Format());
    }
}