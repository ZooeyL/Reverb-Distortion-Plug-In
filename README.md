# Reverb-Distortion-Plug-In
C++ JUCE plug-in 

Distortion side has 3 effects:
- addition of white noise, generated by Box-Muller transform
- lo-fi bit crusher that lowers bit depth
- sample holding rate, a type of quantization effect 

Reverb is made with JUCE's reverb library which comes with its own dry/wet knob, room size, freeze, width, etc. 