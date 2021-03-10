/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JuceComponentsAudioProcessorEditor::JuceComponentsAudioProcessorEditor (JuceComponentsAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (400, 600);
    
    visulizer.setSource(&audioProcessor);
    addAndMakeVisible(visulizer);
    visulizer.turnOn();
}

JuceComponentsAudioProcessorEditor::~JuceComponentsAudioProcessorEditor()
{
    visulizer.turnOff();
}

//==============================================================================
void JuceComponentsAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void JuceComponentsAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();

    visulizer.setBounds(bounds.removeFromTop(200).reduced(10));
    
//    audioSetting->setBounds(bounds.reduced(10));
}
