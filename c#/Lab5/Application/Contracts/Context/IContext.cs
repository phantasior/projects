using Models.Users;

namespace Contracts.Context;

public interface IContext
{
    User? User { get; set; }
}