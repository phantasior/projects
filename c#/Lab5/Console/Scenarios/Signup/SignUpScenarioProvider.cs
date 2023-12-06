using System.Diagnostics.CodeAnalysis;
using Contracts.Context;
using Contracts.Users;

namespace Console.Scenarios.Signup;

public class SignUpScenarioProvider : IScenarioProvider
{
    private readonly IContext _context;
    private readonly IUserService _userService;

    public SignUpScenarioProvider(IContext context, IUserService userService)
    {
        _context = context;
        _userService = userService;
    }

    public bool TryGetScenario([NotNullWhen(true)] out IScenario? scenario)
    {
        if (_context.User is null)
        {
            scenario = new SignUpScenario(_context, _userService);
            return true;
        }

        scenario = null;
        return false;
    }
}