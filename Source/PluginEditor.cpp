#include "PluginProcessor.h"
#include "PluginEditor.h"

DuckerAudioProcessorEditor::DuckerAudioProcessorEditor(DuckerAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    lookAndFeel = std::make_unique<DuckerLookAndFeel>();
    setLookAndFeel(lookAndFeel.get());

    // Setup main controls
    setupSlider(thresholdSlider, thresholdLabel, "Threshold");
    setupSlider(duckAmountSlider, duckAmountLabel, "Duck");
    setupSlider(attackSlider, attackLabel, "Attack");
    setupSlider(holdSlider, holdLabel, "Hold");
    setupSlider(releaseSlider, releaseLabel, "Release");
    setupSlider(rangeSlider, rangeLabel, "Range");
    setupSlider(lookAheadSlider, lookAheadLabel, "Look-Ahead");
    setupSlider(mixSlider, mixLabel, "Mix");

    // Curve shape selector
    curveShapeSelector.addItemList(juce::StringArray{ "Linear", "Exponential", "Logarithmic", "S-Curve" }, 1);
    curveShapeSelector.setSelectedItemIndex(0);
    addAndMakeVisible(curveShapeSelector);

    curveShapeLabel.setText("Curve", juce::dontSendNotification);
    curveShapeLabel.setJustificationType(juce::Justification::centred);
    curveShapeLabel.setColour(juce::Label::textColourId, Colors::textSecondary);
    curveShapeLabel.setFont(juce::Font(11.0f));
    addAndMakeVisible(curveShapeLabel);

    // Bypass button
    setupButton(bypassButton, "Bypass");

    // Sidechain filter controls
    setupSlider(scHPFSlider, scHPFLabel, "SC HPF");
    setupSlider(scLPFSlider, scLPFLabel, "SC LPF");
    setupButton(scHPFButton, "HPF");
    setupButton(scLPFButton, "LPF");
    setupButton(scListenButton, "SC Listen");
    setupButton(zeroCrossingButton, "Zero-X");

    // Tempo sync controls
    setupButton(tempoSyncButton, "Sync");

    holdSyncSelector.addItemList(juce::StringArray{ "1/64", "1/32", "1/16T", "1/16", "1/8T", "1/8", "1/4T", "1/4", "1/2", "1 Bar" }, 1);
    holdSyncSelector.setSelectedItemIndex(3);
    addAndMakeVisible(holdSyncSelector);

    holdSyncLabel.setText("Hold Sync", juce::dontSendNotification);
    holdSyncLabel.setJustificationType(juce::Justification::centred);
    holdSyncLabel.setColour(juce::Label::textColourId, Colors::textSecondary);
    holdSyncLabel.setFont(juce::Font(11.0f));
    addAndMakeVisible(holdSyncLabel);

    releaseSyncSelector.addItemList(juce::StringArray{ "1/64", "1/32", "1/16T", "1/16", "1/8T", "1/8", "1/4T", "1/4", "1/2", "1 Bar" }, 1);
    releaseSyncSelector.setSelectedItemIndex(5);
    addAndMakeVisible(releaseSyncSelector);

    releaseSyncLabel.setText("Rel Sync", juce::dontSendNotification);
    releaseSyncLabel.setJustificationType(juce::Justification::centred);
    releaseSyncLabel.setColour(juce::Label::textColourId, Colors::textSecondary);
    releaseSyncLabel.setFont(juce::Font(11.0f));
    addAndMakeVisible(releaseSyncLabel);

    // Add meters
    addAndMakeVisible(inputMeter);
    addAndMakeVisible(outputMeter);
    addAndMakeVisible(grMeter);

    // Add envelope display
    addAndMakeVisible(envelopeDisplay);

    // Create attachments
    thresholdAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), "threshold", thresholdSlider);
    duckAmountAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), "duckAmount", duckAmountSlider);
    attackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), "attack", attackSlider);
    holdAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), "hold", holdSlider);
    releaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), "release", releaseSlider);
    rangeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), "range", rangeSlider);
    lookAheadAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), "lookAhead", lookAheadSlider);
    mixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), "mix", mixSlider);
    curveShapeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        audioProcessor.getAPVTS(), "curveShape", curveShapeSelector);
    bypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.getAPVTS(), "bypass", bypassButton);

    scHPFAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), "scHPFFreq", scHPFSlider);
    scLPFAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), "scLPFFreq", scLPFSlider);
    scHPFEnabledAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.getAPVTS(), "scHPFEnabled", scHPFButton);
    scLPFEnabledAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.getAPVTS(), "scLPFEnabled", scLPFButton);
    scListenAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.getAPVTS(), "scListen", scListenButton);
    zeroCrossingAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.getAPVTS(), "zeroCrossing", zeroCrossingButton);

    tempoSyncAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.getAPVTS(), "tempoSync", tempoSyncButton);
    holdSyncAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        audioProcessor.getAPVTS(), "holdSync", holdSyncSelector);
    releaseSyncAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        audioProcessor.getAPVTS(), "releaseSync", releaseSyncSelector);

    setSize(700, 450);
    startTimerHz(30);
}

