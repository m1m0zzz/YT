/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WebViewerAudioProcessorEditor::WebViewerAudioProcessorEditor (WebViewerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    auto size = audioProcessor.getSavedSize();
    setSize(size.x, size.y);
    setResizable(true, true);

    addAndMakeVisible(webComponent);
    webComponent.goToURL(targetAddress);
}

WebViewerAudioProcessorEditor::~WebViewerAudioProcessorEditor()
{
}

//==============================================================================
void WebViewerAudioProcessorEditor::paint(juce::Graphics& g) {
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void WebViewerAudioProcessorEditor::resized() {
    webComponent.setBounds(getLocalBounds());
    audioProcessor.setSavedSize({ getWidth(), getHeight() });
}
