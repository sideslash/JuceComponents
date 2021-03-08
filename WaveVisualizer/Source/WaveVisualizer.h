/*
  ==============================================================================

    WaveVisualizer.h
    Created: 6 Mar 2021 7:58:07pm
    Author:  Hao Wu

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class WaveVisualizerSource
{
public:
    virtual ~WaveVisualizerSource()
    {
        
    }

    virtual int historyLength() const = 0;
    
    virtual juce::Array<float> getHistory() = 0;
};


//==============================================================================
class WaveVisualizer  : public juce::Component, public juce::Timer
{
public:
    WaveVisualizer();
    ~WaveVisualizer() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    //----------------------------
    void setSource(WaveVisualizerSource *source);
    void turnOn();
    void turnOff();
    
    //-------------------------
    void timerCallback() override;
private:
    
    std::shared_ptr<WaveVisualizerSource> pSource;
    
    bool isOn { false };
    
    int refreshRate { 10 };
    juce::Image drawingImage;
    std::unique_ptr<juce::Graphics> drawingGraphic;
      
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveVisualizer)
};
