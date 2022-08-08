/*
  ==============================================================================

    ADSRComponent.h
    Created: 3 Aug 2022 9:35:04am
    Author:  Martin Penberthy

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class ADSRComponent  : public juce::Component
{
public:
    ADSRComponent(HelloSamplerAudioProcessor& p);
    ~ADSRComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
  //Sliders
  juce::Slider mSliderAttack, mSliderDecay, mSliderSustain, mSliderRelease;
  //Labels
  juce::Label mLabelAttack, mLabelDecay, mLabelSustain, mLabelRelease;
  
  //Slider Atttachments
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mAttackSliderAttachment, mDecaySliderAttachment, mSustainSliderAttachment, mReleaseSliderAttachment;
  
  HelloSamplerAudioProcessor &processor;  
  
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ADSRComponent)
};
