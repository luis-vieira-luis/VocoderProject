/*
  ==============================================================================

    SynthVoice.h
    Created: 15 Apr 2020 11:52:43am
    Author:  Damien Ronssin

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "SynthSound.h"
#include "maximilian.h"
#include <iostream>

class SynthVoice : public SynthesiserVoice
{
public:
    bool canPlaySound (SynthesiserSound* sound) override
    {
        return dynamic_cast <SynthSound*>(sound) != nullptr;
    }
    
    void startNote (int midiNoteNumber, float velocity, SynthesiserSound *sound, int currentPitchWheelPosition) override
    {
        env1.trigger = 1;

        // Todo: deal with velocity in a better way
        level = 0.2;

        frequency = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    }
    
    void stopNote (float velocity, bool allowTailOff) override
    {
        env1.trigger = 0;
        allowTailOff = true;

        if (velocity == 0)
            clearCurrentNote();
    }
    
    void pitchWheelMoved (int newPitchWheelValue) override
    {
        
    }
    
    void controllerMoved (int controllerNumber, int newControllerValue) override
    {
        
    }
    
    void renderNextBlock (AudioBuffer< float > &outputBuffer, int startSample, int numSamples) override
    {
        env1.setAttack(200);
        env1.setDecay(1000);
        env1.setSustain(0.9);
        env1.setRelease(600);
        
        for (int sample = 0; sample < numSamples; sample++)
        {
            double theWave = osc1.saw(frequency);
            double theSound = env1.adsr(theWave, env1.trigger) * level;

            
            for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
            {
                outputBuffer.addSample(channel, sample, theSound);
            }
            ++startSample;
        }
        
        
        
    }
    
private:
    double level;
    double frequency;
    
    maxiOsc  osc1;
    maxiEnv env1;
};

