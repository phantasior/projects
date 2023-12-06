namespace Itmo.ObjectOrientedProgramming.Lab3.Services;

public static class Uid
{
    private static int _id;

    public static int GetId => ++_id;
}