DuckerAudioProcessorEditor::~DuckerAudioProcessorEditor()
{
    stopTimer();
    setLookAndFeel(nullptr);
}

void DuckerAudioProcessorEditor::setupSlider(juce::Slider& slider, juce::Label& label, const juce::String& name)
{
    slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 18);
    slider.setColour(juce::Slider::textBoxTextColourId, Colors::textPrimary);
    slider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::transparentBlack);
    slider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    addAndMakeVisible(slider);

    label.setText(name, juce::dontSendNotification);
    label.setJustificationType(juce::Justification::centred);
    label.setColour(juce::Label::textColourId, Colors::textSecondary);
    label.setFont(juce::Font(11.0f));
    addAndMakeVisible(label);
}

void DuckerAudioProcessorEditor::setupButton(juce::ToggleButton& button, const juce::String& name)
{
    button.setButtonText(name);
    addAndMakeVisible(button);
}

void DuckerAudioProcessorEditor::paint(juce::Graphics& g)
{
    // Background gradient
    juce::ColourGradient bgGradient(juce::Colour(0xff1a1a1a), 0.0f, 0.0f,
                                     juce::Colour(0xff121212), 0.0f, (float)getHeight(), false);
    g.setGradientFill(bgGradient);
    g.fillAll();

    // Header
    g.setColour(juce::Colour(0xff252525));
    g.fillRect(0, 0, getWidth(), 50);

    // Title
    g.setColour(Colors::textPrimary);
    g.setFont(juce::Font(24.0f, juce::Font::bold));
    g.drawText("DUCKER", 20, 10, 200, 30, juce::Justification::left);

    g.setColour(Colors::textSecondary);
    g.setFont(juce::Font(12.0f));
    g.drawText("Sidechain Ducker", 20, 32, 200, 16, juce::Justification::left);

    // Section labels
    g.setColour(Colors::accent);
    g.setFont(juce::Font(11.0f, juce::Font::bold));
    g.drawText("MAIN", 20, 60, 100, 16, juce::Justification::left);
    g.drawText("ENVELOPE", 20, 200, 100, 16, juce::Justification::left);
    g.drawText("SIDECHAIN", 360, 60, 100, 16, juce::Justification::left);
    g.drawText("TEMPO SYNC", 360, 200, 100, 16, juce::Justification::left);

    // Meter labels
    g.setColour(Colors::textSecondary);
    g.setFont(juce::Font(10.0f));
    g.drawText("IN", getWidth() - 80, 60, 20, 14, juce::Justification::centred);
    g.drawText("GR", getWidth() - 55, 60, 20, 14, juce::Justification::centred);
    g.drawText("OUT", getWidth() - 30, 60, 20, 14, juce::Justification::centred);
}

