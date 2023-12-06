using Abstractions.Repositories;
using Contracts.Accounts;
using Models.Accounts;
using Models.Results;

namespace Application.Accounts;

public class AccountService : IAccountService
{
    private readonly IAccountRepository _accountRepository;
    private readonly IAccountHistoryRepository _accountHistoryRepository;

    public AccountService(IAccountRepository accountRepository, IAccountHistoryRepository accountHistoryRepository)
    {
        _accountRepository = accountRepository;
        _accountHistoryRepository = accountHistoryRepository;
    }

    public IEnumerable<Account> GetAccountsByUserId(long userId)
    {
        return _accountRepository.FindAccountsByUserId(userId);
    }

    public bool CreateAccount(long userId, int pin)
    {
        return _accountRepository.AddAccount(userId, pin);
    }

    public GetBalanceResult GetAccountBalance(long accountId)
    {
        Account? account = _accountRepository.FindAccountById(accountId);
        if (account is null)
            return new GetBalanceResult.NotFound();

        return new GetBalanceResult.Success(account.Balance);
    }

    public WithdrawResult WithdrawMoney(long userId, long accountId, long amount)
    {
        Account? account = _accountRepository.FindAccountById(accountId);
        if (account is null)
            return new WithdrawResult.AccountNotFound();

        if (account.Balance < amount)
            return new WithdrawResult.NotEnoughMoney();

        bool result = _accountRepository.UpdateAccountBalance(accountId, account.Balance - amount);
        if (result)
            _accountHistoryRepository.AddRecord(accountId, -amount);

        return result switch
        {
            true => new WithdrawResult.Success(),
            false => new WithdrawResult.AccountNotFound(),
        };
    }

    public bool DepositMoney(long accountId, long amount)
    {
        Account? account = _accountRepository.FindAccountById(accountId);
        if (account is null)
            return false;

        _accountRepository.UpdateAccountBalance(accountId, account.Balance + amount);
        _accountHistoryRepository.AddRecord(accountId, amount);

        return true;
    }

    public IEnumerable<AccountOperation> GetOperationHistory(long accountId)
    {
        return _accountRepository.GetAccountHistory(accountId);
    }
}