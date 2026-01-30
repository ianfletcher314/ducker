#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UI/LookAndFeel.h"
#include "UI/MeterComponents.h"
#include "UI/EnvelopeDisplay.h"

class DuckerAudioProcessorEditor : public juce::AudioProcessorEditor,
                                    public juce::Timer
{
public:
    DuckerAudioProcessorEditor(DuckerAudioProcessor&);
    ~DuckerAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;

private:
    DuckerAudioProcessor& audioProcessor;

    // Custom LookAndFeel
    std::unique_ptr<DuckerLookAndFeel> lookAndFeel;

    // Main controls
    juce::Slider thresholdSlider, duckAmountSlider;
    juce::Slider attackSlider, holdSlider, releaseSlider;
    juce::Slider rangeSlider, lookAheadSlider, mixSlider;
    juce::ComboBox curveShapeSelector;
    juce::ToggleButton bypassButton;

    // Sidechain controls
    juce::Slider scHPFSlider, scLPFSlider;
    juce::ToggleButton scHPFButton, scLPFButton;
    juce::ToggleButton scListenButton;
    juce::ToggleButton zeroCrossingButton;

    // Tempo sync controls
    juce::ToggleButton tempoSyncButton;
    juce::ComboBox holdSyncSelector, releaseSyncSelector;

    // Labels
    juce::Label thresholdLabel, duckAmountLabel;
    juce::Label attackLabel, holdLabel, releaseLabel;
    juce::Label rangeLabel, lookAheadLabel, mixLabel;
    juce::Label curveShapeLabel;
    juce::Label scHPFLabel, scLPFLabel;
    juce::Label holdSyncLabel, releaseSyncLabel;

    // Meters
    LevelMeter inputMeter, outputMeter;
    GainReductionMeter grMeter;

    // Envelope display
    EnvelopeDisplay envelopeDisplay;

    // Smoothed metering values
    float smoothedInputLevel = 0.0f;
    float smoothedOutputLevel = 0.0f;
    float smoothedGR = 0.0f;

    // APVTS Attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> thresholdAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> duckAmountAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> holdAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> releaseAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rangeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lookAheadAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> curveShapeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> bypassAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> scHPFAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> scLPFAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> scHPFEnabledAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> scLPFEnabledAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> scListenAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> zeroCrossingAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> tempoSyncAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> holdSyncAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> releaseSyncAttachment;

    void setupSlider(juce::Slider& slider, juce::Label& label, const juce::String& name);
    void setupButton(juce::ToggleButton& button, const juce::String& name);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DuckerAudioProcessorEditor)
};
