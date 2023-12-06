using System.Diagnostics.CodeAnalysis;
using Contracts.Accounts;
using Contracts.Context;

namespace Console.Scenarios.WithdrawMoney;

public class WithdrawScenarioProvider : IScenarioProvider
{
    private readonly IContext _context;
    private readonly IAccountService _accountService;

    public WithdrawScenarioProvider(IContext context, IAccountService accountService)
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

        scenario = new WithdrawMoneyScenario(_accountService, _context);
        return true;
    }
}