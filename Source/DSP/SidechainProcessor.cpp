#include "SidechainProcessor.h"

SidechainProcessor::SidechainProcessor()
{
}

void SidechainProcessor::prepare(double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
    currentBlockSize = samplesPerBlock;
    updateFilters();
    reset();
}

void SidechainProcessor::reset()
{
    // HPF state
    hpX1L = hpX2L = hpY1L = hpY2L = 0.0f;
    hpX1R = hpX2R = hpY1R = hpY2R = 0.0f;

    // LPF state
    lpX1L = lpX2L = lpY1L = lpY2L = 0.0f;
    lpX1R = lpX2R = lpY1R = lpY2R = 0.0f;

    filteredLevel = 0.0f;
}

float SidechainProcessor::processSample(float input)
{
    float output = input;

    // Apply high-pass filter
    if (highPassEnabled)
    {
        float y = hpCoeffs.b0 * output + hpCoeffs.b1 * hpX1L + hpCoeffs.b2 * hpX2L
                  - hpCoeffs.a1 * hpY1L - hpCoeffs.a2 * hpY2L;
        hpX2L = hpX1L;
        hpX1L = output;
        hpY2L = hpY1L;
        hpY1L = y;
        output = y;
    }

    // Apply low-pass filter
    if (lowPassEnabled)
    {
        float y = lpCoeffs.b0 * output + lpCoeffs.b1 * lpX1L + lpCoeffs.b2 * lpX2L
                  - lpCoeffs.a1 * lpY1L - lpCoeffs.a2 * lpY2L;
        lpX2L = lpX1L;
        lpX1L = output;
        lpY2L = lpY1L;
        lpY1L = y;
        output = y;
    }

    filteredLevel = std::abs(output);
    return output;
}

void SidechainProcessor::processBuffer(juce::AudioBuffer<float>& buffer)
{
    auto* leftChannel = buffer.getWritePointer(0);
    auto* rightChannel = buffer.getNumChannels() > 1 ? buffer.getWritePointer(1) : nullptr;

    for (int i = 0; i < buffer.getNumSamples(); ++i)
    {
        float inputL = leftChannel[i];
        float inputR = rightChannel ? rightChannel[i] : inputL;

        float outputL = inputL;
        float outputR = inputR;

        // Apply high-pass filter
        if (highPassEnabled)
        {
            // Left channel
            float yL = hpCoeffs.b0 * outputL + hpCoeffs.b1 * hpX1L + hpCoeffs.b2 * hpX2L
                      - hpCoeffs.a1 * hpY1L - hpCoeffs.a2 * hpY2L;
            hpX2L = hpX1L;
            hpX1L = outputL;
            hpY2L = hpY1L;
            hpY1L = yL;
            outputL = yL;

            // Right channel
            float yR = hpCoeffs.b0 * outputR + hpCoeffs.b1 * hpX1R + hpCoeffs.b2 * hpX2R
                      - hpCoeffs.a1 * hpY1R - hpCoeffs.a2 * hpY2R;
            hpX2R = hpX1R;
            hpX1R = outputR;
            hpY2R = hpY1R;
            hpY1R = yR;
            outputR = yR;
        }

        // Apply low-pass filter
        if (lowPassEnabled)
        {
            // Left channel
            float yL = lpCoeffs.b0 * outputL + lpCoeffs.b1 * lpX1L + lpCoeffs.b2 * lpX2L
                      - lpCoeffs.a1 * lpY1L - lpCoeffs.a2 * lpY2L;
            lpX2L = lpX1L;
            lpX1L = outputL;
            lpY2L = lpY1L;
            lpY1L = yL;
            outputL = yL;

            // Right channel
            float yR = lpCoeffs.b0 * outputR + lpCoeffs.b1 * lpX1R + lpCoeffs.b2 * lpX2R
                      - lpCoeffs.a1 * lpY1R - lpCoeffs.a2 * lpY2R;
            lpX2R = lpX1R;
            lpX1R = outputR;
            lpY2R = lpY1R;
            lpY1R = yR;
            outputR = yR;
        }

        leftChannel[i] = outputL;
        if (rightChannel)
            rightChannel[i] = outputR;
    }
}

void SidechainProcessor::setHighPassFreq(float freq)
{
    highPassFreq = freq;
    updateFilters();
}

void SidechainProcessor::setLowPassFreq(float freq)
{
    lowPassFreq = freq;
    updateFilters();
}

void SidechainProcessor::setHighPassEnabled(bool enabled)
{
    highPassEnabled = enabled;
}

void SidechainProcessor::setLowPassEnabled(bool enabled)
{
    lowPassEnabled = enabled;
}

void SidechainProcessor::updateFilters()
{
    hpCoeffs = DSPUtils::calcHighPass(currentSampleRate, highPassFreq, 0.707f);
    lpCoeffs = DSPUtils::calcLowPass(currentSampleRate, lowPassFreq, 0.707f);
}
