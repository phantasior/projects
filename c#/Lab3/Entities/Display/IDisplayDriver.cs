using System.Drawing;

namespace Itmo.ObjectOrientedProgramming.Lab3.Entities.Display;

public interface IDisplayDriver
{
    void SetColor(Color color);

    void ClearOutput();

    void WriteText(string text);
}