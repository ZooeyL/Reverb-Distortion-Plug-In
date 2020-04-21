/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include<stdio.h>
#include<math.h>


//==============================================================================
RevdistAudioProcessor::RevdistAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                     ), apvts(*this, nullptr, "Parameters", createParameters())  
#endif
{
}

RevdistAudioProcessor::~RevdistAudioProcessor()
{
}












//==============================================================================
const String RevdistAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool RevdistAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool RevdistAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool RevdistAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double RevdistAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int RevdistAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int RevdistAudioProcessor::getCurrentProgram()
{
    return 0;
}

void RevdistAudioProcessor::setCurrentProgram (int index)
{
}

const String RevdistAudioProcessor::getProgramName (int index)
{
    return {};
}

void RevdistAudioProcessor::changeProgramName (int index, const String& newName)
{
}










//==============================================================================
void RevdistAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need.. 
    // reverb.setSampleRate(const double sampleRate);

    //The one in prepareToPlay is an upper limit for buffer size set by host (e.g DAW)
    // buffer.setSize(getTotalNumOutputChannels(), samplesPerBlock); 
    /*
    void AudioBuffer<Type>::setSize(int newNumChannels, int newNumSamples,
    bool keepExistingContent=false, bool clearExtraSpace=false, bool avoidReallocating=false)
    */
    reverb.setSampleRate(sampleRate);
}

void RevdistAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    /*

    */
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool RevdistAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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









void RevdistAudioProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.

    /* Keeping Sliders decibel based and using state members for SMOOTH changes
    https://docs.juce.com/master/tutorial_sine_synth.html
    https://docs.juce.com/master/tutorial_simple_synth_noise.html (good reminder of buffer tutorial)
    */

    // Processing =================================================
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());
    // clears all samples 
    // void AudioBuffer<Type>::clear( int channel, int startSample, int numSamples)

    int numSamples = buffer.getNumSamples(); // returns number of samples allocated in each buffer channels 
    // Initializations ============================================
    float noiseLeveldB = *apvts.getRawParameterValue("NOISESLIDER") - 100; 
    // we want max dB to be 0dB in daw, so we subtract 100 
    // slider by default outputs values in dB 
    auto noiseLevelGain = Decibels::decibelsToGain(noiseLeveldB);

    float z0 = 0;
    float z1 = 0;
    bool generate = false;

    float mu = 0; // center (0)
    float sigma = 1; // spread -1 <-> 1

    float output = 0;
    float u1 = 0;
    float u2 = 0;

    Random r = Random::getSystemRandom();
    r.setSeed(Time::getCurrentTime().getMilliseconds());
    const float epsilon = std::numeric_limits<float>::min();

    auto bitDepth = *apvts.getRawParameterValue("CRUSHSLIDER");
    int rateDivide = *apvts.getRawParameterValue("RATESLIDER");


    // // SAFETY CHECK :::: since some hosts will change buffer sizes without calling prepToPlay (Bitwig)
    // if (noiseBuffer.getNumSamples() != numSamples) 
    // {
    //     noiseBuffer.setSize(2, numSamples, false, true, true); // clears
    //     currentOutputBuffer.setSize(2, numSamples, false, true, true); // clearsjoker
    // }

    //   auto *write = noiseBuffer.getWritePointer(0);
    //   for (auto sample = 0; sample < noiseBuffer.getNumSamples(); ++sample)
    //     write[sample] = /* all your random noise maths from getWhiteNoise */;

    /*
    A code I looked at would create a noise array, add noise samples to it, and then add it to output buffer. Rather than doing this, I pass in my noise buffer and do the same noise creation in the already allocated array. 
    */

    /*And it is probably worth pointing out, that getReadPointer (0) and
    getWritePointer (0) return the same address. The only difference is, that
    getReadPointer() returns a const pointer, so you can only
    read, vs. getWritePointer() allows to write to that address.*/

    /* tricky to see the difference between
    buffer[sample] (audio channel struct, looks at buffers as pointers)
    vs buffer.addSample (buffer class)
    // buffer[sample] = val - remainder;  vs  buffer.addSample(channel, sample, (val - remainder));
    */
 
    // NOISE Implementation ====================================================================== 
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            // using box muller method
            // https://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform
            generate = !generate;

            if (!generate)
                output = (z1 * sigma + mu) * noiseLevelGain;
            else
            {
                do
                {
                    u1 = r.nextFloat();
                    u2 = r.nextFloat();
                } while (u1 <= epsilon);

                z0 = sqrtf(-2.0 * logf(u1)) * cosf(2 * float(double_Pi) * u2);
                z1 = sqrtf(-2.0 * logf(u1)) * sinf(2 * float(double_Pi) * u2);

                output = (z0 * sigma + mu) * noiseLevelGain;
            }

            // NAN check ...
            jassert(output == output);
            jassert(output > -50 && output < 50);

            // Add Box Muller generated noise 
            buffer.addSample(channel, sample, output);
            // before I was doing buffer[sample] and trying getWritePointer...
        }
    }

    // SAMPLE AND BIT MANIPULATION
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            // REDUCE BIT DEPTH :::::
            float totalQLevels = powf(2, bitDepth);
            float val = buffer.getSample(channel, sample);
            float remainder = fmodf(val, 1 / totalQLevels);

            buffer.setSample(channel, sample, (val - remainder)); // quantization levels set

            // SAMPLE HOLDING RATE
            if (rateDivide > 1)
            {
                if (sample % rateDivide != 0) // if remainder =/= 0 
                {
                    buffer.setSample(channel, sample, buffer.getSample(channel, sample - (sample % rateDivide)));
                    // if rateDivide = 2, it means every odd index position will replace 
                    // its value at position (e.g 3) with value at previous position 3-1 = 2 (always 1 position before). 
                    // It gets more complicated as the rateDivide increases. Essentially same values will be held over 
                    // multiple positions. 

                    // error: buffer.setSample(channel, sample, (sample - (sample % rateDivide)));
                    // original: if (i%rateDivide != 0) data[i] = data[i - i%rateDivide];
                    // reason you get error is you did not use sample's value, you used sample index
                    // correct: buffer.getSample(sample - (sample % rateDivide)  
                    // what you did: data[i] = i - i%rateDivide essentially 
                    // ratedivide is trying to change sample index position, not the value at sample 
                }
            }
        }
    }

    // REVERB Implementation ==================================================================
    // reverbParams is instantiated from struct
    reverbParams.roomSize = *apvts.getRawParameterValue("ROOMSIZESLIDER");
    reverbParams.damping = *apvts.getRawParameterValue("DAMPINGSLIDER");
    reverbParams.wetLevel = *apvts.getRawParameterValue("WETSLIDER");
    reverbParams.dryLevel = *apvts.getRawParameterValue("DRYSLIDER");
    reverbParams.width = *apvts.getRawParameterValue("WIDTHSLIDER");
    reverbParams.freezeMode = *apvts.getRawParameterValue("FREEZESLIDER");
    reverb.setParameters(reverbParams);

    reverb.processStereo(buffer.getWritePointer(0), buffer.getWritePointer(1), buffer.getNumSamples() ); 
}

// void RevdistAudioProcessor::fillWhiteNoise (AudioBuffer<float>& noiseBuffer)
// {
//   auto *write = noiseBuffer.getWritePointer(0);
//   for (auto sample = 0; sample < noiseBuffer.getNumSamples(); ++sample)
//     write[sample] = /* all your random noise maths from getWhiteNoise */;
// } 

// void RevdistAudioProcessor::addNoise(int noiseLevel, AudioBuffer<float>& buffer)
// {
//     noise = getWhiteNoise(numSamples);  // noise buffer
//     noiseAmt = jlimit<float>(0, 1, noiseLevel); // set range

