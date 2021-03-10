/*
  ==============================================================================

    WaveVisualizer.cpp
    Created: 10 Mar 2021 3:28:00pm
    Author:  xhkj

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveVisualizer.h"

//==============================================================================
WaveVisualizer::WaveVisualizer()
{
    history.ensureStorageAllocated(historySize);
    for (int i = 0; i < historySize; i++)
    {
        history.add(0);
    }
}

WaveVisualizer::~WaveVisualizer()
{
    stopTimer();
}

void WaveVisualizer::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);
    
    juce::Path path;
    float max = 0;
    
    if (isOn && pSource != nullptr)
    {
        if (pSource->getNumReady() > 0)
        {
            int numAdded = pSource->fill(&history, histroyPos);
            histroyPos = (histroyPos + numAdded) % historySize;
    
        }
        
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

        // find the maximum sample value
        max = juce::FloatVectorOperations::findMaximum(history.getRawDataPointer(), history.size());
        max = juce::jlimit<float>(0, 2, max);
    }
    

    if (path.isEmpty()) return;;
    if (path.getBounds().getWidth() < 0.01) return;

    // rescale path to fit size
    path.scaleToFit(0, 0.1*drawingImage.getHeight(), drawingImage.getWidth(), 0.8*drawingImage.getHeight(), false);

    // fade
    drawingImage.multiplyAllAlphas(0.5);
    
    // move wave form
    drawingImage.moveImageSection(-3, -10, 0, 0, drawingImage.getWidth(), drawingImage.getHeight());
    
    // Gradient
    juce::Colour c = juce::Colours::green;
    c = c.withRotatedHue(0.5*max);
    juce::ColourGradient grad(c, drawingImage.getWidth()/2, drawingImage.getHeight()/2, c.withBrightness(0.8*max), 0, drawingImage.getHeight()/2, true);
    drawingGraphic->setGradientFill(grad);
    
    // stroke wave path onto image's graphics
    drawingGraphic->strokePath(path, juce::PathStrokeType(2));
    
    //draw wave image
    g.drawImageAt(drawingImage, 0, 0);
    
    // draw border
//    g.drawRect (getLocalBounds(), 2);
}


void WaveVisualizer::resized()
{
    drawingImage = juce::Image(juce::Image::PixelFormat::ARGB, getWidth(), getHeight(), true);
    drawingGraphic = std::make_unique<juce::Graphics>(drawingImage);
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
