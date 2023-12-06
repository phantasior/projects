using System.Diagnostics.CodeAnalysis;

namespace Console.Scenarios;

public interface IScenarioProvider
{
    bool TryGetScenario([NotNullWhen(true)] out IScenario? scenario);
}