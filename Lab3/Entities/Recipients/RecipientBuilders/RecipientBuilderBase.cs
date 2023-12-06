using System;
using Itmo.ObjectOrientedProgramming.Lab3.Services;
using Itmo.ObjectOrientedProgramming.Lab3.Services.Writers;

namespace Itmo.ObjectOrientedProgramming.Lab3.Entities.Recipients.RecipientBuilders;

public abstract class RecipientBuilderBase<T>
    where T : RecipientBuilderBase<T>
{
    protected bool HasLogger { get; private set; }
    protected IWriter? Writer { get; private set; }

    protected int? Priority { get; private set; }

    public T WithLogger(IWriter? writer = null)
    {
        HasLogger = true;
        Writer = writer;
        return (T)this;
    }

    public T WithFilter(int priority)
    {
        Priority = priority;
        return (T)this;
    }

    public abstract IRecipient Build();

    protected IRecipient BuildHelper(IRecipient recipient)
    {
        if (HasLogger)
        {
            Writer = Writer ?? throw new ArgumentNullException(nameof(recipient));
            recipient = new RecipientLogger(recipient, Writer);
        }

        if (Priority is { } priority)
        {
            recipient = new PriorityRecipient(recipient, priority);
        }

        return recipient;
    }
}