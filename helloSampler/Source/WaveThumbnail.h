/*
  ==============================================================================

    WaveThumbnail.h
    Created: 2 Aug 2022 10:53:57am
    Author:  Martin Penberthy

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class WaveThumbnail  : public juce::Component,
                       public FileDragAndDropTarget
{
public:
    WaveThumbnail(HelloSamplerAudioProcessor&);
    ~WaveThumbnail() override;

    void paint (juce::Graphics&) override;
    void resized() override;
  bool isInterestedInFileDrag(const StringArray &files) override;
  void filesDropped(const StringArray &files, int x, int y) override;

private:
  
  std::vector<float> mAudioPoints;//To save the scaled audio data in to be displayed
  String mFileName {""};
  
  HelloSamplerAudioProcessor &processor;  
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveThumbnail)
};
