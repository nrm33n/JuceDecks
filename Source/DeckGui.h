/*
  ==============================================================================

    DeckGui.h
    Created: 19 Feb 2022 11:11:20am
    Author:  Narmeen


    sources 
    play button: https://www.flaticon.com/premium-icon/play_3318660?term=play&page=1&position=11&page=1&position=11&related_id=3318660&origin=search
    pause button: https://www.flaticon.com/premium-icon/pause-button_3249396?term=pause&page=1&position=7&page=1&position=7&related_id=3249396&origin=search
    rewind button: https://www.flaticon.com/premium-icon/rewind_4211391?term=rewind&page=1&position=13&page=1&position=13&related_id=4211391&origin=search
    fast forward button: https://www.flaticon.com/premium-icon/fast-forward_4211386?term=forward&page=1&position=4&page=1&position=4&related_id=4211386&origin=search


  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"

//==============================================================================
/*
*/
class DeckGui  : public juce::Component,
                 public juce::Button::Listener,
                 public juce::Slider::Listener,
                 public juce::FileDragAndDropTarget,
                 public juce::Timer
{
public:
    DeckGui(DJAudioPlayer* _player,
            juce::AudioFormatManager& formatManagerToUse,
            juce::AudioThumbnailCache& cacheToUse);

    ~DeckGui() override;

    void paint(juce::Graphics&) override;

    void resized() override;

    /** implement button listener*/
    void buttonClicked(juce::Button*) override;

    /** implement Slider::listener*/
    void sliderValueChanged(juce::Slider* slider) override;

    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& files, int x, int y);

    /** create timer for waveform rect */
    void timerCallback() override;

    /** take URL passed from playlist component to load file*/
    void sendToLoad(juce::URL audioURL);

private:

    //create imagebutton to play music
    juce::ImageButton playButton{ "PLAY" };
    juce::Image play = juce::ImageCache::getFromMemory(BinaryData::play_icon_png, BinaryData::play_icon_pngSize); 
    
    //create imagebutton to stop music
    juce::ImageButton stopButton{ "STOP" };
    juce::Image stop = juce::ImageCache::getFromMemory(BinaryData::pause_icon_png, BinaryData::pause_icon_pngSize);

    //create imagebutton to move position forward
    juce::ImageButton ffButton{ "FF" };
    juce::Image ff = juce::ImageCache::getFromMemory(BinaryData::ff_icon_png, BinaryData::ff_icon_pngSize);

    //create imagebutton to restart track
    juce::ImageButton rwButton{ "REWIND" };
    juce::Image rw = juce::ImageCache::getFromMemory(BinaryData::rewind_icon_png, BinaryData::rewind_icon_pngSize);

    //create load button
    juce::TextButton loadButton{ "LOAD" };

    //create volume, speed and position sliders with corresponding labels 
    juce::Slider volSlider;
    juce::Label volLabel;
    juce::Slider speedSlider;
    juce::Label speedLabel;
    juce::Slider posSlider;
    juce::Label posLabel;

    //reference DJAudioPlayer and waveform display
    DJAudioPlayer* player; 
    WaveformDisplay waveformDisplay;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGui)
};
