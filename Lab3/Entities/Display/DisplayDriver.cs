using System;
using System.Drawing;

namespace Itmo.ObjectOrientedProgramming.Lab3.Entities.Display;

public class DisplayDriver : IDisplayDriver
{
    private Color _color = Color.White;

    public void SetColor(Color color)
    {
        _color = color;
    }

    public void ClearOutput()
    {
        Console.Clear();
    }

    public void WriteText(string text)
    {
        Console.WriteLine(text);
    }
}