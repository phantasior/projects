using System;
using System.Collections.Generic;
using System.Linq;
using Itmo.ObjectOrientedProgramming.Lab3.Entities.Messages;

namespace Itmo.ObjectOrientedProgramming.Lab3.Services;

public class Exporter
{
    private readonly List<Topic> _topics = new();

    public Exporter AddTopic(Topic topic)
    {
        _topics.Add(topic);
        return this;
    }

    public Topic? GetTopic(string topicName)
        => _topics.FirstOrDefault(topic => topicName.Equals(topic.Name, System.StringComparison.Ordinal));

    public void SendMessage(Message message, params string[] topics)
    {
        topics = topics ?? throw new ArgumentNullException(nameof(topics));
        foreach (string topic in topics)
        {
            GetTopic(topic)?.SendMessage(message);
        }
    }
}