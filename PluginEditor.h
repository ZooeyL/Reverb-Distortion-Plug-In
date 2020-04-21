/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "Reverb.h"    
#include "Distortion.h"

//============================================================================
/*
The editor doesn't really do much but set the big GUI rectangle - a.k.a main component. All the value tree attachments go in your distortion header file. 
*/
class RevdistAudioProcessorEditor : public AudioProcessorEditor
{
public:
    RevdistAudioProcessorEditor (RevdistAudioProcessor&); // passing processor reference type
    ~RevdistAudioProcessorEditor();

    //=========================================================================
    void paint (Graphics&) override;
    void resized() override;

    //std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> sliderAttachment; 

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    // You will copy this a lot so that each plugin effect creates a processor object
    RevdistAudioProcessor& processor; 

    // each class/type object will occupy own sub-rectangle space 
    Distortion distGui; 
    Rev revGui; 

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RevdistAudioProcessorEditor)
};

// line 37, why do we need to create a reference object at all? Why not just pass reference if we want to change how processor object behaves? 



/*
(a) include header files in everything 
(b) need to go into distortion.h and reverb.h filesto set inheritance, public & private scope declaration. They will inherit the component class. 
(b) in this case, we do not put everything in the editor files but separate it so that one plugin will have its own constructor w/ reference to processor (handles menu choices and apvts). Then it will have destructor and the two pure virtual functions paint & resized. 
(c) 
*/ 