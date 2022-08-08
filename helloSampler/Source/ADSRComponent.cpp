/*
  ==============================================================================

    ADSRComponent.cpp
    Created: 3 Aug 2022 9:35:04am
    Author:  Martin Penberthy

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ADSRComponent.h"


//==============================================================================
ADSRComponent::ADSRComponent(HelloSamplerAudioProcessor& p) : processor(p)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
  
  
  //Set up all the sliders for the ADSR
  //=====================================================//
  mSliderAttack.setSliderStyle (juce::Slider::RotaryVerticalDrag);
  mSliderAttack.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
  mSliderAttack.setRange(0.0f, 5.0f, 0.01f);
  
  mLabelAttack.attachToComponent(&mSliderAttack, false);
  mLabelAttack.setFont(13.0f);
  mLabelAttack.setText("Attack", juce::dontSendNotification);
  mLabelAttack.setJustificationType(juce::Justification::centredTop);
  
  //=====================================================//
  mSliderDecay.setSliderStyle (juce::Slider::RotaryVerticalDrag);
  mSliderDecay.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
  mSliderDecay.setRange(0.0f, 5.0f, 0.01f);
  
  mLabelDecay.attachToComponent(&mSliderDecay, false);
  mLabelDecay.setFont(13.0f);
  mLabelDecay.setText("Decay", juce::dontSendNotification);
  mLabelDecay.setJustificationType(juce::Justification::centredTop);
  
  //=====================================================//
  mSliderSustain.setSliderStyle (juce::Slider::RotaryVerticalDrag);
  mSliderSustain.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
  mSliderSustain.setRange(0.0f, 1.0f, 0.01f);
  
  mLabelSustain.attachToComponent(&mSliderSustain, false);
  mLabelSustain.setFont(13.0f);
  mLabelSustain.setText("Sustain", juce::dontSendNotification);
  mLabelSustain.setJustificationType(juce::Justification::centredTop);
  
  //=====================================================//
  mSliderRelease.setSliderStyle (juce::Slider::RotaryVerticalDrag);
  mSliderRelease.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
  mSliderRelease.setRange(0.0f, 5.0f, 0.01f);
  
  mLabelRelease.attachToComponent(&mSliderRelease, false);
  mLabelRelease.setFont(13.0f);
  mLabelRelease.setText("Release", juce::dontSendNotification);
  mLabelRelease.setJustificationType(juce::Justification::centredTop);
  
  
  
  
  //Attach all the sliders to the correct SliderAttachment
  mAttackSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.apvts, "ATTACK", mSliderAttack);
  mDecaySliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.apvts, "DECAY", mSliderDecay);
  mSustainSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.apvts, "SUSTAIN", mSliderSustain);
  mReleaseSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.apvts, "RELEASE", mSliderRelease);
  
  //Add sliders to the compnent and make them visible
  addAndMakeVisible(mSliderAttack);
  addAndMakeVisible(mSliderDecay);
  addAndMakeVisible(mSliderSustain);
  addAndMakeVisible(mSliderRelease);
}

ADSRComponent::~ADSRComponent()
{
}

void ADSRComponent::paint (juce::Graphics& g)
{

}

void ADSRComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
  //Define valuees for size and placement
  const auto startX = 0.6f;
  const auto startY = 0.2f;
  const auto dialWidth = 0.1f;
  const auto dialHeight = 0.75f;
  
  //Place and set up all the sliders
  mSliderAttack.setBoundsRelative(startX, startY, dialWidth, dialHeight);
  mSliderDecay.setBoundsRelative(startX + dialWidth, startY, dialWidth, dialHeight);
  mSliderSustain.setBoundsRelative(startX + (2 * dialWidth), startY, dialWidth, dialHeight);
  mSliderRelease.setBoundsRelative(startX + (3 * dialWidth), startY, dialWidth, dialHeight);

}
