/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "WaveThumbnail.h"
#include "ADSRComponent.h"

//==============================================================================
/**
*/
class HelloSamplerAudioProcessorEditor  : public juce::AudioProcessorEditor/*,
                                          public juce::Slider::Listener*/
{
public:
    HelloSamplerAudioProcessorEditor (HelloSamplerAudioProcessor&);
    ~HelloSamplerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
  //WaveThumbnail object
  WaveThumbnail mWaveThumbnail;
  ADSRComponent mADSR; //ADSRComponent
  
  juce::Slider mSliderVol;//Volume slider
  juce::Label mLabelVol; //Volume label
  //SliderAttachment for the volume slider
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mVolSliderAttachment;
  
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    HelloSamplerAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HelloSamplerAudioProcessorEditor)
};
