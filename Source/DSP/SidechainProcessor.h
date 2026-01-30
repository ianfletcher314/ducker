#pragma once

#include <JuceHeader.h>
#include "DSPUtils.h"

class SidechainProcessor
{
public:
    SidechainProcessor();

    void prepare(double sampleRate, int samplesPerBlock);
    void reset();

    // Process sidechain signal and return filtered result
    float processSample(float input);

    // Process entire buffer (for sidechain listen mode)
    void processBuffer(juce::AudioBuffer<float>& buffer);

    // Setters
    void setHighPassFreq(float freq);
    void setLowPassFreq(float freq);
    void setHighPassEnabled(bool enabled);
    void setLowPassEnabled(bool enabled);

    // Getters
    float getFilteredLevel() const { return filteredLevel; }

private:
    void updateFilters();

    // Filter parameters
    float highPassFreq = 20.0f;   // Hz
    float lowPassFreq = 20000.0f; // Hz
    bool highPassEnabled = false;
    bool lowPassEnabled = false;

    // Biquad filter state (stereo)
    DSPUtils::BiquadCoeffs hpCoeffs;
    DSPUtils::BiquadCoeffs lpCoeffs;

    // HPF state
    float hpX1L = 0.0f, hpX2L = 0.0f, hpY1L = 0.0f, hpY2L = 0.0f;
    float hpX1R = 0.0f, hpX2R = 0.0f, hpY1R = 0.0f, hpY2R = 0.0f;

    // LPF state
    float lpX1L = 0.0f, lpX2L = 0.0f, lpY1L = 0.0f, lpY2L = 0.0f;
    float lpX1R = 0.0f, lpX2R = 0.0f, lpY1R = 0.0f, lpY2R = 0.0f;

    float filteredLevel = 0.0f;

    // Runtime
    double currentSampleRate = 44100.0;
    int currentBlockSize = 512;
};
