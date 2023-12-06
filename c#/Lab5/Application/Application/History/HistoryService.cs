using Abstractions.Repositories;
using Contracts.History;
using Models.History;

namespace Application.History;

public class HistoryService : IHistoryService
{
    private readonly IAccountHistoryRepository _repository;

    public HistoryService(IAccountHistoryRepository repository)
    {
        _repository = repository;
    }

    public IEnumerable<AccountHistoryItem> GetHistoryByAccountId(long accountId)
    {
        return _repository.ShowHistoryByAccount(accountId);
    }
}