#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h" 

class Distortion : public Component
{
public:
    Distortion(RevdistAudioProcessor&); 
    // constructor will set input parameter as reference type
    ~Distortion();
    void paint(Graphics&) override;
    void resized() override;

private:
    RevdistAudioProcessor& processor; // declared first to use apvts object
    LookAndFeel_V4 orangeOption; // component customization class
    LookAndFeel_V4 whiteOption; 
    

    Slider distNoiseKnob; 
    Label distNoiseLabel; 
    Slider distCrushKnob{ Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow };
    Label distCrushLabel; 
    Slider distRateKnob{ Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow }; 
    Label distRateLabel; 

    // apvts attachment creations 
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> noiseVal; // old method, but worth trying 

    AudioProcessorValueTreeState::SliderAttachment crushVal {processor.apvts, "CRUSHSLIDER", distCrushKnob};
    AudioProcessorValueTreeState::SliderAttachment rateVal { processor.apvts, "RATESLIDER", distRateKnob}; 
    // necessary to add; it will access processor.cpp file and update its changed GUI values 



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Distortion)
    /*
    // unique ptr (replaces ScopedPointer); placed after sliders to get destroyed first. But even this is outdated method compared to what you see with CrushKnob. 
    */
};

/*

Original code is:

std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> noiseVal; // dist h file
noiseVal = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.apvts, "NOISESLIDER", distNoiseKnob);  // dist cpp file 

It is preferred to have members rather than pointers, even though smart pointers are already a step in the right direction. The arguments in the {} are passed to the constructor, if not overriden in the constructor implementations initaliser list. 

Slider::Slider (SliderStyle          style,
                TextEntryBoxPosition textBoxPosition)  

// example of different ways of allocation                  
private:
    int foo = 10;
    int bar { 10 };

// that was with basic types. You can do it with both pointers and classes too:

private:
Slider distNoiseKnob { Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow };

// Why> Beause "arguments in the {} are passed to the constructor, if not overridden in the constructor implementations initiaiser list"

Notice you don't need to call the entire Slider::SliderStyle::RotaryHorizontaVerticalDrag since SliderStyle is an enum, not an enum class, it is enough to call the Slider::namespace, no need for the enum name. 

Just adding the 2 parameters you sent in extra calls to the constructor. And final step, you can reference members, that appeared before: 

private:
    MyProcessor& processor;  // will be set in the cpp from the supplied argument
    Slider distNoiseKnob { Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow };

    AudioProcessorValueTreeState::SliderAttachment noiseVal {processor.apvts, "NOISESLIDER", distNoiseKnob};

// now you only need to call addAndMakeVisible on your slider. And that method understands & and * (there are two overloaded implementations). SetRange and SetValue are unnecessary (apvts overrides it), so just worry about slider type and text box, which can just be set in one line. 

*/ 

/*
Mix is a one-liner that can sound good and make processing more subtle. 

a + p(b-a)
a = dry signal
b = wet signal
p = mix knob (0 to 1)

According to daniel: a mix is the obvious choice to control, if you want to control
the amount of your effect. Unfortunately that is not what is happening, but the clean
input signal is crossfaded with the signal, that has the effect applied (wet). And for 
many effects that is abad idea, because that exposes artifacts from processing.
E.g. if your effect introduced latency, it is crucial to apply the same latency 
to the clean signal (delay). If your latency is frequency dependent like for many 
equalisers, you create a mess. But then, there are users who love a mess, why else
was the bitcrusher invented? :) TL:DR: IMHO the mix is a bad idea in most cases,
but if users want it, they shall have it. 
*/




/*
Traditionally 'new' allocates memory on heap and 'delete' deletes memory to free it. Smart pointers do this automatically, like unique_ptr, when the unique_ptr object goes out of scope. 

- unique_ptr is a class template
- it is one of the 'smart' pointer provided by c++11 to prevent memory leaks 
- unique ptr wraps a RAW pointer in it, and de-allocates the raw pointer, when unique ptr object goes out of scope 

Class Entity
{
public:
    Entity() // constructor 
    {
        std::cout << "Created Entity!" << std::endl; 
    }
}
// std::unique_ptr<Entity> entity = new Entity(); 
 --> produces error since the unique_ptr's constructor is set to keyword 'explicit' 
// std::unique_ptr<Entity> entity(new Entity());   
--> entity is name given, Entity is template argument, notice no equal sign
// std::unique_ptr<Entity> entity = std::make_unique<Entity>() 
--> the preferred way, supposedly, due to EXCEPTION safety. 
*/ 
