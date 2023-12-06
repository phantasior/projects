using Models.Users;

namespace Abstractions.Repositories;

public interface IUserRepository
{
    User? FindUserByUsername(string username);

    bool AddUser(string username, string password);
}