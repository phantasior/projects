using Contracts.Accounts;
using Contracts.Context;
using Models.Users;
using Spectre.Console;

namespace Console.Scenarios.CreateAccount;

public class CreateAccountScenario : IScenario
{
    private readonly IAccountService _accountService;
    private readonly IContext _context;

    public CreateAccountScenario(IAccountService accountService, IContext context)
    {
        _accountService = accountService;
        _context = context;
    }

    public string Name => "Create new account";

    public void Run()
    {
        int pin = AnsiConsole.Ask<int>("Enter pin to your account: ");
        User user = _context.User ?? throw new ArgumentNullException(nameof(user));
        bool result = _accountService.CreateAccount(user.Id, pin);
        string message = result switch
        {
            true => "Account successfully created",
            false => "Something went wrong",
        };

        AnsiConsole.WriteLine(message);
        System.Console.ReadLine();
    }
}