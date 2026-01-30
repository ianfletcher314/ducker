#pragma once

#include <JuceHeader.h>
#include "LookAndFeel.h"

class LevelMeter : public juce::Component
{
public:
    LevelMeter(bool isVertical = true) : vertical(isVertical) {}

    void setLevel(float newLevel) { level = newLevel; repaint(); }

    void paint(juce::Graphics& g) override
    {
        auto bounds = getLocalBounds().toFloat().reduced(1.0f);

        // Background
        g.setColour(juce::Colour(0xff151515));
        g.fillRoundedRectangle(bounds, 3.0f);

        // Convert to dB and normalize
        float db = juce::Decibels::gainToDecibels(level, -60.0f);
        float normalized = juce::jmap(db, -60.0f, 0.0f, 0.0f, 1.0f);
        normalized = juce::jlimit(0.0f, 1.0f, normalized);

        // Level bar with color zones
        juce::Colour barColour;
        if (normalized < 0.6f)
            barColour = Colors::meterGreen;
        else if (normalized < 0.85f)
            barColour = Colors::meterYellow;
        else
            barColour = Colors::meterRed;

        g.setColour(barColour);

        if (vertical)
        {
            float barHeight = bounds.getHeight() * normalized;
            g.fillRoundedRectangle(bounds.getX(), bounds.getBottom() - barHeight,
                                    bounds.getWidth(), barHeight, 2.0f);
        }
        else
        {
            float barWidth = bounds.getWidth() * normalized;
            g.fillRoundedRectangle(bounds.getX(), bounds.getY(),
                                    barWidth, bounds.getHeight(), 2.0f);
        }
    }

private:
    float level = 0.0f;
    bool vertical;
};

class GainReductionMeter : public juce::Component
{
public:
    GainReductionMeter(bool isVertical = true) : vertical(isVertical) {}

    void setGainReduction(float gr) { gainReduction = gr; repaint(); }

    void paint(juce::Graphics& g) override
    {
        auto bounds = getLocalBounds().toFloat().reduced(1.0f);

        // Background
        g.setColour(juce::Colour(0xff151515));
        g.fillRoundedRectangle(bounds, 3.0f);

        // Normalize GR (0-20 dB range)
        float normalized = juce::jmap(gainReduction, 0.0f, 20.0f, 0.0f, 1.0f);
        normalized = juce::jlimit(0.0f, 1.0f, normalized);

        // GR meter typically shows in orange/amber
        g.setColour(Colors::meterYellow);

        if (vertical)
        {
            // GR meter shows from top down
            float barHeight = bounds.getHeight() * normalized;
            g.fillRoundedRectangle(bounds.getX(), bounds.getY(),
                                    bounds.getWidth(), barHeight, 2.0f);
        }
        else
        {
            float barWidth = bounds.getWidth() * normalized;
            g.fillRoundedRectangle(bounds.getX(), bounds.getY(),
                                    barWidth, bounds.getHeight(), 2.0f);
        }
    }

private:
    float gainReduction = 0.0f;
    bool vertical;
};
