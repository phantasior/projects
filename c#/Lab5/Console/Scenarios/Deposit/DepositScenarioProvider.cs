using System.Diagnostics.CodeAnalysis;
using Contracts.Accounts;
using Contracts.Context;

namespace Console.Scenarios.Deposit;

public class DepositScenarioProvider : IScenarioProvider
{
    private readonly IContext _context;
    private readonly IAccountService _accountService;

    public DepositScenarioProvider(IContext context, IAccountService accountService)
    {
        _context = context;
        _accountService = accountService;
    }

    public bool TryGetScenario([NotNullWhen(true)] out IScenario? scenario)
    {
        if (_context.User is null)
        {
            scenario = null;
            return false;
        }

        scenario = new DepositScenario(_accountService);
        return true;
    }
}