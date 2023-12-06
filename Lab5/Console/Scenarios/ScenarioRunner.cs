using Spectre.Console;

namespace Console.Scenarios;

public class ScenarioRunner
{
    private readonly IEnumerable<IScenarioProvider> _providers;

    public ScenarioRunner(IEnumerable<IScenarioProvider> providers)
    {
        _providers = providers;
    }

    public void Run()
    {
        IEnumerable<IScenario> scenarios = GetScenarios();
        IScenario scenario = AnsiConsole.Prompt(
            new SelectionPrompt<IScenario>()
                .Title("Select action")
                .AddChoices(scenarios)
                .UseConverter(scenario => scenario.Name));

        scenario.Run();
    }

    private IEnumerable<IScenario> GetScenarios()
    {
        foreach (IScenarioProvider provider in _providers)
        {
            if (provider.TryGetScenario(out IScenario? scenario))
                yield return scenario;
        }
    }
}