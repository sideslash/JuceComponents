/*
  ==============================================================================

    LockFreeQueue.h
    Created: 10 Mar 2021 3:29:50pm
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
    void writeFrom(const float* writeData, int numToWrite);
    void readTo(float* readData, int numToRead);
    void dropExcess(int gateNum);
    
private:
    int mSize;
    
    //juce::AbstractFifo is more like a convenient calculator for counting the read and write pointer of a RingBuffer
    std::unique_ptr<juce::AbstractFifo> mFifo;
    juce::Array<float> mBuffer;
};
 
