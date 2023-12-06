using Contracts.Accounts;
using Spectre.Console;

namespace Console.Scenarios.Deposit;

public class DepositScenario : IScenario
{
    private readonly IAccountService _accountService;

    public DepositScenario(IAccountService accountService)
    {
        _accountService = accountService;
    }

    public string Name => "Deposit money";

    public void Run()
    {
        long accountId = AnsiConsole.Ask<long>("Enter account id to deposit?: ");
        long amount = AnsiConsole.Ask<long>("How much money to deposit?: ");
        string message = _accountService.DepositMoney(accountId, amount) switch
        {
            true => "Money deposited",
            false => "Something went wrong",
        };

        AnsiConsole.WriteLine(message);
        System.Console.ReadLine();
    }
}