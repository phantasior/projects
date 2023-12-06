using Contracts.Context;
using Models.Users;

namespace Application;

public class ProgramContext : IContext
{
    public User? User { get; set; }
}