using System;
using Itmo.ObjectOrientedProgramming.Lab3.Entities.Extensions;
using Itmo.ObjectOrientedProgramming.Lab3.Entities.Messages;
using Itmo.ObjectOrientedProgramming.Lab3.Entities.Recipients;
using Itmo.ObjectOrientedProgramming.Lab3.Services.Writers;

namespace Itmo.ObjectOrientedProgramming.Lab3.Services;

public class RecipientLogger : IRecipient
{
    private readonly IWriter _writer;

    private readonly IRecipient _recipient;

    public RecipientLogger(IRecipient recipient, IWriter writer)
    {
        _writer = writer;
        _recipient = recipient;
    }

    public void ReceiveMessage(IMessage message)
    {
        _writer.Write($"{DateTime.Now.ToShortDateString()}: {message.Format()}");
        _recipient.ReceiveMessage(message);
    }
}