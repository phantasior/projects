// Used pragma cuz it said i have to implement ToResult method (useless)

#pragma warning disable CA2225

using System;

namespace Itmo.ObjectOrientedProgramming.Lab2.Exceptions;

// https://en.cppreference.com/w/cpp/utility/expected
// TL;TR this class owns either _value or _error in time unit
// There is no std::variant for good implementation so i did like this
// T - for computer
// E - for error string
public class Result<T, TErr>
{
    private readonly T? _value;

    private readonly TErr? _error;

    public Result(TErr? error)
    {
        HasValue = false;
        _error = error;
    }

    public Result(T? value)
    {
        HasValue = true;
        _value = value;
    }

    public bool HasValue { get; }

    public static implicit operator Result<T, TErr>(T value) => new(value);

    public static implicit operator Result<T, TErr>(TErr error) => new(error);

    public T Value()
    {
        return HasValue && _value is not null
            ? _value
            : throw new ArgumentException("There is no value in c#::Expected");
    }

    public TErr Error()
    {
        return !HasValue && _error is not null
            ? _error
            : throw new ArgumentException("There is no error in s#::Expected");
    }
}

#pragma warning restore CA2225