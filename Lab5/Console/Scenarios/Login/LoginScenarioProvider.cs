using System.Diagnostics.CodeAnalysis;
using Contracts.Context;
using Contracts.Users;

namespace Console.Scenarios.Login;

public class LoginScenarioProvider : IScenarioProvider
{
    private readonly IContext _context;
    private readonly IUserService _userService;

    public LoginScenarioProvider(IUserService userService, IContext context)
    {
        _userService = userService;
        _context = context;
    }

    public bool TryGetScenario([NotNullWhen(true)] out IScenario? scenario)
    {
        if (_context.User is not null)
        {
            scenario = null;
            return false;
        }

        scenario = new LoginScenario(_userService);
        return true;
    }
}