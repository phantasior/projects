using System.Diagnostics.CodeAnalysis;
using Contracts.Accounts;
using Contracts.Context;
using Models.Users;

namespace Console.Scenarios.CreateAccount;

public class CreateAccountProvider : IScenarioProvider
{
    private readonly IAccountService _accountService;
    private readonly IContext _context;

    public CreateAccountProvider(IAccountService accountService, IContext context)
    {
        _accountService = accountService;
        _context = context;
    }

    public bool TryGetScenario([NotNullWhen(true)] out IScenario? scenario)
    {
        User? user = _context.User;
        if (user is null)
        {
            scenario = null;
            return false;
        }

        if (user.Role != UserRole.User)
        {
            scenario = null;
            return false;
        }

        scenario = new CreateAccountScenario(_accountService, _context);
        return true;
    }
}