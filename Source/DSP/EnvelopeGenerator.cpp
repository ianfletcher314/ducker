#include "EnvelopeGenerator.h"

EnvelopeGenerator::EnvelopeGenerator()
{
}

void EnvelopeGenerator::prepare(double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
    currentBlockSize = samplesPerBlock;
    updateCoefficients();
    reset();
}

void EnvelopeGenerator::reset()
{
    currentEnvelope = 0.0f;
    holdCounter = 0;
    currentState = State::Idle;
    triggered = false;
}

float EnvelopeGenerator::processSample(float triggerLevel)
{
    // Convert input level to dB
    float inputDb = DSPUtils::linearToDecibels(std::abs(triggerLevel));

    // Check if we should trigger
    bool shouldTrigger = inputDb > threshold;

    if (shouldTrigger)
    {
        triggered = true;
        holdCounter = holdSamples;

        if (currentState == State::Idle || currentState == State::Release)
        {
            currentState = State::Attack;
        }
        else if (currentState == State::Hold)
        {
            // Re-trigger hold
            holdCounter = holdSamples;
        }
    }

    // State machine
    switch (currentState)
    {
        case State::Idle:
            currentEnvelope = 0.0f;
            break;

        case State::Attack:
            currentEnvelope += attackCoeff * (1.0f - currentEnvelope);
            if (currentEnvelope >= 0.999f)
            {
                currentEnvelope = 1.0f;
                currentState = State::Hold;
            }
            break;

        case State::Hold:
            currentEnvelope = 1.0f;
            if (--holdCounter <= 0 && !shouldTrigger)
            {
                currentState = State::Release;
            }
            break;

        case State::Release:
            currentEnvelope -= releaseCoeff * currentEnvelope;
            if (currentEnvelope < 0.001f)
            {
                currentEnvelope = 0.0f;
                currentState = State::Idle;
                triggered = false;
            }
            break;
    }

    // Apply curve shape to the envelope
    return DSPUtils::applyCurveShape(currentEnvelope, curveShape);
}

void EnvelopeGenerator::setThreshold(float thresholdDb)
{
    threshold = thresholdDb;
}

void EnvelopeGenerator::setAttack(float ms)
{
    attackMs = ms;
    updateCoefficients();
}

void EnvelopeGenerator::setHold(float ms)
{
    holdMs = ms;
    updateCoefficients();
}

void EnvelopeGenerator::setRelease(float ms)
{
    releaseMs = ms;
    updateCoefficients();
}

void EnvelopeGenerator::setCurveShape(DSPUtils::CurveShape shape)
{
    curveShape = shape;
}

void EnvelopeGenerator::updateCoefficients()
{
    attackCoeff = DSPUtils::calculateCoefficient(currentSampleRate, attackMs);
    releaseCoeff = DSPUtils::calculateCoefficient(currentSampleRate, releaseMs);
    holdSamples = static_cast<int>(holdMs * 0.001f * currentSampleRate);
}
