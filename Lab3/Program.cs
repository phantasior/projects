using Itmo.ObjectOrientedProgramming.Lab3.Entities;
using Itmo.ObjectOrientedProgramming.Lab3.Entities.Messages;
using Itmo.ObjectOrientedProgramming.Lab3.Entities.Recipients.RecipientBuilders;
using Itmo.ObjectOrientedProgramming.Lab3.Services;

namespace Itmo.ObjectOrientedProgramming.Lab3;

public static class Program
{
    public static void Main()
    {
        var user = new User();
        Topic topic = Topic.Build
            .WithName("TopicName")
            .WithRecipient(
                new UserRecipientBuilder()
                    .WithUser(user)
                    .Build())
            .Build();

        var message = new Message("header", "body", 31);
        topic.SendMessage(message);
    }
}