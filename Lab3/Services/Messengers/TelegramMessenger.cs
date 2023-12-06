using System;

namespace Itmo.ObjectOrientedProgramming.Lab3.Services.Messengers;

public class TelegramMessenger : ITelegramMessanger
{
    public void SendMessage(string apiKey, long userId, string message)
    {
        Console.WriteLine("[Telegram] " + message);
    }
}