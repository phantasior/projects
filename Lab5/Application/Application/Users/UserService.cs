using Abstractions.Repositories;
using Contracts.Context;
using Contracts.Users;
using DataAccess.PasswordHandlers;
using Models.Results;
using Models.Users;

namespace Application.Users;

public class UserService : IUserService
{
    private readonly IUserRepository _repository;
    private readonly IContext _context;

    public UserService(IUserRepository repository, IContext context)
    {
        _repository = repository;
        _context = context;
    }

    public LogInResult Login(string username, string password)
    {
        User? user = _repository.FindUserByUsername(username);

        if (user is null)
            return new LogInResult.NotFound();

        if (!PasswordHandler.VerifyPassword(password, user.Password))
            return new LogInResult.IncorrectPassword();

        _context.User = user;

        return new LogInResult.Success();
    }

    public SignUpResult SignUp(string username, string password)
    {
        bool result = _repository.AddUser(username, password);
        return result switch
        {
            false => new SignUpResult.Failure(),
            true => new SignUpResult.Success(),
        };
    }
}