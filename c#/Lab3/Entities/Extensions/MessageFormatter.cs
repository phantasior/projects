using System;
using System.Text;
using Itmo.ObjectOrientedProgramming.Lab3.Entities.Messages;

namespace Itmo.ObjectOrientedProgramming.Lab3.Entities.Extensions;

public static class MessageFormatter
{
    public static string Format(this IMessage message)
    {
        message = message ?? throw new ArgumentNullException(nameof(message));

        var sb = new StringBuilder();
        sb.AppendLine(message.Header);
        sb.AppendLine(message.Body);

        return sb.ToString();
    }
}