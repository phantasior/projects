using System.Collections.Generic;

namespace Itmo.ObjectOrientedProgramming.Lab3.Entities.Recipients.RecipientBuilders;

public class GroupRecipientBuilder : RecipientBuilderBase<GroupRecipientBuilder>
{
    private readonly List<IRecipient> _recipients = new();

    public void WithRecipient(IRecipient recipient) => _recipients.Add(recipient);

    public override IRecipient Build()
    {
        var group = new GroupRecipient();
        foreach (IRecipient recipient in _recipients)
        {
            group.AddRecipient(recipient);
        }

        return BuildHelper(group);
    }
}