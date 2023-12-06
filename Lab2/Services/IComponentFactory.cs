namespace Itmo.ObjectOrientedProgramming.Lab2.Services;

public interface IComponentFactory<out T>
{
    T? GetByName(string name);
}