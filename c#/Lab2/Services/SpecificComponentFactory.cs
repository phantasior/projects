using System.Collections.Generic;
using System.Linq;
using Itmo.ObjectOrientedProgramming.Lab2.Entities.Сomponents;

namespace Itmo.ObjectOrientedProgramming.Lab2.Services;

public class SpecificComponentFactory<T> : IComponentFactory<T>
    where T : ComponentBase
{
    private readonly IList<T> _componentList;

    public SpecificComponentFactory(IEnumerable<T> list)
    {
        _componentList = list.ToList();
    }

    public T? GetByName(string name) => _componentList.FirstOrDefault(value => value.Name == name);
}