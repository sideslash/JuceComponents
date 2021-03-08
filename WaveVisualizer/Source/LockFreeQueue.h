/*
  ==============================================================================

    LockFreeQueue.h
    Created: 8 Mar 2021 2:18:38pm
    Author:  xhkj

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class LockFreeQueue
{
public:
    LockFreeQueue(int size = 512);
    
    int getNumReady();
    void write(const float* writeData, int numToWrite);
    void read(float* readData, int numToRead);
private:
    int mSize;
    
    //juce::AbstractFifo is more like a convenient calculator for counting the read and write pointer of a RingBuffer
    std::unique_ptr<juce::AbstractFifo> mFifo;
    juce::Array<float> mBuffer;
};
 
