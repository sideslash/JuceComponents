/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "WaveVisualizer.h"

//==============================================================================
/**
*/
class JuceComponentsAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    JuceComponentsAudioProcessorEditor (JuceComponentsAudioProcessor&);
    ~JuceComponentsAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    WaveVisualizer visulizer;
    
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    JuceComponentsAudioProcessor& audioProcessor;

//    juce::Array<float> history;
    
//    juce::AudioDeviceManager otherDeviceManager;
//    std::unique_ptr<juce::AudioDeviceSelectorComponent> audioSetting;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JuceComponentsAudioProcessorEditor)
};
