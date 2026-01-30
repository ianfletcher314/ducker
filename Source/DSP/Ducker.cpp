#include "Ducker.h"

Ducker::Ducker()
{
}

void Ducker::prepare(double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
    currentBlockSize = samplesPerBlock;

    // Prepare sub-modules
    envelopeGenerator.prepare(sampleRate, samplesPerBlock);
    sidechainProcessor.prepare(sampleRate, samplesPerBlock);

    // Initialize look-ahead delay lines (max 20ms at sample rate)
    int maxDelaySamples = static_cast<int>(0.02 * sampleRate) + 1;
    delayLineL.resize(maxDelaySamples, 0.0f);
    delayLineR.resize(maxDelaySamples, 0.0f);

    updateLookAhead();
    reset();
}

void Ducker::reset()
{
    envelopeGenerator.reset();
    sidechainProcessor.reset();

    std::fill(delayLineL.begin(), delayLineL.end(), 0.0f);
    std::fill(delayLineR.begin(), delayLineR.end(), 0.0f);
    delayWritePos = 0;

    currentGainReduction = 0.0f;
    lastSampleL = lastSampleR = 0.0f;
    pendingGainL = pendingGainR = 1.0f;
    targetGain = 1.0f;
}

void Ducker::process(juce::AudioBuffer<float>& mainBuffer, const juce::AudioBuffer<float>& sidechainBuffer)
{
    if (bypassed)
        return;

    auto numSamples = mainBuffer.getNumSamples();
    auto numChannels = mainBuffer.getNumChannels();

    auto* mainL = mainBuffer.getWritePointer(0);
    auto* mainR = numChannels > 1 ? mainBuffer.getWritePointer(1) : nullptr;

    // Get sidechain input (mono sum if stereo)
    auto* scL = sidechainBuffer.getReadPointer(0);
    auto* scR = sidechainBuffer.getNumChannels() > 1 ? sidechainBuffer.getReadPointer(1) : nullptr;

    float maxGainReduction = 0.0f;

    // Calculate mix coefficients
    float wetMix = mix / 100.0f;
    float dryMix = 1.0f - wetMix;

    for (int i = 0; i < numSamples; ++i)
    {
        // Sum sidechain to mono and process through filters
        float scInput = scL[i];
        if (scR != nullptr)
            scInput = (scInput + scR[i]) * 0.5f;

        float filteredSC = sidechainProcessor.processSample(scInput);

        // Get envelope from sidechain
        float envelope = envelopeGenerator.processSample(filteredSC);

        // Calculate gain reduction
        // envelope goes 0->1 as sidechain triggers
        // gainReduction should go 1->duckLinear as envelope goes 0->1
        float duckLinear = DSPUtils::decibelsToLinear(duckAmount);
        float rangeLinear = DSPUtils::decibelsToLinear(range);

        // Clamp ducked level to range (floor)
        float duckedGain = std::max(duckLinear, rangeLinear);
        targetGain = 1.0f - envelope * (1.0f - duckedGain);

        float gain = targetGain;

        // Zero-crossing detection for click-free ducking
        if (zeroCrossingEnabled)
        {
            // Left channel
            float currentL = mainL[i];
            bool zeroCrossL = (lastSampleL >= 0.0f && currentL < 0.0f) ||
                              (lastSampleL <= 0.0f && currentL > 0.0f);
            if (zeroCrossL || std::abs(currentL) < 0.001f)
                pendingGainL = gain;
            lastSampleL = currentL;

            // Right channel
            if (mainR)
            {
                float currentR = mainR[i];
                bool zeroCrossR = (lastSampleR >= 0.0f && currentR < 0.0f) ||
                                  (lastSampleR <= 0.0f && currentR > 0.0f);
                if (zeroCrossR || std::abs(currentR) < 0.001f)
                    pendingGainR = gain;
                lastSampleR = currentR;
            }
        }
        else
        {
            pendingGainL = pendingGainR = gain;
        }

        // Write to delay line (for look-ahead)
        delayLineL[delayWritePos] = mainL[i];
        if (mainR)
            delayLineR[delayWritePos] = mainR[i];

        // Read from delay line
        int readPos = delayWritePos - lookAheadSamples;
        if (readPos < 0)
            readPos += static_cast<int>(delayLineL.size());

        float dryL = delayLineL[readPos];
        float dryR = mainR ? delayLineR[readPos] : dryL;

        // Apply gain with mix (parallel ducking)
        float wetL = dryL * pendingGainL;
        float wetR = dryR * pendingGainR;

        mainL[i] = dryL * dryMix + wetL * wetMix;
        if (mainR)
            mainR[i] = dryR * dryMix + wetR * wetMix;

        // Sidechain listen mode - replace output with filtered sidechain
        if (sidechainListen)
        {
            mainL[i] = filteredSC;
            if (mainR)
                mainR[i] = filteredSC;
        }

        // Update write position
        delayWritePos = (delayWritePos + 1) % static_cast<int>(delayLineL.size());

        // Track gain reduction for metering
        float grDb = DSPUtils::linearToDecibels(gain);
        maxGainReduction = std::max(maxGainReduction, -grDb);
    }

    currentGainReduction = maxGainReduction;
}

