namespace Console.Scenarios;

public interface IScenario
{
    string Name { get; }

    void Run();
}