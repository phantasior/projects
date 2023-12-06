using Contracts.Users;
using Models.Results;
using Spectre.Console;

namespace Console.Scenarios.Login;

public class LoginScenario : IScenario
{
    private readonly IUserService _userService;

    public LoginScenario(IUserService userService)
    {
        _userService = userService;
    }

    public string Name => "Login";

    public void Run()
    {
        string username = AnsiConsole.Ask<string>("Enter your username: ");
        string password = AnsiConsole.Ask<string>("Enter your password: ");
        LogInResult result = _userService.Login(username, password);
        string message = result switch
        {
            LogInResult.Success => "Successful login",
            LogInResult.NotFound => "User not found",
            LogInResult.IncorrectPassword => "Incorrect password",
            _ => throw new ArgumentOutOfRangeException(nameof(result)),
        };

        AnsiConsole.WriteLine(message);
        System.Console.ReadLine();
    }
}