/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

const juce::String targetAddress = "https://www.youtube.com/";

//==============================================================================
/**
*/
class WebViewerAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    WebViewerAudioProcessorEditor (WebViewerAudioProcessor&);
    ~WebViewerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    WebViewerAudioProcessor& audioProcessor;

    juce::WebBrowserComponent webComponent{
        juce::WebBrowserComponent::Options{}
        .withBackend(juce::WebBrowserComponent::Options::Backend::webview2)
        .withWinWebView2Options(
            juce::WebBrowserComponent::Options::WinWebView2{}
            .withUserDataFolder(juce::File::getSpecialLocation(
                juce::File::SpecialLocationType::tempDirectory))
            )
    };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WebViewerAudioProcessorEditor)
};
