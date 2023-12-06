using Models.Accounts;

namespace Abstractions.Repositories;

public interface IAccountRepository
{
    IEnumerable<Account> FindAccountsByUserId(long usedId);

    public Account? FindAccountById(long accountId);

    bool AddAccount(long userId, int pin);

    bool UpdateAccountBalance(long accountId, long newBalance);

    IEnumerable<AccountOperation> GetAccountHistory(long accountId);
}