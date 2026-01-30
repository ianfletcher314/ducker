#pragma once

#include <JuceHeader.h>
#include "DSP/Ducker.h"

class DuckerAudioProcessor : public juce::AudioProcessor
{
public:
    DuckerAudioProcessor();
    ~DuckerAudioProcessor() override;

    // Standard AudioProcessor overrides
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    using AudioProcessor::processBlock;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    // Public API for editor access
    juce::AudioProcessorValueTreeState& getAPVTS() { return apvts; }

    // Metering accessors
    float getInputLevel() const { return inputLevel.load(); }
    float getOutputLevel() const { return outputLevel.load(); }
    float getGainReduction() const { return ducker.getGainReduction(); }
    float getEnvelopeValue() const { return ducker.getEnvelopeValue(); }
    bool isTriggered() const { return ducker.isTriggered(); }

private:
    juce::AudioProcessorValueTreeState apvts;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    // DSP Module
    Ducker ducker;

    // Sidechain buffer
    juce::AudioBuffer<float> sidechainBuffer;

    // Parameter pointers (cached for fast access)
    std::atomic<float>* threshold = nullptr;
    std::atomic<float>* duckAmount = nullptr;
    std::atomic<float>* attack = nullptr;
    std::atomic<float>* hold = nullptr;
    std::atomic<float>* release = nullptr;
    std::atomic<float>* range = nullptr;
    std::atomic<float>* lookAhead = nullptr;
    std::atomic<float>* curveShape = nullptr;
    std::atomic<float>* mix = nullptr;
    std::atomic<float>* bypass = nullptr;

    std::atomic<float>* scHPFFreq = nullptr;
    std::atomic<float>* scLPFFreq = nullptr;
    std::atomic<float>* scHPFEnabled = nullptr;
    std::atomic<float>* scLPFEnabled = nullptr;
    std::atomic<float>* scListen = nullptr;
    std::atomic<float>* zeroCrossing = nullptr;

    std::atomic<float>* tempoSync = nullptr;
    std::atomic<float>* holdSync = nullptr;
    std::atomic<float>* releaseSync = nullptr;

    // Metering state
    std::atomic<float> inputLevel { 0.0f };
    std::atomic<float> outputLevel { 0.0f };

    // Runtime
    double currentSampleRate = 44100.0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DuckerAudioProcessor)
};
