using Contracts.Context;
using Contracts.Users;
using Models.Results;
using Spectre.Console;

namespace Console.Scenarios.Signup;

public class SignUpScenario : IScenario
{
    private readonly IContext _context;
    private readonly IUserService _userService;

    public SignUpScenario(IContext context, IUserService userService)
    {
        _context = context;
        _userService = userService;
    }

    public string Name => "SignUp";

    public void Run()
    {
        TextPrompt<string> usernamePrompt = new TextPrompt<string>("Enter your username")
            .PromptStyle("green")
            .ValidationErrorMessage("Username is incorrect")
            .Validate(username => username.Length < 3
                ? ValidationResult.Error("Username must has at least 3 characters")
                : ValidationResult.Success());

        TextPrompt<string> passwordPrompt = new TextPrompt<string>("Enter your password")
            .PromptStyle("red")
            .Secret()
            .ValidationErrorMessage("Password is incorrect")
            .Validate(username => username.Length < 3
                ? ValidationResult.Error("Password must has at least 3 characters")
                : ValidationResult.Success());

        string username = AnsiConsole.Prompt(usernamePrompt);
        string password = AnsiConsole.Prompt(passwordPrompt);

        SignUpResult result = _userService.SignUp(username, password);
        string message = result switch
        {
            SignUpResult.Success => "User successfully added",
            SignUpResult.Failure => "Something went wrong",
            _ => throw new ArgumentOutOfRangeException(nameof(result)),
        };

        AnsiConsole.WriteLine(message);
        System.Console.ReadLine();
    }
}