using System;

namespace Itmo.ObjectOrientedProgramming.Lab3.Entities.Recipients.RecipientBuilders;

public class UserRecipientBuilder : RecipientBuilderBase<UserRecipientBuilder>
{
    private User? _user;

    public UserRecipientBuilder WithUser(User user)
    {
        _user = user;
        return this;
    }

    public override IRecipient Build()
    {
        _user = _user ?? throw new ArgumentNullException(nameof(_user));

        return BuildHelper(new UserRecipient(_user));
    }
}