using System.Diagnostics.CodeAnalysis;
using Contracts.Context;

namespace Console.Scenarios.Logout;

public class LogoutScenarioProvider : IScenarioProvider
{
    private readonly IContext _context;

    public LogoutScenarioProvider(IContext context)
    {
        _context = context;
    }

    public bool TryGetScenario([NotNullWhen(true)] out IScenario? scenario)
    {
        if (_context.User is null)
        {
            scenario = null;
            return false;
        }

        scenario = new LogoutScenario(_context);
        return true;
    }
}