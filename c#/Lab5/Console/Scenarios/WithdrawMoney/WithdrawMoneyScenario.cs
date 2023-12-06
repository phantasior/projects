using Contracts.Accounts;
using Contracts.Context;
using Models.Results;
using Models.Users;
using Spectre.Console;

namespace Console.Scenarios.WithdrawMoney;

public class WithdrawMoneyScenario : IScenario
{
    private readonly IAccountService _accountService;
    private readonly IContext _context;

    public WithdrawMoneyScenario(IAccountService accountService, IContext context)
    {
        _accountService = accountService;
        _context = context;
    }

    public string Name => "Withdraw money";

    public void Run()
    {
        User user = _context.User ?? throw new ArgumentNullException(nameof(user));
        long accountId = AnsiConsole.Ask<long>("Enter id of your account: ");
        long amount = AnsiConsole.Ask<long>("How much money to withdraw?: ");

        WithdrawResult result = _accountService.WithdrawMoney(user.Id, accountId, amount);
        string message = result switch
        {
            WithdrawResult.StrangerAccount => "you can't access to foreign account",
            WithdrawResult.NotEnoughMoney => "you don't have enough money",
            WithdrawResult.AccountNotFound => "can't find such account",
            WithdrawResult.Success => "success",
            _ => "something went wrong",
        };

        AnsiConsole.WriteLine(message);
        System.Console.ReadLine();
    }
}