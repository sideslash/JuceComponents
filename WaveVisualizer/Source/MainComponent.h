#pragma once

#include <JuceHeader.h>
#include "WaveVisualizer.h"
#include "LockFreeQueue.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent, public WaveVisualizerSource
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

    //==============================================================================
//    int historyLength() const override;
//    juce::Array<float> getHistory() override;
    int getNumReady()  override;
    int fill(juce::Array<float>* bufferToFill, int pos) override;
    
private:
    //==============================================================================
    // Your private member variables go here...
    WaveVisualizer visulizer;
    
//    juce::Array<float> history;
    
    juce::AudioDeviceManager otherDeviceManager;
    std::unique_ptr<juce::AudioDeviceSelectorComponent> audioSetting;
    
    LockFreeQueue historyQueue;
    //current sample offset for next block of samples to count in
    int currentHistoryOffset = 0;
    
    
    juce::Array<float> testSource {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};
    juce::Array<float> testBuffer;
    int testSize { 10 };
    int testPointer { 0 };
    int stride { 3 };
    int offset { 0 };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
