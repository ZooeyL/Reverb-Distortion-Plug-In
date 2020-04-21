/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
RevdistAudioProcessorEditor::RevdistAudioProcessorEditor (RevdistAudioProcessor& p)
 : AudioProcessorEditor (&p), processor (p), distGui (p), revGui (p)
{
    // initialize distGui and revGui variables with parameter p 
    setSize (400, 400); // should be bigger than component in distortion or reverb
    addAndMakeVisible(&distGui);
    addAndMakeVisible(&revGui); 

}

RevdistAudioProcessorEditor::~RevdistAudioProcessorEditor()
{
}

//==============================================================================
void RevdistAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(Colours::white); 
}

void RevdistAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto area = getLocalBounds(); // returns rectangle we subdivide 
    
    distGui.setBounds(area.removeFromLeft( getWidth() / 2 )); 
    revGui.setBounds(area.removeFromLeft( getWidth()/2) );
    /*
    We take a rectangle which represents whole component; it has same height (y-coordinate) but we will give it 1/2 the width of big rectangle. You could manually subtract it (e.g int componentWidth = 300 pixels)
    
    Think of position based on the upper left corner. 
    (0,0) 
       |
       |
       |__________ (3,3)
    */
}



