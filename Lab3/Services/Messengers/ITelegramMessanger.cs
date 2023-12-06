namespace Itmo.ObjectOrientedProgramming.Lab3.Services.Messengers;

public interface ITelegramMessanger
{
    void SendMessage(string apiKey, long userId, string message);
}