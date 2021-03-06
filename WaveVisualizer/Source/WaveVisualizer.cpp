/*
  ==============================================================================

    WaveVisualizer.cpp
    Created: 6 Mar 2021 7:58:07pm
    Author:  Hao Wu

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveVisualizer.h"

//==============================================================================
WaveVisualizer::WaveVisualizer()
{

}

WaveVisualizer::~WaveVisualizer()
{
    stopTimer();
}

void WaveVisualizer::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    if (!isOn) return;;
    if (pSource == nullptr) return;

    juce::Array<float> history = pSource->getHistory();
    juce::Path path;

    for (int i = 0; i < getWidth(); i++)
    {
        int scaledIndex = float(i*history.size()) / floor(getWidth());
        float val = history[scaledIndex];
        val = juce::jlimit<float>(-1, 1, val);

        if (i == 0)
        {
            path.startNewSubPath(0, 1);
            path.startNewSubPath(0, -1);
            path.startNewSubPath(0, val);
        }
        else path.lineTo(i, val);
    }

    if (path.isEmpty()) return;;
    if (path.getBounds().getWidth() < 0.01) return;

    path.scaleToFit(0, 0, getWidth(), getHeight(), false);

//    historyGraphic->setColour(juce::Colours::white);
//    historyGraphic->strokePath(path, juce::PathStrokeType(2));
    g.setColour(juce::Colours::white);
    g.strokePath(path, juce::PathStrokeType (1));

//    g.drawImageAt(historyImage, 0, 0);

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);
}


void WaveVisualizer::resized()
{
    
}

//==============================================================================
void WaveVisualizer::setSource(WaveVisualizerSource *source)
{
    pSource = std::shared_ptr<WaveVisualizerSource>(source);
}

void WaveVisualizer::turnOn()
{
    isOn = true;
    startTimerHz(refreshRate);
}

void WaveVisualizer::turnOff()
{
    isOn = false;
    stopTimer();
}

//================================================
void WaveVisualizer::timerCallback()
{
    repaint();
}
