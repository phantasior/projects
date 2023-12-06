using Models.Accounts;
using Models.Results;

namespace Contracts.Accounts;

public interface IAccountService
{
    bool CreateAccount(long userId, int pin);

    IEnumerable<Account> GetAccountsByUserId(long userId);

    GetBalanceResult GetAccountBalance(long accountId);

    WithdrawResult WithdrawMoney(long userId, long accountId, long amount);

    public bool DepositMoney(long accountId, long amount);

    IEnumerable<AccountOperation> GetOperationHistory(long accountId);
}