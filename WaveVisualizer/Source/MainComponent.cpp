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
    
    for (int i = 0; i < historyLength(); i++)
        history.add(0);
    
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
    
    //put one in history after each 10 samples
    for (int i = 0; i < numberSamples; i+=10)
    {
//        DBG(bufferToFill.buffer->getSample(0, i));
        history.add(bufferToFill.buffer->getSample(0, i));
        
        if (history.size() > historyLength())
            history.remove(0);
    }
    
    bufferToFill.clearActiveBufferRegion();
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
int MainComponent::historyLength() const
{
    return 400;
}

juce::Array<float> MainComponent::getHistory()
{
    return history;
}
