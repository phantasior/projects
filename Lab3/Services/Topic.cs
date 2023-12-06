// wanna make nested builder

#pragma warning disable CA1034

using System;
using Itmo.ObjectOrientedProgramming.Lab3.Entities.Messages;
using Itmo.ObjectOrientedProgramming.Lab3.Entities.Recipients;

namespace Itmo.ObjectOrientedProgramming.Lab3.Services;

public class Topic
{
    private Topic(string name, IRecipient recipient)
    {
        Name = name;
        Recipient = recipient;
    }

    public static Builder Build => new Builder();

    public string Name { get; }

    public IRecipient Recipient { get; }

    public void SendMessage(IMessage message)
    {
        Recipient.ReceiveMessage(message);
    }

    public class Builder
    {
        private string? _name;
        private IRecipient? _recipient;

        public Builder WithName(string name)
        {
            _name = name;
            return this;
        }

        public Builder WithRecipient(IRecipient recipient)
        {
            _recipient = recipient;
            return this;
        }

        public Topic Build()
        {
            _name = _name ?? throw new ArgumentNullException(nameof(_name));
            _recipient = _recipient ?? throw new ArgumentNullException(nameof(_recipient));

            return new Topic(_name, _recipient);
        }
    }
}

#pragma warning restore CA1034