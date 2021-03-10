/*
  ==============================================================================

    WaveVisualizer.h
    Created: 10 Mar 2021 3:28:00pm
    Author:  xhkj

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

    virtual int getNumReady() = 0;
    //return number of filled samples
    virtual int fill(juce::Array<float>* bufferToFill, int pos) = 0;
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
    
    int refreshRate { 20 };
    juce::Image drawingImage;
    std::unique_ptr<juce::Graphics> drawingGraphic;
      
    juce::Array<float> history;
    int historySize { 512 };
    int histroyPos { 0 }; //local write position
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveVisualizer)
};
