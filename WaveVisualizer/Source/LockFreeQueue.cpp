/*
  ==============================================================================

    LockFreeQueue.cpp
    Created: 8 Mar 2021 2:18:38pm
    Author:  xhkj

  ==============================================================================
*/

#include "LockFreeQueue.h"

LockFreeQueue::LockFreeQueue(int size) : mSize(size)
{
    mFifo = std::make_unique<juce::AbstractFifo>(mSize);
    
    mBuffer.ensureStorageAllocated(mSize);
    juce::FloatVectorOperations::clear(mBuffer.getRawDataPointer(), mSize);
    
    while (mBuffer.size() < mSize) {
        mBuffer.add(1.0);
    }
    
}

void LockFreeQueue::writeFrom(const float* writeData, int numToWrite)
{
    int startIndex1, blockSize1, startIndex2, blockSize2;
    
    mFifo->prepareToWrite(numToWrite, startIndex1, blockSize1, startIndex2, blockSize2);
    
    if (blockSize1 > 0)
        juce::FloatVectorOperations::copy(mBuffer.getRawDataPointer() + startIndex1, writeData, blockSize1);
    if (blockSize2 > 0)
        juce::FloatVectorOperations::copy(mBuffer.getRawDataPointer() + startIndex2, writeData + blockSize1, blockSize2);
    
    mFifo->finishedWrite(numToWrite);
}

void LockFreeQueue::readTo(float* readData, int numToRead)
{
    int startIndex1, blockSize1, startIndex2, blockSize2;
    
    mFifo->prepareToRead(numToRead, startIndex1, blockSize1, startIndex2, blockSize2);
    
    if (blockSize1 > 0)
        juce::FloatVectorOperations::copy(readData, mBuffer.getRawDataPointer() + startIndex1, blockSize1);
    if (blockSize2 > 0)
        juce::FloatVectorOperations::copy(readData + blockSize1, mBuffer.getRawDataPointer() + startIndex2, blockSize2);
    
    mFifo->finishedRead(blockSize1 + blockSize2);
}

void LockFreeQueue::dropExcess(int gateNum)
{
    while (getNumReady() > gateNum) {
        mFifo->finishedRead(getNumReady() - gateNum);
    }
}

int LockFreeQueue::getNumReady()
{
    return mFifo->getNumReady();
}