//     // stereo addition of noise 
//     FloatVectorOperations::add(noiseBuffer.getWritePointer(0), noise.getRawDataPointer(), numSamples); 

// }




















//==============================================================================
bool RevdistAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
    /* 
    So RevdistAudioProcessor already inherited from Processor class, which gives access to this key function
    */
}

AudioProcessorEditor* RevdistAudioProcessor::createEditor()
{
    return new RevdistAudioProcessorEditor (*this);
}

//==============================================================================
void RevdistAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void RevdistAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}





//================================================================
AudioProcessorValueTreeState::ParameterLayout RevdistAudioProcessor::createParameters()
// using scope resolution to give ParameterLayout class type  
{
    std::vector< std::unique_ptr<RangedAudioParameter> > parameters; 
    /*
    RangedAudioParameter is inherited by AudioParameterFloat, AudioParameterChoice, etc. 
    */

    // DISTORTION PARAMETERS: identifier within program, name used by DAW, range + default
    parameters.push_back (std::make_unique<AudioParameterFloat> ("NOISESLIDER", "white noise", 0.0f, 100.0f, 0.0f));
    parameters.push_back (std::make_unique<AudioParameterFloat>("CRUSHSLIDER", "bit crush", 0.0f, 32.0f, 32.0f)); 
    parameters.push_back (std::make_unique<AudioParameterInt> ("RATESLIDER", "index modulus rate", 1, 50, 0)); 
    
    // REVERB PARAMETERS
    parameters.push_back (std::make_unique<AudioParameterFloat> ("ROOMSIZESLIDER", "room size", 0.0f, 1.0f, 0.5f));
    parameters.push_back (std::make_unique<AudioParameterFloat>("DAMPINGSLIDER", "damping", 0.0f, 1.0f, 0.5f)); 
    parameters.push_back (std::make_unique<AudioParameterFloat> ("WETSLIDER", "wet", 0.0, 1.0, 0.33)); 
    parameters.push_back (std::make_unique<AudioParameterFloat> ("DRYSLIDER", "dry", 0.0f, 1.0f, 0.4f));
    parameters.push_back (std::make_unique<AudioParameterFloat> ("WIDTHSLIDER", "width", 0.0f, 1.0f, 1.0f));    
    parameters.push_back (std::make_unique<AudioParameterFloat> ("FREEZESLIDER", "freeze", 0.0f, 2.0f, 0.0f));

    // Return 
    return {parameters.begin(), parameters.end()}; 

    // RACKAFX implementation abandoned 
    // parameters.push_back (std::make_unique<AudioParameterFloat> ("PREDELAY", "Predelay", 0.0f, 100.0f, 40.0f));
    // parameters.push_back (std::make_unique<AudioParameterFloat> ("PREDELAYATN", "Predelayatn", -96.0f, 100.0f, 0.0f));
    // parameters.push_back (std::make_unique<AudioParameterFloat> ("BANDWIDTH", "Bandwidth", 0.0f, 1.0f, 0.45f));

    // // Input Diffusion
    // parameters.push_back (std::make_unique<AudioParameterFloat> ("APF1DELAY", "Apfdelay1", 0.0f, 100.0f, 13.28f));
    // parameters.push_back (std::make_unique<AudioParameterFloat> ("APF1g", "Apfg1", -1.0f, 1.0f, 0.7f));
    // parameters.push_back (std::make_unique<AudioParameterFloat> ("PREDELAY", "Predelay", 0.0f, 100.0f, 40.0f));
    // parameters.push_back (std::make_unique<AudioParameterFloat> ("PREDELAY", "Predelay", 0.0f, 100.0f, 40.0f));


    /*
    std::vector::push_back -> adds a new element at the end of the vector, after its current last element. 
    The first parameter (e.g "PREDELAY") is 
    The second parameter (e.g "Predelay") is what shows up in your DAW as parameter name. 
    */
} 

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new RevdistAudioProcessor();
}
