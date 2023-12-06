using Itmo.ObjectOrientedProgramming.Lab3.Entities.Messengers;

namespace Itmo.ObjectOrientedProgramming.Lab3.Services.Messengers;

public class TelegramMessengerAdapter : IMessenger
{
    private readonly ITelegramMessanger _telegramMessenger;

    public TelegramMessengerAdapter(ITelegramMessanger telegramMessanger)
    {
        _telegramMessenger = telegramMessanger;
    }

    public void Write(string text)
    {
        _telegramMessenger.SendMessage("api", 0, text);
    }
}