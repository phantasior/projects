using System.Drawing;

namespace Itmo.ObjectOrientedProgramming.Lab3.Entities.Recipients.RecipientBuilders;

public class DisplayRecipientBuilder : RecipientBuilderBase<DisplayRecipientBuilder>
{
    private Color? _color;

    public DisplayRecipientBuilder WithColor(Color color)
    {
        _color = color;
        return this;
    }

    public override IRecipient Build()
    {
        var display = new DisplayRecipient();

        if (_color is { } color)
        {
            display.SetColor(color);
        }

        return BuildHelper(display);
    }
}