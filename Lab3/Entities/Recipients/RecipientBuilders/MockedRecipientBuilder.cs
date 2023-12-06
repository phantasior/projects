using NSubstitute;

namespace Itmo.ObjectOrientedProgramming.Lab3.Entities.Recipients.RecipientBuilders;

public class MockedRecipientBuilder : RecipientBuilderBase<MockedRecipientBuilder>
{
    public override IRecipient Build()
    {
        return BuildHelper(Substitute.For<IRecipient>());
    }
}