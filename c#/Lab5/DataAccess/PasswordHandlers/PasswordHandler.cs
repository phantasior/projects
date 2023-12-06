using System.Globalization;
using System.Security.Cryptography;
using System.Text;

namespace DataAccess.PasswordHandlers;

public static class PasswordHandler
{
    public static string GenerateHash(string password)
    {
        byte[] hashBytes = SHA256.HashData(Encoding.UTF8.GetBytes(password));
        return Convert.ToBase64String(hashBytes);
    }

    public static string GenerateHash(int pin)
    {
        return GenerateHash(string.Create(CultureInfo.CurrentCulture, $"{pin}"));
    }

    public static bool VerifyPassword(int pin, string hash)
    {
        string inputHash = GenerateHash(pin);
        return inputHash == hash;
    }

    public static bool VerifyPassword(string password, string hash)
    {
        string inputHash = GenerateHash(password);
        return inputHash == hash;
    }
}