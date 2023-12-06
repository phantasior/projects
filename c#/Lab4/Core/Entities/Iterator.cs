using System.Collections.Generic;
using System.Linq;

namespace Itmo.ObjectOrientedProgramming.Lab4.Core.Entities;

public class Iterator<T>
{
    private readonly List<T> _array;
    private int _idx;

    public Iterator(IEnumerable<T> array)
    {
        _array = array.ToList();
        _idx = 0;
    }

    public bool HasNext() => _idx < _array.Count;

    public T Current() => _array[_idx];

    public T? ShowKth(int idx)
    {
        if (idx < _array.Count)
        {
            return _array[idx];
        }

        return default;
    }

    public T CheckNext() => _array[_idx + 1];

    public T Next() => _array[_idx++];

    public void Skip(int cnt) => _idx += cnt;

    public void Clear() => _idx = 0;
}