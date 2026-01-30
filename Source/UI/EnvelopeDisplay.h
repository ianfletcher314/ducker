#pragma once

#include <JuceHeader.h>
#include "LookAndFeel.h"
#include "../DSP/DSPUtils.h"

class EnvelopeDisplay : public juce::Component
{
public:
    EnvelopeDisplay()
    {
        // Initialize envelope history
        envelopeHistory.resize(historySize, 0.0f);
    }

    void setEnvelopeValue(float value)
    {
        // Shift history and add new value
        for (int i = historySize - 1; i > 0; --i)
            envelopeHistory[i] = envelopeHistory[i - 1];
        envelopeHistory[0] = value;
        repaint();
    }

    void setAttackMs(float ms) { attackMs = ms; }
    void setHoldMs(float ms) { holdMs = ms; }
    void setReleaseMs(float ms) { releaseMs = ms; }
    void setDuckAmount(float db) { duckAmountDb = db; }
    void setCurveShape(int shape) { curveShape = static_cast<DSPUtils::CurveShape>(shape); }
    void setTriggered(bool trig) { triggered = trig; }

    void paint(juce::Graphics& g) override
    {
        auto bounds = getLocalBounds().toFloat().reduced(2.0f);

        // Background
        g.setColour(juce::Colour(0xff151515));
        g.fillRoundedRectangle(bounds, 4.0f);

        // Border
        g.setColour(juce::Colour(0xff303030));
        g.drawRoundedRectangle(bounds, 4.0f, 1.0f);

        // Draw grid lines
        g.setColour(juce::Colour(0xff252525));
        for (int i = 1; i < 4; ++i)
        {
            float y = bounds.getY() + bounds.getHeight() * (i / 4.0f);
            g.drawHorizontalLine((int)y, bounds.getX(), bounds.getRight());
        }

        // Draw duck curve shape preview
        drawCurvePreview(g, bounds);

        // Draw envelope history
        drawEnvelopeHistory(g, bounds);

        // Draw labels
        g.setColour(Colors::textSecondary);
        g.setFont(10.0f);
        g.drawText("0 dB", bounds.getX() + 4, (int)bounds.getY() + 2, 40, 12, juce::Justification::left);
        g.drawText(juce::String(duckAmountDb, 0) + " dB", bounds.getX() + 4,
                   (int)bounds.getBottom() - 14, 50, 12, juce::Justification::left);

        // Trigger indicator
        if (triggered)
        {
            g.setColour(Colors::led);
            g.fillEllipse(bounds.getRight() - 12, bounds.getY() + 4, 8, 8);
        }
    }

private:
    void drawCurvePreview(juce::Graphics& g, juce::Rectangle<float>& bounds)
    {
        // Calculate total time for the envelope display
        float totalTime = attackMs + holdMs + releaseMs;
        if (totalTime <= 0.0f) return;

        juce::Path curvePath;
        float padding = 10.0f;
        auto drawArea = bounds.reduced(padding);

        bool pathStarted = false;
        for (float t = 0.0f; t <= totalTime; t += totalTime / 100.0f)
        {
            float envelope = 0.0f;

            if (t < attackMs)
            {
                // Attack phase
                float attackProgress = t / attackMs;
                envelope = DSPUtils::applyCurveShape(attackProgress, curveShape);
            }
            else if (t < attackMs + holdMs)
            {
                // Hold phase
                envelope = 1.0f;
            }
            else
            {
                // Release phase
                float releaseProgress = (t - attackMs - holdMs) / releaseMs;
                releaseProgress = juce::jlimit(0.0f, 1.0f, releaseProgress);
                envelope = 1.0f - DSPUtils::applyCurveShape(releaseProgress, curveShape);
            }

            float x = drawArea.getX() + (t / totalTime) * drawArea.getWidth();
            float y = drawArea.getY() + (1.0f - envelope) * drawArea.getHeight();

            if (!pathStarted)
            {
                curvePath.startNewSubPath(x, y);
                pathStarted = true;
            }
            else
            {
                curvePath.lineTo(x, y);
            }
        }

        // Draw the curve
        g.setColour(Colors::duckBlue.withAlpha(0.3f));
        g.strokePath(curvePath, juce::PathStrokeType(2.0f));
    }

    void drawEnvelopeHistory(juce::Graphics& g, juce::Rectangle<float>& bounds)
    {
        float padding = 10.0f;
        auto drawArea = bounds.reduced(padding);

        juce::Path historyPath;
        bool pathStarted = false;

        for (int i = 0; i < historySize; ++i)
        {
            float x = drawArea.getRight() - (i / (float)historySize) * drawArea.getWidth();
            float y = drawArea.getY() + (1.0f - envelopeHistory[i]) * drawArea.getHeight();

            if (!pathStarted)
            {
                historyPath.startNewSubPath(x, y);
                pathStarted = true;
            }
            else
            {
                historyPath.lineTo(x, y);
            }
        }

        // Fill under the curve
        juce::Path fillPath = historyPath;
        fillPath.lineTo(drawArea.getX(), drawArea.getBottom());
        fillPath.lineTo(drawArea.getRight(), drawArea.getBottom());
        fillPath.closeSubPath();

        g.setColour(Colors::duckPurple.withAlpha(0.15f));
        g.fillPath(fillPath);

        // Draw the history line
        g.setColour(Colors::duckPurple);
        g.strokePath(historyPath, juce::PathStrokeType(2.0f));
    }

    std::vector<float> envelopeHistory;
    static constexpr int historySize = 128;

    float attackMs = 10.0f;
    float holdMs = 50.0f;
    float releaseMs = 200.0f;
    float duckAmountDb = -20.0f;
    DSPUtils::CurveShape curveShape = DSPUtils::CurveShape::Linear;
    bool triggered = false;
};
