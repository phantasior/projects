using System.Diagnostics.CodeAnalysis;
using Contracts.Accounts;
using Contracts.Context;

namespace Console.Scenarios.AccountsList;

public class GetAccountsListScenarioProvider : IScenarioProvider
{
    private readonly IContext _context;
    private readonly IAccountService _accountService;

    public GetAccountsListScenarioProvider(IContext context, IAccountService accountService)
    {
        _context = context;
        _accountService = accountService;
    }

    public bool TryGetScenario([NotNullWhen(true)] out IScenario? scenario)
    {
        if (_context.User is not null)
        {
            scenario = new GetAccountsListScenario(_context, _accountService);
            return true;
        }

        scenario = null;
        return false;
    }
}