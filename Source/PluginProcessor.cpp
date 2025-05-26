/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WebViewerAudioProcessor::WebViewerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
  : AudioProcessor (BusesProperties()
    #if ! JucePlugin_IsMidiEffect
    #if ! JucePlugin_IsSynth
      .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
    #endif
      .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
    #endif
      )
#endif
    , state("State", {}, {})
{
}

WebViewerAudioProcessor::~WebViewerAudioProcessor()
{
}

//==============================================================================
const juce::String WebViewerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool WebViewerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool WebViewerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool WebViewerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double WebViewerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int WebViewerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int WebViewerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void WebViewerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String WebViewerAudioProcessor::getProgramName (int index)
{
    return {};
}

void WebViewerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void WebViewerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void WebViewerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool WebViewerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void WebViewerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    //auto totalNumInputChannels  = getTotalNumInputChannels();
    //auto totalNumOutputChannels = getTotalNumOutputChannels();
}

//==============================================================================
bool WebViewerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* WebViewerAudioProcessor::createEditor()
{
    return new WebViewerAudioProcessorEditor (*this);
}

//==============================================================================
void WebViewerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    DBG("getStateInformation");

    auto editor = state.getOrCreateChildWithName("editor", nullptr);
    editor.setProperty("sizeX", editorSize.x, nullptr);
    editor.setProperty("sizeY", editorSize.y, nullptr);
    juce::MemoryOutputStream stream(destData, false);
    state.writeToStream(stream);
}

void WebViewerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    DBG("setStateInformation");

    juce::ValueTree tree = juce::ValueTree::readFromData(data, sizeInBytes);
    if (tree.isValid()) {
        DBG("valid");
        auto editor = tree.getChildWithName("editor");
        editorSize.setX(editor.getProperty("sizeX", 1280));
        editorSize.setY(editor.getProperty("sizeY", 720));
        if (auto* activeEditor = getActiveEditor()) {
            activeEditor->setSize(editorSize.x, editorSize.y);
        }
    }
    else {
        editorSize.setX(1280);
        editorSize.setY(720);
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WebViewerAudioProcessor();
}

//==============================================================================
juce::Point<int> WebViewerAudioProcessor::getSavedSize() const
{
    return editorSize;
}

void WebViewerAudioProcessor::setSavedSize(const juce::Point<int>& size)
{
    editorSize = size;
}