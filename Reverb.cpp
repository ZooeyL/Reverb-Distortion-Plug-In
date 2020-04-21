#include "../JuceLibraryCode/JuceHeader.h"
#include "Reverb.h"


Rev::Rev(RevdistAudioProcessor& p) : processor(p)
{
  // passing audio processor object by reference   

  // set color option of knob usinig LookAndFeel
  powderblueOption.setColour(Slider::thumbColourId, Colours::powderblue);

  // Adding reverb parameters as knobs
  revRoomSizeKnob.setLookAndFeel(&powderblueOption);
  revRoomSizeLabel.setFont(10.0f);
  revRoomSizeLabel.setText("Room Size", dontSendNotification);
  revRoomSizeLabel.setJustificationType(Justification::centred);
  revRoomSizeLabel.attachToComponent(&revRoomSizeKnob, false);
  addAndMakeVisible(&revRoomSizeKnob);  // allows component to be visible 

  revDampingKnob.setLookAndFeel(&powderblueOption); 
  revDampingLabel.setFont(10.0f);
  revDampingLabel.setText("Damping", dontSendNotification);
  revDampingLabel.setJustificationType(Justification::centred);
  revDampingLabel.attachToComponent(&revDampingKnob, false);
  addAndMakeVisible(&revDampingKnob);  

  revDryKnob.setLookAndFeel(&powderblueOption); 
  revDryLabel.setFont(10.0f);
  revDryLabel.setText("Dry Level", dontSendNotification);
  revDryLabel.setJustificationType(Justification::centred);
  revDryLabel.attachToComponent(&revDryKnob, false);
  addAndMakeVisible(&revDryKnob);  

  revWetKnob.setLookAndFeel(&powderblueOption); 
  revWetLabel.setFont(10.0f);
  revWetLabel.setText("Wet Level", dontSendNotification);
  revWetLabel.setJustificationType(Justification::centred);
  revWetLabel.attachToComponent(&revWetKnob, false);
  addAndMakeVisible(&revWetKnob);  

  revWidthKnob.setLookAndFeel(&powderblueOption); 
  revWidthLabel.setFont(10.0f);
  revWidthLabel.setText("Width", dontSendNotification);
  revWidthLabel.setJustificationType(Justification::centred);
  revWidthLabel.attachToComponent(&revWidthKnob, false);
  addAndMakeVisible(&revWidthKnob);  

  revFreezeKnob.setLookAndFeel(&powderblueOption); 
  revFreezeLabel.setFont(10.0f);
  revFreezeLabel.setText("Freeze", dontSendNotification);
  revFreezeLabel.setJustificationType(Justification::centred);
  revFreezeLabel.attachToComponent(&revFreezeKnob, false);
  addAndMakeVisible(&revFreezeKnob);  
}

Rev::~Rev() 
{
     
}

void Rev::paint(Graphics& g)
{
  g.fillAll (Colours::slategrey);
  g.setColour(Colours::white);
}

void Rev::resized()
{
      auto area = getLocalBounds(); 

      // to ensure it's consistent and not subject to local bound changing 
      // this is calculated using original area, before it gets modified 
      auto width = (area.getWidth())/2;  
      auto height = (area.getHeight())/3; 
      
      // 1st row; no setBounds() applied yet, this is just marking bounds
      auto top1 = area.removeFromTop(height); 
      // removeFromTop returns upper 1/3 rectangle
      // returns bottom 2/3 modified rect as 'area'

      auto top2 = area.removeFromTop(height); // 2nd row

      auto top3 = area.removeFromTop(height); 
      // 3rd rect; it should be different from top1 and top2 ideally
      // I initially set top3 = getLocalBounds() as that should have be space of the bottom 3rd row. Doesn't work however. 

      revRoomSizeKnob.setBounds(top1.removeFromLeft(width).withSizeKeepingCentre(85, 85));
      
      revDryKnob.setBounds(top2.removeFromLeft(width).withSizeKeepingCentre(85,85)); 

      revWidthKnob.setBounds(top3.removeFromLeft(width).withSizeKeepingCentre(85,85)); 

      revDampingKnob.setBounds(top1.removeFromLeft(width).withSizeKeepingCentre(85,85));

      revWetKnob.setBounds(top2.removeFromLeft(width).withSizeKeepingCentre(85, 85)); 

      revFreezeKnob.setBounds(top3.removeFromLeft(width).withSizeKeepingCentre(85,85)); 
}