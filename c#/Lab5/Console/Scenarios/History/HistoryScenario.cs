using Contracts.Context;
using Contracts.History;
using Models.History;
using Models.Users;
using Spectre.Console;

namespace Console.Scenarios.History;

public class HistoryScenario : IScenario
{
    private readonly IContext _context;
    private readonly IHistoryService _historyService;

    public HistoryScenario(IContext context, IHistoryService historyService)
    {
        _context = context;
        _historyService = historyService;
    }

    public string Name => "Get history of transactions";

    public void Run()
    {
        User user = _context.User ?? throw new ArgumentNullException(nameof(user));
        IEnumerable<AccountHistoryItem> history = _historyService.GetHistoryByAccountId(user.Id);
        foreach (AccountHistoryItem historyItem in history)
        {
            AnsiConsole.WriteLine(historyItem.Amount < 0
                ? $"Withdraw {historyItem.Amount} from {historyItem.AccountId} at {historyItem.Date}"
                : $"Deposit {historyItem.Amount} from {historyItem.AccountId} at {historyItem.Date}");
        }

        System.Console.ReadLine();
    }
}