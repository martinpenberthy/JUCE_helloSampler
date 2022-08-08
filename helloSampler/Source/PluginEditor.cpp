/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
HelloSamplerAudioProcessorEditor::HelloSamplerAudioProcessorEditor (HelloSamplerAudioProcessor& p)
    : AudioProcessorEditor (&p), mWaveThumbnail(p), mADSR(p), audioProcessor (p)
{

  mSliderVol.setSliderStyle (juce::Slider::LinearBarVertical);
  mSliderVol.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
  mLabelVol.attachToComponent(&mSliderVol, true);
  mLabelVol.setText("Volume", juce::dontSendNotification);
  
  //Add to the editor component
  addAndMakeVisible(mSliderVol);
  addAndMakeVisible(mWaveThumbnail);
  addAndMakeVisible(mADSR);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
  std::printf("In the editor");
    setSize (600, 800);
  
  
  mVolSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "GAIN", mSliderVol);
  
  
  
}

HelloSamplerAudioProcessorEditor::~HelloSamplerAudioProcessorEditor()
{
}

/*
void HelloSamplerAudioProcessorEditor::sliderValueChanged(juce::Slider *slider)
{
  if(slider == &mSliderAttack)
  {
    audioProcessor.getADSRParams().attack = mSliderAttack.getValue();
  }else if(slider == &mSliderDecay)
  {
    audioProcessor.getADSRParams().decay = mSliderDecay.getValue();
  }else if(slider == &mSliderSustain)
  {
    audioProcessor.getADSRParams().sustain = mSliderSustain.getValue();
  }else if(slider == &mSliderRelease)
  {
    audioProcessor.getADSRParams().release = mSliderRelease.getValue();
  }
  audioProcessor.updateADSR();
}*/

//==============================================================================
void HelloSamplerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (Colours::black);
}


void HelloSamplerAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
  
  mWaveThumbnail.setBoundsRelative(0.0f, 0.1f, 1.0f, 0.2f);
  mADSR.setBoundsRelative(0.0f, 0.75f, 1.0f, 0.25f);
  
  //(float x, float y, float w, float h)
  mSliderVol.setBoundsRelative(0.5f, 0.4f, 0.05f, 0.3f);


}



/*void HelloSamplerAudioProcessorEditor::sliderValueChanged (juce::Slider* slider)
{
    audioProcessor.noteVol = mSliderVol.getValue();
  //DBG(audioProcessor.noteVol);
  //std::printf("%f",audioProcessor.noteVol);
}

*/
