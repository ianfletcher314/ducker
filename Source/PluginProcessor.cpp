#include "PluginProcessor.h"
#include "PluginEditor.h"

DuckerAudioProcessor::DuckerAudioProcessor()
     : AudioProcessor(BusesProperties()
                      .withInput("Input", juce::AudioChannelSet::stereo(), true)
                      .withInput("Sidechain", juce::AudioChannelSet::stereo(), true)
                      .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
       apvts(*this, nullptr, "Parameters", createParameterLayout())
{
    // Cache parameter pointers
    threshold = apvts.getRawParameterValue("threshold");
    duckAmount = apvts.getRawParameterValue("duckAmount");
    attack = apvts.getRawParameterValue("attack");
    hold = apvts.getRawParameterValue("hold");
    release = apvts.getRawParameterValue("release");
    range = apvts.getRawParameterValue("range");
    lookAhead = apvts.getRawParameterValue("lookAhead");
    curveShape = apvts.getRawParameterValue("curveShape");
    mix = apvts.getRawParameterValue("mix");
    bypass = apvts.getRawParameterValue("bypass");

    scHPFFreq = apvts.getRawParameterValue("scHPFFreq");
    scLPFFreq = apvts.getRawParameterValue("scLPFFreq");
    scHPFEnabled = apvts.getRawParameterValue("scHPFEnabled");
    scLPFEnabled = apvts.getRawParameterValue("scLPFEnabled");
    scListen = apvts.getRawParameterValue("scListen");
    zeroCrossing = apvts.getRawParameterValue("zeroCrossing");

    tempoSync = apvts.getRawParameterValue("tempoSync");
    holdSync = apvts.getRawParameterValue("holdSync");
    releaseSync = apvts.getRawParameterValue("releaseSync");
}

DuckerAudioProcessor::~DuckerAudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout DuckerAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    // Main ducking parameters
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("threshold", 1), "Threshold",
        juce::NormalisableRange<float>(-60.0f, 0.0f, 0.1f, 1.0f),
        -20.0f,
        juce::AudioParameterFloatAttributes().withLabel("dB")));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("duckAmount", 1), "Duck Amount",
        juce::NormalisableRange<float>(-40.0f, 0.0f, 0.1f, 1.0f),
        -20.0f,
        juce::AudioParameterFloatAttributes().withLabel("dB")));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("attack", 1), "Attack",
        juce::NormalisableRange<float>(0.1f, 100.0f, 0.1f, 0.4f),
        10.0f,
        juce::AudioParameterFloatAttributes().withLabel("ms")));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("hold", 1), "Hold",
        juce::NormalisableRange<float>(0.0f, 500.0f, 1.0f, 0.5f),
        50.0f,
        juce::AudioParameterFloatAttributes().withLabel("ms")));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("release", 1), "Release",
        juce::NormalisableRange<float>(10.0f, 2000.0f, 1.0f, 0.4f),
        200.0f,
        juce::AudioParameterFloatAttributes().withLabel("ms")));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("range", 1), "Range/Floor",
        juce::NormalisableRange<float>(-80.0f, 0.0f, 0.1f, 1.0f),
        -40.0f,
        juce::AudioParameterFloatAttributes().withLabel("dB")));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("lookAhead", 1), "Look-Ahead",
        juce::NormalisableRange<float>(0.0f, 20.0f, 0.1f, 0.5f),
        5.0f,
        juce::AudioParameterFloatAttributes().withLabel("ms")));

    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID("curveShape", 1), "Curve Shape",
        juce::StringArray{ "Linear", "Exponential", "Logarithmic", "S-Curve" }, 0));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("mix", 1), "Mix",
        juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f, 1.0f),
        100.0f,
        juce::AudioParameterFloatAttributes().withLabel("%")));

    params.push_back(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID("bypass", 1), "Bypass", false));

    // Sidechain filter parameters
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("scHPFFreq", 1), "SC HPF Freq",
        juce::NormalisableRange<float>(20.0f, 2000.0f, 1.0f, 0.4f),
        80.0f,
        juce::AudioParameterFloatAttributes().withLabel("Hz")));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("scLPFFreq", 1), "SC LPF Freq",
        juce::NormalisableRange<float>(1000.0f, 20000.0f, 1.0f, 0.4f),
        12000.0f,
        juce::AudioParameterFloatAttributes().withLabel("Hz")));

    params.push_back(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID("scHPFEnabled", 1), "SC HPF On", false));

    params.push_back(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID("scLPFEnabled", 1), "SC LPF On", false));

    params.push_back(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID("scListen", 1), "SC Listen", false));

    params.push_back(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID("zeroCrossing", 1), "Zero Cross", false));

    // Tempo sync parameters
    params.push_back(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID("tempoSync", 1), "Tempo Sync", false));

    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID("holdSync", 1), "Hold Sync",
        juce::StringArray{ "1/64", "1/32", "1/16T", "1/16", "1/8T", "1/8", "1/4T", "1/4", "1/2", "1 Bar" }, 3));

    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID("releaseSync", 1), "Release Sync",
        juce::StringArray{ "1/64", "1/32", "1/16T", "1/16", "1/8T", "1/8", "1/4T", "1/4", "1/2", "1 Bar" }, 5));

    return { params.begin(), params.end() };
}

