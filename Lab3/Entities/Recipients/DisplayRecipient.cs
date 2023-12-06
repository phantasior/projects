using System;
using System.Drawing;
using Itmo.ObjectOrientedProgramming.Lab3.Entities.Display;
using Itmo.ObjectOrientedProgramming.Lab3.Entities.Extensions;
using Itmo.ObjectOrientedProgramming.Lab3.Entities.Messages;

namespace Itmo.ObjectOrientedProgramming.Lab3.Entities.Recipients;

public class DisplayRecipient : IRecipient
{
    private readonly IDisplayDriver _driver;

    public DisplayRecipient(IDisplayDriver? driver = null)
    {
        _driver = driver ?? new DisplayDriver();
    }

    public void SetColor(Color color)
    {
        _driver.SetColor(color);
    }

    public void ReceiveMessage(IMessage message)
    {
        message = message ?? throw new ArgumentNullException(nameof(message));
        _driver.ClearOutput();
        _driver.WriteText(message.Format());
    }
}