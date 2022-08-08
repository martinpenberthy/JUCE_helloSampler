/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

using namespace juce;
//==============================================================================
HelloSamplerAudioProcessor::HelloSamplerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "Parameters", createParameters() )//Constructor for apvts
#endif
{
    //Register formats
    mFormatManager.registerBasicFormats();
    //Add AudioParameterValueTreeState to the processor object
    apvts.state.addListener(this);
  
    //Add correct number of voices
    for(int i = 0; i < mNumVoices; i++)
      mSampler.addVoice(new SamplerVoice());
}

HelloSamplerAudioProcessor::~HelloSamplerAudioProcessor()
{
    mFormatReader = nullptr;
}

//==============================================================================
const juce::String HelloSamplerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool HelloSamplerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool HelloSamplerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool HelloSamplerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double HelloSamplerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int HelloSamplerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int HelloSamplerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void HelloSamplerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String HelloSamplerAudioProcessor::getProgramName (int index)
{
    return {};
}

void HelloSamplerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void HelloSamplerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
  //Set the sample rate
  mSampler.setCurrentPlaybackSampleRate(sampleRate);
  //Init ADSR
  updateADSR();
}

void HelloSamplerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool HelloSamplerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void HelloSamplerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
  
    //Get gain value from the slider
    auto gainValue = apvts.getRawParameterValue("GAIN")->load();
    //Clear buffer
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
  
    //Render buffer for all voices
    mSampler.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
  
    //Scale samples by gain value
    for (auto c = totalNumInputChannels; c < totalNumOutputChannels; ++c)
      for(auto i = 0; i < buffer.getNumSamples(); ++i)
        buffer.setSample(c, i, buffer.getSample(c, i) * gainValue);
  
    //Update ADSR if it has changed
    if(mShouldUpdate)
      updateADSR();
}

//==============================================================================
bool HelloSamplerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* HelloSamplerAudioProcessor::createEditor()
{
    return new HelloSamplerAudioProcessorEditor (*this);
}

//==============================================================================
void HelloSamplerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void HelloSamplerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//Method for loading a file
/*void HelloSamplerAudioProcessor::loadFile()
{
    mSampler.clearSounds();
  //Make file chooser object //MP
    FileChooser chooser { "Please load a file" };
    
    if(chooser.browseForFileToOpen())
    {
        auto file = chooser.getResult();
        mFormatReader = mFormatManager.createReaderFor(file);
    }
  
    BigInteger range;
    range.setRange(0, 128, true);
    
    mSampler.addSound(new SamplerSound("Sample", *mFormatReader, range, 60, 0.05f, 0.05f, 10.0f));

}*/

//Method for loading file via drag and drop 
void HelloSamplerAudioProcessor::loadFile(const String &path)
{
    //Set up for adding SamplerSound
    mSampler.clearSounds();
    auto file = File (path);
    mFormatReader = mFormatManager.createReaderFor(file);
  
    auto sampleLength = static_cast<int>(mFormatReader->lengthInSamples);
  
    mWaveform.setSize(1, sampleLength);
    mFormatReader->read(&mWaveform, 0, sampleLength, 0, true, false);
    
    BigInteger range;
    range.setRange(0, 128, true);
    //Add sound from the Format Reader
    mSampler.addSound(new SamplerSound("Sample", *mFormatReader, range, 60, 0.05f, 0.05f, 10.0f));

    //Initialize the ADSR values
    updateADSR();
}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new HelloSamplerAudioProcessor();
}


/*
  This function is called in the initializer list for the HelloSamplerAudioProcessor constructor
  as a parameter for the AudioParameterValueTreeState and sets up the parameters needed
  for this plugin.
 */
AudioProcessorValueTreeState::ParameterLayout HelloSamplerAudioProcessor::createParameters()
{
  //Make a vector of RangedAudioParameters to store all the parameters in
  std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
  //Add a float parameter for the Gain value
  params.push_back(std::make_unique<juce::AudioParameterFloat>("GAIN", "Gain", 0.0f, 1.0f, 0.25f));
  
  //Add AudioParameterFloat parameters for ADSR values
  params.push_back(std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack", 0.0f, 5.0f, 0.01f));
  params.push_back(std::make_unique<juce::AudioParameterFloat>("DECAY", "Decay",0.0f, 5.0f, 0.01f));
  params.push_back(std::make_unique<juce::AudioParameterFloat>("SUSTAIN", "Sustain",0.0f, 1.0f, 1.0f));
  params.push_back(std::make_unique<juce::AudioParameterFloat>("RELEASE", "Release", 0.0f, 5.0f, 0.01f));

  //Return ParameterLayout
  return {params.begin(), params.end()};
}


/*
  This function gets the ADSR values from the AudioParameterValueTreeState and sets
  the corresponding membere of the ADSR::Parameters mADSRParams object. Then it sets
  the envelope parameters for each SamplerSound in the mSampler Synthesizer object
 */
void HelloSamplerAudioProcessor::updateADSR()
{
  //Get values and set them
  mADSRParams.attack = apvts.getRawParameterValue("ATTACK")->load();
  mADSRParams.decay = apvts.getRawParameterValue("DECAY")->load();
  mADSRParams.sustain = apvts.getRawParameterValue("SUSTAIN")->load();
  mADSRParams.release = apvts.getRawParameterValue("RELEASE")->load();

  //Set evnvelope parameters for each SamplerSound in the Synthesizer objects
  for(int i = 0; i < mSampler.getNumSounds(); ++i)
    if(auto sound = dynamic_cast<SamplerSound*>(mSampler.getSound(i).get()))
      sound->setEnvelopeParameters(mADSRParams);
}

/*
  Override of the valueTreePropertyChanged function. Similar to the valueChanged
  function used with Sliders and SliderListeners. If the value has changed, the
  flag is set which then causes the ADSR values to be updated from the processBlock
  function
 */
void HelloSamplerAudioProcessor::valueTreePropertyChanged (ValueTree &treeWhosePropertyHasChanged, const Identifier &property)
{
    mShouldUpdate = true;
}