const juce::String DuckerAudioProcessor::getName() const { return JucePlugin_Name; }
bool DuckerAudioProcessor::acceptsMidi() const { return false; }
bool DuckerAudioProcessor::producesMidi() const { return false; }
bool DuckerAudioProcessor::isMidiEffect() const { return false; }
double DuckerAudioProcessor::getTailLengthSeconds() const { return 0.0; }
int DuckerAudioProcessor::getNumPrograms() { return 1; }
int DuckerAudioProcessor::getCurrentProgram() { return 0; }
void DuckerAudioProcessor::setCurrentProgram(int index) { juce::ignoreUnused(index); }
const juce::String DuckerAudioProcessor::getProgramName(int index) { juce::ignoreUnused(index); return {}; }
void DuckerAudioProcessor::changeProgramName(int index, const juce::String& newName) { juce::ignoreUnused(index, newName); }

void DuckerAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
    ducker.prepare(sampleRate, samplesPerBlock);

    // Allocate sidechain buffer
    sidechainBuffer.setSize(2, samplesPerBlock);

    // Report latency for look-ahead compensation
    setLatencySamples(ducker.getLatencyInSamples());
}

void DuckerAudioProcessor::releaseResources()
{
    ducker.reset();
}

bool DuckerAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    // Main output must be mono or stereo
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // Main input and output must match
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;

    // Sidechain can be mono or stereo (or disabled)
    auto scChannels = layouts.getChannelSet(true, 1);
    if (!scChannels.isDisabled()
        && scChannels != juce::AudioChannelSet::mono()
        && scChannels != juce::AudioChannelSet::stereo())
        return false;

    return true;
}

void DuckerAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused(midiMessages);
    juce::ScopedNoDenormals noDenormals;

    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Clear unused output channels
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // Check bypass
    if (bypass->load() > 0.5f)
        return;

    // Measure input level
    float inLevel = 0.0f;
    for (int ch = 0; ch < juce::jmin(2, (int)totalNumInputChannels); ++ch)
        inLevel = std::max(inLevel, buffer.getMagnitude(ch, 0, buffer.getNumSamples()));
    inputLevel.store(inLevel);

    // Get sidechain input
    auto mainBus = getBusBuffer(buffer, true, 0);
    auto sidechainBus = getBusBuffer(buffer, true, 1);

    // Copy sidechain to separate buffer (or use main input if no sidechain)
    sidechainBuffer.setSize(2, buffer.getNumSamples(), false, false, true);
    if (sidechainBus.getNumChannels() > 0)
    {
        for (int ch = 0; ch < sidechainBuffer.getNumChannels(); ++ch)
        {
            int srcCh = juce::jmin(ch, sidechainBus.getNumChannels() - 1);
            sidechainBuffer.copyFrom(ch, 0, sidechainBus, srcCh, 0, buffer.getNumSamples());
        }
    }
    else
    {
        // No sidechain connected - use main input
        for (int ch = 0; ch < sidechainBuffer.getNumChannels(); ++ch)
        {
            int srcCh = juce::jmin(ch, mainBus.getNumChannels() - 1);
            sidechainBuffer.copyFrom(ch, 0, mainBus, srcCh, 0, buffer.getNumSamples());
        }
    }

    // Calculate tempo-synced times if enabled
    float holdTimeMs = hold->load();
    float releaseTimeMs = release->load();

    if (tempoSync->load() > 0.5f)
    {
        auto playHead = getPlayHead();
        if (playHead != nullptr)
        {
            auto position = playHead->getPosition();
            if (position.hasValue() && position->getBpm().hasValue())
            {
                double bpm = *position->getBpm();
                double beatLengthMs = 60000.0 / bpm;

                // Note divisions: 1/64, 1/32, 1/16T, 1/16, 1/8T, 1/8, 1/4T, 1/4, 1/2, 1 Bar
                const float divisions[] = { 0.0625f, 0.125f, 0.167f, 0.25f, 0.333f, 0.5f, 0.667f, 1.0f, 2.0f, 4.0f };

                int holdIdx = static_cast<int>(holdSync->load());
                int releaseIdx = static_cast<int>(releaseSync->load());

                holdTimeMs = static_cast<float>(beatLengthMs * divisions[holdIdx]);
                releaseTimeMs = static_cast<float>(beatLengthMs * divisions[releaseIdx]);
            }
        }
    }

    // Update ducker parameters
    ducker.setThreshold(threshold->load());
    ducker.setDuckAmount(duckAmount->load());
    ducker.setAttack(attack->load());
    ducker.setHold(holdTimeMs);
    ducker.setRelease(releaseTimeMs);
    ducker.setRange(range->load());
    ducker.setLookAhead(lookAhead->load());
    ducker.setCurveShape(static_cast<int>(curveShape->load()));
    ducker.setMix(mix->load());

    ducker.setSidechainHPF(scHPFFreq->load());
    ducker.setSidechainLPF(scLPFFreq->load());
    ducker.setSidechainHPFEnabled(scHPFEnabled->load() > 0.5f);
    ducker.setSidechainLPFEnabled(scLPFEnabled->load() > 0.5f);
    ducker.setSidechainListen(scListen->load() > 0.5f);
    ducker.setZeroCrossingEnabled(zeroCrossing->load() > 0.5f);

    // Update latency if look-ahead changed
    setLatencySamples(ducker.getLatencyInSamples());

    // Process ducking
    ducker.process(mainBus, sidechainBuffer);

    // Measure output level
    float outLevel = 0.0f;
    for (int ch = 0; ch < juce::jmin(2, (int)totalNumInputChannels); ++ch)
        outLevel = std::max(outLevel, mainBus.getMagnitude(ch, 0, mainBus.getNumSamples()));
    outputLevel.store(outLevel);
}

bool DuckerAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* DuckerAudioProcessor::createEditor()
{
    return new DuckerAudioProcessorEditor(*this);
}

void DuckerAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void DuckerAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    if (xml != nullptr && xml->hasTagName(apvts.state.getType()))
        apvts.replaceState(juce::ValueTree::fromXml(*xml));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DuckerAudioProcessor();
}