float Ducker::processSample(float input, float sidechainInput)
{
    // Filter sidechain
    float filteredSC = sidechainProcessor.processSample(sidechainInput);

    // Get envelope
    float envelope = envelopeGenerator.processSample(filteredSC);

    // Calculate gain
    float duckLinear = DSPUtils::decibelsToLinear(duckAmount);
    float rangeLinear = DSPUtils::decibelsToLinear(range);
    float duckedGain = std::max(duckLinear, rangeLinear);
    float gain = 1.0f - envelope * (1.0f - duckedGain);

    return input * gain;
}

void Ducker::setThreshold(float thresholdDb)
{
    threshold = thresholdDb;
    envelopeGenerator.setThreshold(thresholdDb);
}

void Ducker::setDuckAmount(float amountDb)
{
    duckAmount = amountDb;
}

void Ducker::setAttack(float ms)
{
    attackMs = ms;
    envelopeGenerator.setAttack(ms);
}

void Ducker::setHold(float ms)
{
    holdMs = ms;
    envelopeGenerator.setHold(ms);
}

void Ducker::setRelease(float ms)
{
    releaseMs = ms;
    envelopeGenerator.setRelease(ms);
}

void Ducker::setRange(float rangeDb)
{
    range = rangeDb;
}

void Ducker::setLookAhead(float ms)
{
    lookAheadMs = ms;
    updateLookAhead();
}

void Ducker::setCurveShape(int shapeIndex)
{
    curveShape = static_cast<DSPUtils::CurveShape>(shapeIndex);
    envelopeGenerator.setCurveShape(curveShape);
}

void Ducker::setMix(float mixPercent)
{
    mix = mixPercent;
}

void Ducker::setBypass(bool shouldBypass)
{
    bypassed = shouldBypass;
}

void Ducker::setSidechainHPF(float freq)
{
    sidechainProcessor.setHighPassFreq(freq);
}

void Ducker::setSidechainLPF(float freq)
{
    sidechainProcessor.setLowPassFreq(freq);
}

void Ducker::setSidechainHPFEnabled(bool enabled)
{
    sidechainProcessor.setHighPassEnabled(enabled);
}

void Ducker::setSidechainLPFEnabled(bool enabled)
{
    sidechainProcessor.setLowPassEnabled(enabled);
}

void Ducker::setSidechainListen(bool listen)
{
    sidechainListen = listen;
}

void Ducker::setZeroCrossingEnabled(bool enabled)
{
    zeroCrossingEnabled = enabled;
}

void Ducker::updateLookAhead()
{
    lookAheadSamples = static_cast<int>(lookAheadMs * 0.001f * currentSampleRate);
}
