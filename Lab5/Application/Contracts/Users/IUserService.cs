using Models.Results;

namespace Contracts.Users;

public interface IUserService
{
    LogInResult Login(string username, string password);

    SignUpResult SignUp(string username, string password);
}