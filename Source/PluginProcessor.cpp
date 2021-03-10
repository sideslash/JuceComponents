/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JuceComponentsAudioProcessor::JuceComponentsAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

JuceComponentsAudioProcessor::~JuceComponentsAudioProcessor()
{
}

//==============================================================================
const juce::String JuceComponentsAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool JuceComponentsAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool JuceComponentsAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool JuceComponentsAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double JuceComponentsAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int JuceComponentsAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int JuceComponentsAudioProcessor::getCurrentProgram()
{
    return 0;
}

void JuceComponentsAudioProcessor::setCurrentProgram (int index)
{
    
}

const juce::String JuceComponentsAudioProcessor::getProgramName (int index)
{
    return {};
}

void JuceComponentsAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void JuceComponentsAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void JuceComponentsAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool JuceComponentsAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void JuceComponentsAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
//    for (int channel = 0; channel < totalNumInputChannels; ++channel)
//    {
//        auto* channelData = buffer.getWritePointer (channel);
//
//        // ..do something to the data...
//    }
//

    const int numberSamples = buffer.getNumSamples();

    const int startSample = strideOffset > 0 ? (stride - strideOffset) : 0;
    // ceiling for dividing result of two integer: (M+N-1) / N
    const int numToWrite = ((numberSamples - startSample) + stride - 1) / stride;

    //put one in history after each 10 samples
    for (int i = 0; i < numToWrite; i++)
    {        float sample = buffer.getSample(0, startSample + stride * i);
        historyQueue.writeFrom(&sample, 1);
    }

    strideOffset = (numberSamples + strideOffset) % stride;
}

//==============================================================================
bool JuceComponentsAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* JuceComponentsAudioProcessor::createEditor()
{
    return new JuceComponentsAudioProcessorEditor (*this);
}

//==============================================================================
void JuceComponentsAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void JuceComponentsAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new JuceComponentsAudioProcessor();
}

//==============================================================================
int JuceComponentsAudioProcessor::getNumReady()
{
    return historyQueue.getNumReady();
}

int JuceComponentsAudioProcessor::fill(juce::Array<float>* bufferToFill, int pos)
{
    //DRAIN the excess
    historyQueue.dropExcess(bufferToFill->size());
    
    const int numToAppend = historyQueue.getNumReady();
    
    if (pos + numToAppend < bufferToFill->size())
    {
        historyQueue.readTo(&bufferToFill->getRawDataPointer()[pos], numToAppend);
    }
    else
    {
        int numToEnd = bufferToFill->size() - pos;
        historyQueue.readTo(&bufferToFill->getRawDataPointer()[pos], numToEnd);
        historyQueue.readTo(&bufferToFill->getRawDataPointer()[0], numToAppend - numToEnd);
    }
    
    return numToAppend;
}