void DuckerAudioProcessorEditor::resized()
{
    int knobSize = 70;
    int labelHeight = 18;
    int margin = 10;

    // Main section - row 1 (Threshold, Duck, Range, Mix)
    int row1Y = 80;
    thresholdSlider.setBounds(20, row1Y, knobSize, knobSize);
    thresholdLabel.setBounds(20, row1Y + knobSize, knobSize, labelHeight);

    duckAmountSlider.setBounds(100, row1Y, knobSize, knobSize);
    duckAmountLabel.setBounds(100, row1Y + knobSize, knobSize, labelHeight);

    rangeSlider.setBounds(180, row1Y, knobSize, knobSize);
    rangeLabel.setBounds(180, row1Y + knobSize, knobSize, labelHeight);

    mixSlider.setBounds(260, row1Y, knobSize, knobSize);
    mixLabel.setBounds(260, row1Y + knobSize, knobSize, labelHeight);

    // Envelope section - row 2 (Attack, Hold, Release, Look-ahead, Curve)
    int row2Y = 220;
    attackSlider.setBounds(20, row2Y, knobSize, knobSize);
    attackLabel.setBounds(20, row2Y + knobSize, knobSize, labelHeight);

    holdSlider.setBounds(100, row2Y, knobSize, knobSize);
    holdLabel.setBounds(100, row2Y + knobSize, knobSize, labelHeight);

    releaseSlider.setBounds(180, row2Y, knobSize, knobSize);
    releaseLabel.setBounds(180, row2Y + knobSize, knobSize, labelHeight);

    lookAheadSlider.setBounds(260, row2Y, knobSize, knobSize);
    lookAheadLabel.setBounds(260, row2Y + knobSize, knobSize, labelHeight);

    curveShapeSelector.setBounds(20, row2Y + knobSize + 25, 130, 24);
    curveShapeLabel.setBounds(20, row2Y + knobSize + 50, 130, labelHeight);

    // Sidechain section
    int scX = 360;
    scHPFSlider.setBounds(scX, row1Y, knobSize - 10, knobSize - 10);
    scHPFLabel.setBounds(scX, row1Y + knobSize - 10, knobSize - 10, labelHeight);
    scHPFButton.setBounds(scX, row1Y + knobSize + 8, 55, 22);

    scLPFSlider.setBounds(scX + 70, row1Y, knobSize - 10, knobSize - 10);
    scLPFLabel.setBounds(scX + 70, row1Y + knobSize - 10, knobSize - 10, labelHeight);
    scLPFButton.setBounds(scX + 70, row1Y + knobSize + 8, 55, 22);

    scListenButton.setBounds(scX + 140, row1Y, 70, 26);
    zeroCrossingButton.setBounds(scX + 140, row1Y + 32, 70, 26);

    // Tempo sync section
    tempoSyncButton.setBounds(scX, row2Y, 60, 26);

    holdSyncSelector.setBounds(scX + 70, row2Y, 80, 24);
    holdSyncLabel.setBounds(scX + 70, row2Y + 26, 80, labelHeight);

    releaseSyncSelector.setBounds(scX + 160, row2Y, 80, 24);
    releaseSyncLabel.setBounds(scX + 160, row2Y + 26, 80, labelHeight);

    // Bypass button
    bypassButton.setBounds(getWidth() - 90, 12, 70, 26);

    // Envelope display
    envelopeDisplay.setBounds(20, 360, 460, 75);

    // Meters
    int meterWidth = 16;
    int meterHeight = 280;
    int meterY = 75;
    inputMeter.setBounds(getWidth() - 80, meterY, meterWidth, meterHeight);
    grMeter.setBounds(getWidth() - 55, meterY, meterWidth, meterHeight);
    outputMeter.setBounds(getWidth() - 30, meterY, meterWidth, meterHeight);
}

void DuckerAudioProcessorEditor::timerCallback()
{
    // Smooth level metering
    float targetIn = audioProcessor.getInputLevel();
    float targetOut = audioProcessor.getOutputLevel();
    float targetGR = audioProcessor.getGainReduction();

    smoothedInputLevel = smoothedInputLevel * 0.8f + targetIn * 0.2f;
    smoothedOutputLevel = smoothedOutputLevel * 0.8f + targetOut * 0.2f;
    smoothedGR = smoothedGR * 0.8f + targetGR * 0.2f;

    // Faster decay when signal drops
    if (targetIn < smoothedInputLevel)
        smoothedInputLevel *= 0.92f;
    if (targetOut < smoothedOutputLevel)
        smoothedOutputLevel *= 0.92f;
    if (targetGR < smoothedGR)
        smoothedGR *= 0.92f;

    inputMeter.setLevel(smoothedInputLevel);
    outputMeter.setLevel(smoothedOutputLevel);
    grMeter.setGainReduction(smoothedGR);

    // Update envelope display
    envelopeDisplay.setEnvelopeValue(audioProcessor.getEnvelopeValue());
    envelopeDisplay.setAttackMs(attackSlider.getValue());
    envelopeDisplay.setHoldMs(holdSlider.getValue());
    envelopeDisplay.setReleaseMs(releaseSlider.getValue());
    envelopeDisplay.setDuckAmount((float)duckAmountSlider.getValue());
    envelopeDisplay.setCurveShape(curveShapeSelector.getSelectedItemIndex());
    envelopeDisplay.setTriggered(audioProcessor.isTriggered());

    // Repaint meters and display
    inputMeter.repaint();
    outputMeter.repaint();
    grMeter.repaint();
    envelopeDisplay.repaint();
}
