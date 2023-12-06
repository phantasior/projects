using Itmo.ObjectOrientedProgramming.Lab3.Entities.Messages;

namespace Itmo.ObjectOrientedProgramming.Lab3.Entities.Recipients;

    public interface IRecipient
    {
        void ReceiveMessage(IMessage message);
    }