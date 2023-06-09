/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 18 Feb 2022 6:37:04pm
    Author:  Narmeen

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class DJAudioPlayer : public juce::AudioSource {
    public: 
        DJAudioPlayer(juce::AudioFormatManager& _formatManager);
        ~DJAudioPlayer();

        //==============================================================================
        void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
        void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
        void releaseResources() override;

        void loadURL(juce::URL audioURL);
        void setGain(double gain);
        void setSpeed(double ratio);
        void setPosition(double posInSecs);
        void setPositionRelative(double pos);


        void start();
        void stop();

        /**get relative position of playhead*/
        double getPositionRelative();

    private:
        juce::AudioFormatManager& formatManager;
        std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
        juce::AudioTransportSource transportSource;
        juce::ResamplingAudioSource resampleSource{&transportSource, false, 2};
 
};