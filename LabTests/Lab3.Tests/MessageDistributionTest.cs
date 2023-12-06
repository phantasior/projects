using System;
using System.ComponentModel.Design;
using System.Linq;
using Itmo.ObjectOrientedProgramming.Lab3.Entities;
using Itmo.ObjectOrientedProgramming.Lab3.Entities.Messages;
using Itmo.ObjectOrientedProgramming.Lab3.Entities.Messengers;
using Itmo.ObjectOrientedProgramming.Lab3.Entities.Recipients.RecipientBuilders;
using Itmo.ObjectOrientedProgramming.Lab3.Services;
using Itmo.ObjectOrientedProgramming.Lab3.Services.Writers;
using NSubstitute;
using Xunit;

namespace Itmo.ObjectOrientedProgramming.Lab3.Tests;

public class MessageDistributionTest
{
    [Theory]
    [InlineData("Header", "Body", 3)]
    public void UnreadMessageTest(string header, string body, int priority)
    {
        Exporter exporter = new();
        var message = new Message(header, body, priority);
        var user = new User();

        Topic topic = Topic.Build
            .WithName("TopicName")
            .WithRecipient(
                new UserRecipientBuilder()
                    .WithUser(user)
                    .Build())
            .Build();

        exporter.AddTopic(topic)
            .SendMessage(message, "TopicName");

        Assert.True(user.Messages.Count > 0);
        Assert.False(user.Messages.First(curMessage => curMessage.Equals(message)).WasRead);
    }

    [Theory]
    [InlineData("Header", "Body", 3)]
    public void ReadMessageTest(string header, string body, int priority)
    {
        var exporter = new Exporter();
        var user = new User();
        var message = new Message(header, body, priority);

        Topic topic = Topic.Build
            .WithName("Topic Name")
            .WithRecipient(
                new UserRecipientBuilder()
                    .WithUser(user)
                    .Build())
            .Build();

        exporter.AddTopic(topic)
            .SendMessage(message, "Topic Name");
        user.ReadMessage(user.Messages.First());

        Assert.True(user.Messages.First(curMessage => curMessage.Equals(message)).WasRead);
    }

    [Theory]
    [InlineData("Header", "Body", 3)]
    public void DoubleReadMessageTest(string header, string body, int priority)
    {
        var exporter = new Exporter();
        var user = new User();
        var message = new Message(header, body, priority);

        Topic topic = Topic.Build
            .WithName("topic name")
            .WithRecipient(
                new UserRecipientBuilder()
                    .WithUser(user)
                    .Build())
            .Build();

        exporter.AddTopic(topic)
            .SendMessage(message, "topic name");
        user.ReadMessage(user.Messages.First());

        CheckoutException ex = Assert.Throws<CheckoutException>(() => user.ReadMessage(user.Messages.First()));
        Assert.Equal("You can't read message two times", ex.Message);
    }

    [Theory]
    [InlineData("Header", "Body", 3)]
    public void LowPriorityLevelTest(string header, string body, int priority)
    {
        var exporter = new Exporter();
        var user = new User();
        var message = new Message(header, body, priority);
        Topic topic = Topic.Build
            .WithName("topic name")
            .WithRecipient(
                new MockedRecipientBuilder()
                    .WithFilter(1)
                    .Build())
            .Build();

        exporter.AddTopic(topic)
            .SendMessage(message, "topic name");

        Assert.DoesNotContain(user.Messages, msg => msg.Equals(message));
    }

    [Theory]
    [InlineData("Header", "Body", 3)]
    public void LoggingTest(string header, string body, int priority)
    {
        var exporter = new Exporter();
        IWriter writer = Substitute.For<IWriter>();
        var message = new Message(header, body, priority);

        Topic topic = Topic.Build
            .WithName("topic name")
            .WithRecipient(
                new MockedRecipientBuilder()
                    .WithLogger(writer)
                    .Build())
            .Build();

        exporter.AddTopic(topic)
            .SendMessage(message, "topic name");

        writer.Received().Write($"{DateTime.Now.ToShortDateString()}: {header}\n{body}\n");
    }

    [Theory]
    [InlineData("Header", "Body", 3)]
    public void MessengerTest(string header, string body, int priority)
    {
        var exporter = new Exporter();
        IMessenger messenger = Substitute.For<IMessenger>();
        var message = new Message(header, body, priority);

        Topic topic = Topic.Build
            .WithName("topic name")
            .WithRecipient(
                new MessageRecipientBuilder()
                    .WithMessenger(messenger)
                    .Build())
            .Build();

        exporter.AddTopic(topic)
            .SendMessage(message, "topic name");

        messenger.Received().Write($"{header}\n{body}\n");
    }
}