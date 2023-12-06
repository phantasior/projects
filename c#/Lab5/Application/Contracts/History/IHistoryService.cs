using Models.History;

namespace Contracts.History;

public interface IHistoryService
{
    IEnumerable<AccountHistoryItem> GetHistoryByAccountId(long accountId);
}