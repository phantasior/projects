using System.Collections.Generic;
using System.Collections.ObjectModel;
using Itmo.ObjectOrientedProgramming.Lab3.Entities.Messages;

namespace Itmo.ObjectOrientedProgramming.Lab3.Entities.Recipients;

public class GroupRecipient : IRecipient
{
    private readonly ICollection<IRecipient> _recipients;

    public GroupRecipient(ICollection<IRecipient>? recipients = null)
    {
        _recipients = recipients ?? new Collection<IRecipient>();
    }

    public void AddRecipient(IRecipient recipient) => _recipients.Add(recipient);

    public void ReceiveMessage(IMessage message)
    {
        foreach (IRecipient recipient in _recipients)
        {
            recipient.ReceiveMessage(message);
        }
    }
}