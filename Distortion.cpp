
#include "../JuceLibraryCode/JuceHeader.h"
#include "Distortion.h"

  // Declare processor(p) in both editor.h and distorion.h. Probably because we are using scope resolution on the class Distortion and RevdistAudioProcessorEditor, and the private variable needs to exist within the scope of each class. 'processor' kept private in h file so it can only be accessed in this and its cpp file. 

// Constructor & Destructor =====================================================
Distortion::Distortion(RevdistAudioProcessor& p) : processor(p)  
{
  // Knob color options 
  whiteOption.setColour(Slider::thumbColourId, Colours::white);
  orangeOption.setColour(Slider::thumbColourId, Colours::orange);

  // Adding noise knob child component 
  distNoiseKnob.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag); 
  distNoiseKnob.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 20);
  distNoiseKnob.setLookAndFeel(&whiteOption); // set slider to white 
  addAndMakeVisible(&distNoiseKnob);  // vallows component to be visible 

  // Label class for noise knob
  distNoiseLabel.setFont (10.0f); 
  distNoiseLabel.setText ("White Noise", dontSendNotification); 
  distNoiseLabel.setJustificationType (Justification::centred); 
  distNoiseLabel.attachToComponent(&distNoiseKnob, false); // lets us avoid manual placement of text in resized()

  // Prepare an attachment 
  noiseVal = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.apvts, "NOISESLIDER", distNoiseKnob); 
  // this method is outdated but I wanted to try it for just noise slider 
  // slider id is NOISESLIDER, slider object to control is distNoiseKnob
  // these two things are connected now through audioprocessorvaluetreestate
  // std::make_unique<T> creates an isntance of std::unique_ptr<T> 
  // once we set 'noiseVal', we don't really use it anywhere else; you just use it to connect. 

  // Adding crush knob child component
  distCrushKnob.setLookAndFeel(&orangeOption);
  addAndMakeVisible(&distCrushKnob);
  // Label class for crush knob
  distCrushLabel.setFont(10.0f);
  distCrushLabel.setText("Lo-fi Crush", dontSendNotification);
  distCrushLabel.setJustificationType(Justification::centred);
  distCrushLabel.attachToComponent(&distCrushKnob, false);

  // Adding rate knob child component 
  distRateKnob.setLookAndFeel(&orangeOption);
  addAndMakeVisible(&distRateKnob);
  // Label class for rate knob
  distRateLabel.setFont(10.0f);
  distRateLabel.setText("Index Modulus Rate", dontSendNotification);
  distRateLabel.setJustificationType(Justification::centred);
  distRateLabel.attachToComponent(&distRateKnob, false);

  
  /*
  The apvts class has several component sub classes, like AudioProcessorValueTreeState::SliderAttachment class. 
  std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(AudioProcessorValueTreeState &stateToControl, const String &parameterID, Slider &sliderToControl); 

  processor.apvts -> processor is reference to class RevdistAudioProcessor and apvts is an AudioProcessorValueTreeState object instantiated in processor.h

  "NOISE SLIDER" is something we created first in the ParameterLayout class from processor.cpp. 
  */ 

  /*
  - you can't see knob without addAndMakeVisible
  - void Component::addAndMakeVisible (Component * child, int zORDER=-1)
  */


  // waveSelection = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment>(processor.tree, "wavetype", oscMenu); 

  //setSize(150,150);  
  /*
  JUCE says 'Make sure that before the constructor has finished, you've set the editor's size to whatever you need it to be'. Probably don't need it here in the sub components, just the main component (editor files). 
  */
}

Distortion::~Distortion()
{
}

// Background, Positioning ========================================================= 
void Distortion::paint (Graphics& g)
{
  g.fillAll (Colours::slategrey);
  g.setColour(Colours::white);
}

void Distortion::resized() 
{
  auto area = getLocalBounds();

  distNoiseKnob.setBounds(area.removeFromTop(getHeight()/2).withSizeKeepingCentre(100, 100));
  
  distCrushKnob.setBounds(area.removeFromLeft(getWidth()/2).withSize(100, 100)); 
  // now positioned at bottom since top is taken by noise knob
  distRateKnob.setBounds(area.removeFromLeft(getWidth()/2).withSize(100, 100)); 

  /*
  Imagine removefromLeft and removefromtop as an arrow that starts from the 
  upper left corner of main screen, then moves to the right then moves to bottom. 
  While it's doing so it's like when we click a mouse on latop it makesa transparent rectangle. 
  
  Any space already occupied (like header or footer) means that setSize() will work with whatever space you have left. So this must all be done sequentially, meaning header and footer must be set before the sidebar like in the 'advanced gui layout' tutorial. 
  */
} 

// Steps to make:  ===========================================
// make pushback functions in cpp first ion createParameters() 
// declare unique pointers in plugin.h 
// write connection (e.g noiseVal) inside plugin.cpp file 

// using namespace std; 
// #include "PluginProcessor.h"   not necessary for this plugin 
// #include "PluginEditor.h"

/* INITIALIZATION LIST 
``` Distortion::Distortion(RevdistAudioProcessor& p) : processor(p)  ```
That's not inheritance that's an initializer list. It's passing parameter 'p' of reference type RevdistAudioProcessor. The 'p' itself is not important; the 'processor' is just a storage variable. Inheritance is only defined in the class definition itself. The purpose is to initialize the data members of a class. Initalization list is common when defining constructors. It's preferred to indent it, so it doesn't look like inheritance. 

Whatever is in the () or {} is the initialized value. 

class Point{
private: 
  int x;
  int y;
public: 
  Point(int i=0, int j=0) : x(i), y(j) {}
}

Same thing is if you write:
  Point (int i = 0, int j = 0) {
    x = i;
    y = j; 
  }
}

However, initializer list is sometimes necessary when members are (a) non-static constant (b) reference members (like our processor), (c) members which do not have default constructor, (d) base class members

It is actually prefered to use processor{p} over processor(p) because this uniform initilization takes into account size differences between different types. 

Both reference and const variables have this property in common that they both must be initialized with declaration. 
*/

  // juce::Rectangle<int> titleNoise(0, 40, getWidth(), 10);
  // g.drawText("White Noise", titleNoise, Justification::centredBottom);

  // /*teal rectangle */
  // juce::Rectangle <float> area( (getWidth()/2)- (getWidth()/4), getHeight()/2 , 90, 90); // positioned to rect's upper left corner
  // g.setColour(Colours::aquamarine);
  // g.setOpacity(0.6);
  // g.drawRoundedRectangle(area, 40.0f, 2.0f);



