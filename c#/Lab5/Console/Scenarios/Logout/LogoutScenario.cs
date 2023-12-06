using Contracts.Context;

namespace Console.Scenarios.Logout;

public class LogoutScenario : IScenario
{
    private readonly IContext _context;

    public LogoutScenario(IContext context)
    {
        _context = context;
    }

    public string Name => "LogOut";

    public void Run()
    {
        _context.User = null;
    }
}