#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h" 

class Rev : public Component
{
public:
    Rev(RevdistAudioProcessor&); // a constructor sets input parameter as reference type
    ~Rev();
    void paint(Graphics&) override;
    void resized() override;


private:
    RevdistAudioProcessor& processor; // declared first to use apvts object declaration 
    LookAndFeel_V4 powderblueOption; // component customization class

    Slider revRoomSizeKnob{ Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow };
    Slider revDampingKnob{ Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow };
    Slider revWetKnob{ Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow };
    Slider revDryKnob{ Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow };
    Slider revWidthKnob{ Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow };
    Slider revFreezeKnob{ Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow };

    // Labels
    Label revRoomSizeLabel; 
    Label revDampingLabel;
    Label revWetLabel;
    Label revDryLabel;
    Label revWidthLabel;
    Label revFreezeLabel; 

    // Creating apvt attachments
    AudioProcessorValueTreeState::SliderAttachment SizeVal{processor.apvts, "ROOMSIZESLIDER", revRoomSizeKnob};
    AudioProcessorValueTreeState::SliderAttachment DampingVal{processor.apvts, "DAMPINGSLIDER", revDampingKnob};   
    AudioProcessorValueTreeState::SliderAttachment WetVal{processor.apvts, "WETSLIDER", revWetKnob};
    AudioProcessorValueTreeState::SliderAttachment DryVal{processor.apvts, "DRYSLIDER", revDryKnob};
    AudioProcessorValueTreeState::SliderAttachment WidthVal{processor.apvts, "WIDTHSLIDER", revWidthKnob};
    AudioProcessorValueTreeState::SliderAttachment FreezeVal{processor.apvts, "FREEZESLIDER", revFreezeKnob};




    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Rev)
};