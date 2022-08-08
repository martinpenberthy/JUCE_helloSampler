/*
  ==============================================================================

    WaveThumbnail.cpp
    Created: 2 Aug 2022 10:53:57am
    Author:  Martin Penberthy

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveThumbnail.h"

//==============================================================================
WaveThumbnail::WaveThumbnail(HelloSamplerAudioProcessor& p) : processor(p)
{

}

WaveThumbnail::~WaveThumbnail()
{
}

/*
  Paint function for the WaveThumbnail component which displays the waveform
  given by the user
 */
void WaveThumbnail::paint (juce::Graphics& g)
{
  g.fillAll(Colours::cadetblue);
  
  //Get the waveform buffer from the audio proessor
  auto waveform = processor.getWaveform();
  //Check if file has been given
  if(waveform.getNumSamples() > 0)  
  {
    Path p;
    mAudioPoints.clear();//Clear buffer in case there is old data
    //Calculate scaling ratio
    auto xRatio = waveform.getNumSamples() / getWidth();
    //Read pointer for the waveform
    auto buffer = waveform.getReadPointer(0);
  
    //Scale audio file to window on x axis and gather needed data from the buffer
    for(int sample = 0; sample < waveform.getNumSamples(); sample += xRatio)
        mAudioPoints.push_back(buffer[sample]);
    
    //Setup waveform draw
    g.setColour(Colours::yellow);
    p.startNewSubPath(0, getHeight() / 2);
    
    //scale on y axis
    for(int sample = 0; sample < mAudioPoints.size(); ++sample)
    {
        auto point = jmap<float>(mAudioPoints[sample], -1.0f, 1.0f, getHeight(), 0);
        p.lineTo(sample, point);
    }
    //Set stroke for path
    g.strokePath(p, PathStrokeType(2));
    
    g.setColour(Colours::white);
    g.setFont(15.0f);
    auto textBounds = getLocalBounds().reduced(10, 10);
    
    g.drawFittedText(mFileName, textBounds, Justification::topRight, 1);
    
  }else
  {
    g.setColour(Colours::white);
    g.setFont(40.0f);
    g.drawFittedText("Drop an Audio File to Load", getLocalBounds(), Justification::centred, 1);
  }
  
}//End paint

void WaveThumbnail::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
}

/*
  Override for the isInterestedInFileDrag(const StringArray &files) function
  from FileDragAndDropTarget.
 const StringArray &files is an array of strings of file names
 */
bool WaveThumbnail::isInterestedInFileDrag(const StringArray &files)
{
    //Check if the file is an accepted format
    for(auto file : files)
      if(file.contains(".wav") || file.contains(".mp3") || file.contains(".aif"))
        return true;
    
    return false;
}


/*
  Override for the filesDropped(const StringArray &files, int x, int y) function
  from FileDragAndDropTarget.
 */
void WaveThumbnail::filesDropped(const StringArray &files, int x, int y)
{
    //Check if the dropped file is an accepted format using isInterestedInFileDrag
    for(auto file : files)
      if(isInterestedInFileDrag(files))
      {
        //Load the file and extract the name
        auto myFile = std::make_unique<File> (file);
        mFileName = myFile->getFileNameWithoutExtension();
        processor.loadFile(file);
      }
  //Make sure the new file gets displayedq
  repaint();
}


