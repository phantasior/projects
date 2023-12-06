using Models.History;

namespace Abstractions.Repositories;

public interface IAccountHistoryRepository
{
    bool AddRecord(long accountId, long amount);

    IEnumerable<AccountHistoryItem> ShowHistoryByAccount(long accountId);
}