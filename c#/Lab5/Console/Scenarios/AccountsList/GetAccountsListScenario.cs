using Contracts.Accounts;
using Contracts.Context;
using Models.Accounts;
using Models.Users;
using Spectre.Console;

namespace Console.Scenarios.AccountsList;

public class GetAccountsListScenario : IScenario
{
    private readonly IContext _context;
    private readonly IAccountService _accountService;

    public GetAccountsListScenario(IContext context, IAccountService accountService)
    {
        _context = context;
        _accountService = accountService;
    }

    public string Name => "Get list of your accounts";

    public void Run()
    {
        User user = _context.User ?? throw new ArgumentNullException(nameof(user));
        IEnumerable<Account> accountsList = _accountService.GetAccountsByUserId(user.Id);
        AnsiConsole.WriteLine("List of your accounts:");
        foreach (Account account in accountsList)
        {
            AnsiConsole.WriteLine($"Account id: {account.Id}, balance: {account.Balance}");
        }

        System.Console.ReadLine();
    }
}