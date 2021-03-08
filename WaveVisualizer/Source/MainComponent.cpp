#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() : juce::AudioAppComponent(otherDeviceManager)
{
    otherDeviceManager.initialise(1, 2, nullptr, true);
    audioSetting.reset(new juce::AudioDeviceSelectorComponent(otherDeviceManager, 0, 1, 0, 2, true, true, false, true));
    addAndMakeVisible(audioSetting.get());
    
    setSize (500, 800);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
    
//    for (int i = 0; i < historyLength(); i++)
//        history.add(0);
    
    visulizer.setSource(this);
    addAndMakeVisible(visulizer);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    visulizer.turnOn();
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    const int numberSamples = bufferToFill.numSamples;
    
    const int stride = 20;
    const int numToWrite = (numberSamples - currentHistoryOffset) / stride;
    const int offsetStart = currentHistoryOffset%stride;

    //put one in history after each 10 samples
    for (int i = 0; i < numToWrite; i++)
    {
        float sample = bufferToFill.buffer->getSample(0, offsetStart + stride * i);
        historyQueue.write(&sample, 1);
    }

    currentHistoryOffset = (currentHistoryOffset + numberSamples) % stride;
    
//    juce::Array<float> block;
//    block.ensureStorageAllocated(testSize);
//    juce::FloatVectorOperations::clear(block.getRawDataPointer(), testSize);
//    while (block.size() < testSize) {
//        block.add(0);
//    }
//
//    juce::FloatVectorOperations::copy(block.getRawDataPointer(), testSource.getRawDataPointer() + testPointer, testSize);
//    testPointer += testSize;
//
//    DBG("------------------ testblock ----------------------");
//    for (int i = 0; i < block.size(); i++)
//    {
//        DBG(block[i]);
//    }
//
//    const int numToWrite = testSize > 0 ? (testSize - offset) / stride : 0;
//    const int offsetStart = currentHistoryOffset%stride;
//
//    //put one in history after each 10 samples
//    for (int i = 0; i < numToWrite; i++)
//    {
//        float sample = block[offsetStart + stride*i];
//        testBuffer.add(sample);
//    }
//
//    offset = (offset + testSize) % stride;
//
//
//    DBG("------------------ testBuffer ----------------------");
//    for (int i = 0; i < testBuffer.size(); i++)
//    {
//        DBG(testBuffer[i]);
//    }
//
//    bufferToFill.clearActiveBufferRegion();
}

void MainComponent::releaseResources()
{
    visulizer.turnOff();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
    
    
}

void MainComponent::resized()
{
    auto bounds = getLocalBounds();

    visulizer.setBounds(bounds.removeFromTop(200).reduced(10));
    
    audioSetting->setBounds(bounds.reduced(10));
}


//==============================================================================
//int MainComponent::historyLength() const
//{
//    return 400;
//}
//
//juce::Array<float> MainComponent::getHistory()
//{
//    return history;
//}

int MainComponent::getNumReady()
{
    return historyQueue.getNumReady();
}

int MainComponent::fill(juce::Array<float>* bufferToFill, int pos)
{
    return 0;
}
