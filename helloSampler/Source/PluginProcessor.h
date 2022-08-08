/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
using namespace juce;
//==============================================================================
/**
*/
class HelloSamplerAudioProcessor  : public juce::AudioProcessor, public ValueTree::Listener
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    HelloSamplerAudioProcessor();
    ~HelloSamplerAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
  void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

  void loadFile();
  void loadFile(const String &path);
  
  int getNumSamplerSounds() { return mSampler.getNumSounds() ; }
  AudioBuffer<float>& getWaveform(){ return mWaveform; };

  AudioProcessorValueTreeState apvts;
  
  void updateADSR();
  ADSR::Parameters& getADSRParams(){ return mADSRParams; }
  
  
private:
  Synthesiser mSampler;//Synth object
  const int mNumVoices = 3; //Hardcoded value for the number of voices we allow

  AudioFormatManager mFormatManager; //Format manager for opening and reading files
  AudioFormatReader* mFormatReader { nullptr };//Format reader for opening and reading files
  
  AudioBuffer<float> mWaveform; //AudioBuffer to store audio data for use in WaveThumbnail
  
  AudioProcessorValueTreeState::ParameterLayout createParameters(); //Setup for APVTS
  ADSR::Parameters mADSRParams; //Object for storing ADSR paramter values
  
  //override for the apvts
  void valueTreePropertyChanged (ValueTree &treeWhosePropertyHasChanged, const Identifier &property) override;
  std::atomic<bool> mShouldUpdate { false };

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HelloSamplerAudioProcessor)
};
