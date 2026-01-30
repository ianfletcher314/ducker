#pragma once

#include <JuceHeader.h>
#include "DSPUtils.h"
#include "EnvelopeGenerator.h"
#include "SidechainProcessor.h"

class Ducker
{
public:
    Ducker();

    void prepare(double sampleRate, int samplesPerBlock);
    void process(juce::AudioBuffer<float>& mainBuffer, const juce::AudioBuffer<float>& sidechainBuffer);
    void reset();

    // Parameter setters
    void setThreshold(float thresholdDb);
    void setDuckAmount(float amountDb);
    void setAttack(float attackMs);
    void setHold(float holdMs);
    void setRelease(float releaseMs);
    void setRange(float rangeDb);
    void setLookAhead(float lookAheadMs);
    void setCurveShape(int shapeIndex);
    void setMix(float mixPercent);
    void setBypass(bool shouldBypass);

    // Sidechain filter setters
    void setSidechainHPF(float freq);
    void setSidechainLPF(float freq);
    void setSidechainHPFEnabled(bool enabled);
    void setSidechainLPFEnabled(bool enabled);
    void setSidechainListen(bool listen);

    // Zero-crossing option
    void setZeroCrossingEnabled(bool enabled);

    // Getters for metering/visualization
    float getGainReduction() const { return currentGainReduction; }
    float getEnvelopeValue() const { return envelopeGenerator.getCurrentEnvelope(); }
    bool isTriggered() const { return envelopeGenerator.isTriggered(); }
    bool isSidechainListening() const { return sidechainListen; }

    // Get latency in samples (for look-ahead)
    int getLatencyInSamples() const { return lookAheadSamples; }

private:
    float processSample(float input, float sidechainInput);
    void updateLookAhead();

    // DSP Modules
    EnvelopeGenerator envelopeGenerator;
    SidechainProcessor sidechainProcessor;

    // Parameters
    float threshold = -20.0f;      // dB
    float duckAmount = -20.0f;     // dB (how much to reduce)
    float attackMs = 10.0f;        // ms
    float holdMs = 50.0f;          // ms
    float releaseMs = 200.0f;      // ms
    float range = -40.0f;          // dB (floor/minimum level)
    float lookAheadMs = 5.0f;      // ms
    float mix = 100.0f;            // % (parallel ducking)
    DSPUtils::CurveShape curveShape = DSPUtils::CurveShape::Linear;

    // State
    float currentGainReduction = 0.0f;
    bool bypassed = false;
    bool sidechainListen = false;
    bool zeroCrossingEnabled = false;

    // Look-ahead delay line
    std::vector<float> delayLineL;
    std::vector<float> delayLineR;
    int delayWritePos = 0;
    int lookAheadSamples = 0;

    // Zero-crossing state
    float lastSampleL = 0.0f;
    float lastSampleR = 0.0f;
    float pendingGainL = 1.0f;
    float pendingGainR = 1.0f;
    float targetGain = 1.0f;

    // Runtime
    double currentSampleRate = 44100.0;
    int currentBlockSize = 512;
};
