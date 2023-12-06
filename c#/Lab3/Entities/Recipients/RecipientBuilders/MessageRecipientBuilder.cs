using System;
using Itmo.ObjectOrientedProgramming.Lab3.Entities.Messengers;

namespace Itmo.ObjectOrientedProgramming.Lab3.Entities.Recipients.RecipientBuilders;

public class MessageRecipientBuilder : RecipientBuilderBase<MessageRecipientBuilder>
{
    private IMessenger? _messenger;

    public MessageRecipientBuilder WithMessenger(IMessenger messenger)
    {
        _messenger = messenger;
        return this;
    }

    public override IRecipient Build()
    {
        _messenger = _messenger ?? throw new ArgumentNullException(nameof(_messenger));
        return BuildHelper(new MessengerRecipient(_messenger));
    }
}