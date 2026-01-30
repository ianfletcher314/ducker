#pragma once

#include <JuceHeader.h>

namespace Colors
{
    const juce::Colour background    = juce::Colour(0xff1a1a1a);
    const juce::Colour panelBg       = juce::Colour(0xff252525);
    const juce::Colour accent        = juce::Colour(0xff4a9eff);
    const juce::Colour textPrimary   = juce::Colour(0xfff0f0f0);
    const juce::Colour textSecondary = juce::Colour(0xff909090);
    const juce::Colour meterGreen    = juce::Colour(0xff22c55e);
    const juce::Colour meterYellow   = juce::Colour(0xffeab308);
    const juce::Colour meterRed      = juce::Colour(0xffef4444);
    const juce::Colour knobBody      = juce::Colour(0xff404040);
    const juce::Colour led           = juce::Colour(0xffff3333);
    const juce::Colour duckBlue      = juce::Colour(0xff3b82f6);
    const juce::Colour duckPurple    = juce::Colour(0xff8b5cf6);
}

class DuckerLookAndFeel : public juce::LookAndFeel_V4
{
public:
    DuckerLookAndFeel()
    {
        setColour(juce::Slider::thumbColourId, Colors::accent);
        setColour(juce::Slider::rotarySliderFillColourId, Colors::accent);
        setColour(juce::Slider::rotarySliderOutlineColourId, Colors::knobBody);
        setColour(juce::Label::textColourId, Colors::textPrimary);
        setColour(juce::ComboBox::backgroundColourId, Colors::panelBg);
        setColour(juce::ComboBox::textColourId, Colors::textPrimary);
        setColour(juce::ComboBox::outlineColourId, Colors::knobBody);
        setColour(juce::PopupMenu::backgroundColourId, Colors::panelBg);
        setColour(juce::PopupMenu::textColourId, Colors::textPrimary);
        setColour(juce::PopupMenu::highlightedBackgroundColourId, Colors::accent);
        setColour(juce::ToggleButton::textColourId, Colors::textPrimary);
        setColour(juce::ToggleButton::tickColourId, Colors::accent);
    }

    void setAccentColour(juce::Colour c) { accentColour = c; }

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPosProportional, float, float,
                          juce::Slider&) override
    {
        auto bounds = juce::Rectangle<float>((float)x, (float)y, (float)width, (float)height).reduced(2.0f);
        float cx = bounds.getCentreX();
        float cy = bounds.getCentreY();
        float radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f - 2.0f;

        // Outer ring (knurled edge)
        g.setColour(juce::Colour(0xff303030));
        g.fillEllipse(cx - radius, cy - radius, radius * 2.0f, radius * 2.0f);

        // Knurl pattern
        g.setColour(juce::Colour(0xff404040));
        int numKnurls = 24;
        for (int i = 0; i < numKnurls; ++i)
        {
            float angle = i * juce::MathConstants<float>::twoPi / numKnurls;
            float x1 = cx + (radius - 1.0f) * std::cos(angle);
            float y1 = cy + (radius - 1.0f) * std::sin(angle);
            float x2 = cx + (radius - 4.0f) * std::cos(angle);
            float y2 = cy + (radius - 4.0f) * std::sin(angle);
            g.drawLine(x1, y1, x2, y2, 1.5f);
        }

        // Main knob body with gradient
        float innerRadius = radius * 0.78f;
        juce::ColourGradient knobGradient(juce::Colour(0xff555555), cx - innerRadius * 0.5f, cy - innerRadius * 0.5f,
                                           juce::Colour(0xff252525), cx + innerRadius * 0.5f, cy + innerRadius * 0.5f, true);
        g.setGradientFill(knobGradient);
        g.fillEllipse(cx - innerRadius, cy - innerRadius, innerRadius * 2.0f, innerRadius * 2.0f);

        // Ring
        g.setColour(juce::Colour(0xff606060));
        g.drawEllipse(cx - innerRadius, cy - innerRadius, innerRadius * 2.0f, innerRadius * 2.0f, 1.0f);

        // Indicator line (7 o'clock to 5 o'clock range)
        float indicatorAngle = juce::jmap(sliderPosProportional, 0.0f, 1.0f, -1.047f, 4.189f) + juce::MathConstants<float>::pi;
        float indicatorLength = innerRadius * 0.65f;
        float ix1 = cx + (innerRadius * 0.2f) * std::cos(indicatorAngle);
        float iy1 = cy + (innerRadius * 0.2f) * std::sin(indicatorAngle);
        float ix2 = cx + indicatorLength * std::cos(indicatorAngle);
        float iy2 = cy + indicatorLength * std::sin(indicatorAngle);
        g.setColour(accentColour);
        g.drawLine(ix1, iy1, ix2, iy2, 3.0f);

        // Center cap
        float capRadius = innerRadius * 0.25f;
        g.setColour(juce::Colour(0xff404040));
        g.fillEllipse(cx - capRadius, cy - capRadius, capRadius * 2.0f, capRadius * 2.0f);
    }

    void drawToggleButton(juce::Graphics& g, juce::ToggleButton& button,
                          bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        auto bounds = button.getLocalBounds().toFloat();
        auto isOn = button.getToggleState();

        // Draw button background
        auto bgColour = isOn ? accentColour : Colors::panelBg;
        if (shouldDrawButtonAsHighlighted)
            bgColour = bgColour.brighter(0.1f);
        if (shouldDrawButtonAsDown)
            bgColour = bgColour.darker(0.1f);

        g.setColour(bgColour);
        g.fillRoundedRectangle(bounds.reduced(2.0f), 4.0f);

        // Draw border
        g.setColour(isOn ? accentColour.brighter(0.2f) : Colors::knobBody);
        g.drawRoundedRectangle(bounds.reduced(2.0f), 4.0f, 1.0f);

        // Draw text
        g.setColour(isOn ? Colors::background : Colors::textPrimary);
        g.setFont(12.0f);
        g.drawText(button.getButtonText(), bounds, juce::Justification::centred);
    }

private:
    juce::Colour accentColour = Colors::accent;
};
