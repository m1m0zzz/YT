/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Frontend.h"

const juce::String targetAddress = "https://www.youtube.com/";

const juce::String jsCode = R"(
console.log('hello, user script')

const onload = () => {
    console.log('onload')
    console.log(window.__JUCE__)
    console.log(getNativeFunction)
    const video = document.querySelector('video')
    if (!video) {
        console.warn('video tag not find!')
        return;
    }
    console.log(video)
}

window.addEventListener('load', onload)
)";

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
        .withUserScript(frontendLib)
        .withUserScript(jsCode)
    };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WebViewerAudioProcessorEditor)
};
