using System.Diagnostics.CodeAnalysis;
using Contracts.Context;
using Contracts.History;

namespace Console.Scenarios.History;

public class HistoryScenarioProvider : IScenarioProvider
{
    private readonly IContext _context;
    private readonly IHistoryService _historyService;

    public HistoryScenarioProvider(IContext context, IHistoryService historyService)
    {
        _context = context;
        _historyService = historyService;
    }

    public bool TryGetScenario([NotNullWhen(true)] out IScenario? scenario)
    {
        if (_context.User is null)
        {
            scenario = null;
            return false;
        }

        scenario = new HistoryScenario(_context, _historyService);
        return true;
    }
}