using Abstractions.Repositories;
using Application.Accounts;
using Models.Accounts;
using Models.Results;
using NSubstitute;
using Xunit;

namespace Itmo.ObjectOrientedProgramming.Lab5.Tests;

public class MoneyTests
{
    [Theory]
    [InlineData(1, 1, 50)]
    public void SuccessfulMoneyWithdrawTest(long userId, long accountId, long amount)
    {
        var account = new Account(accountId, "hash", userId, 100);
        IAccountRepository accountRepository = Substitute.For<IAccountRepository>();
        IAccountHistoryRepository accountHistoryRepository = Substitute.For<IAccountHistoryRepository>();
        accountRepository.FindAccountById(accountId).Returns(account);
        accountRepository.UpdateAccountBalance(accountId, amount).Returns(true);
        var accountService = new AccountService(accountRepository, accountHistoryRepository);

        WithdrawResult result = accountService.WithdrawMoney(userId, accountId, amount);

        accountRepository.Received().FindAccountById(accountId);
        Assert.Equal(new WithdrawResult.Success(), result);
    }

    [Theory]
    [InlineData(1, 1, 2000)]
    [InlineData(2, 2, 101)]
    public void NotEnoughMoneyMoneyWithdrawTest(long userId, long accountId, long amount)
    {
        var account = new Account(accountId, "hash", userId, 100);
        IAccountRepository accountRepository = Substitute.For<IAccountRepository>();
        IAccountHistoryRepository accountHistoryRepository = Substitute.For<IAccountHistoryRepository>();
        accountRepository.FindAccountById(accountId).Returns(account);
        var accountService = new AccountService(accountRepository, accountHistoryRepository);

        WithdrawResult result = accountService.WithdrawMoney(userId, accountId, amount);

        accountRepository.Received().FindAccountById(accountId);
        Assert.Equal(new WithdrawResult.NotEnoughMoney(), result);
    }

    [Theory]
    [InlineData(10, 100)]
    [InlineData(4, 1000)]
    public void AccountDepositTest(long accountId, long amount)
    {
        var account = new Account(accountId, "hash", 0, 100);
        IAccountRepository accountRepository = Substitute.For<IAccountRepository>();
        IAccountHistoryRepository accountHistoryRepository = Substitute.For<IAccountHistoryRepository>();
        accountRepository.FindAccountById(accountId).Returns(account);
        accountRepository.UpdateAccountBalance(accountId, amount);
        var accountService = new AccountService(accountRepository, accountHistoryRepository);

        bool result = accountService.DepositMoney(accountId, amount);
        accountRepository.Received().FindAccountById(accountId);
        accountRepository.Received().UpdateAccountBalance(accountId, account.Balance + amount);
        Assert.True(result);
    }
}