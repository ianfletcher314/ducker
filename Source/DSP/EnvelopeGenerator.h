#pragma once

#include <JuceHeader.h>
#include "DSPUtils.h"

class EnvelopeGenerator
{
public:
    EnvelopeGenerator();

    void prepare(double sampleRate, int samplesPerBlock);
    void reset();

    // Process a single sample and return envelope value (0 to 1)
    float processSample(float triggerLevel);

    // Setters
    void setThreshold(float thresholdDb);
    void setAttack(float attackMs);
    void setHold(float holdMs);
    void setRelease(float releaseMs);
    void setCurveShape(DSPUtils::CurveShape shape);

    // Getters
    float getCurrentEnvelope() const { return currentEnvelope; }
    bool isTriggered() const { return triggered; }

private:
    enum class State
    {
        Idle,
        Attack,
        Hold,
        Release
    };

    void updateCoefficients();

    // Parameters
    float threshold = -20.0f;      // dB
    float attackMs = 10.0f;        // ms
    float holdMs = 50.0f;          // ms
    float releaseMs = 200.0f;      // ms
    DSPUtils::CurveShape curveShape = DSPUtils::CurveShape::Linear;

    // Derived coefficients
    float attackCoeff = 0.0f;
    float releaseCoeff = 0.0f;
    int holdSamples = 0;

    // State
    State currentState = State::Idle;
    float currentEnvelope = 0.0f;
    int holdCounter = 0;
    bool triggered = false;

    // Runtime
    double currentSampleRate = 44100.0;
    int currentBlockSize = 512;
};
