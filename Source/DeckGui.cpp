/*
  ==============================================================================

    DeckGui.cpp
    Created: 19 Feb 2022 11:11:20am
    Author:  Narmeen

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGui.h"

//==============================================================================
DeckGui::DeckGui(DJAudioPlayer* _player,
                juce::AudioFormatManager & formatManagerToUse,
                juce::AudioThumbnailCache & cacheToUse
                ) : player(_player), 
                    waveformDisplay(formatManagerToUse, cacheToUse)
{
    //add and make visible all playback control buttons 
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(ffButton);
    addAndMakeVisible(rwButton);
    addAndMakeVisible(loadButton);

    //make waveform display visible
    addAndMakeVisible(waveformDisplay);

    //initialize all button images and add listeners 
    //include custom colours for when buttons are covered over and clicked
    playButton.setImages(true, true, true, play, 1.0f, {}, play, 1.0f, {juce::Colours::pink}, play, 1.0f, {juce::Colours::lightgreen});
    playButton.addListener(this);

    stopButton.setImages(true, true, true, stop, 1.0f, {}, stop, 1.0f, { juce::Colours::pink }, stop, 1.0f, { juce::Colours::lightgreen });
    stopButton.addListener(this);

    ffButton.setImages(true, true, true, ff, 1.0f, {}, ff, 1.0f, { juce::Colours::pink }, ff, 1.0f, { juce::Colours::lightgreen });
    ffButton.addListener(this);

    rwButton.setImages(true, true, true, rw, 1.0f, {}, rw, 1.0f, { juce::Colours::pink }, rw, 1.0f, { juce::Colours::lightgreen });
    rwButton.addListener(this);

    loadButton.addListener(this);

    //make all sliders and their labels visible, add listeners
    //also set label text and attach to their respective components 
    addAndMakeVisible(volSlider);
    addAndMakeVisible(volLabel);
    volSlider.addListener(this);
    volSlider.setRange(0.0, 1.0);
    volLabel.setText("Volume", juce::dontSendNotification);
    volLabel.attachToComponent(&volSlider, true);

    addAndMakeVisible(speedSlider);
    addAndMakeVisible(speedLabel);
    speedSlider.addListener(this);
    speedSlider.setRange(0.0, 10.0);
    speedLabel.setText("Speed", juce::dontSendNotification);
    speedLabel.attachToComponent(&speedSlider, true);

    addAndMakeVisible(posSlider);
    addAndMakeVisible(posLabel);
    posSlider.addListener(this);
    posSlider.setRange(0.0, 1.0);
    posLabel.setText("Position", juce::dontSendNotification);
    posLabel.attachToComponent(&posSlider, true);

    //begin timer for waveform component 
    startTimer(500);
}

DeckGui::~DeckGui()
{
    stopTimer();
}

void DeckGui::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::lightgrey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text

    //create custom paint designs around each button in the deck
    double rowH = getHeight() / 7;
    double col = getWidth() / 4; 
    g.setColour(juce::Colours::pink);
    g.drawRect(0, 0, col, rowH);
    g.drawRect(col, 0, col, rowH);
    g.drawRect(col * 2, 0, col, rowH);
    g.drawRect(col * 3, 0, col, rowH);
}

void DeckGui::resized()
{
    double rowH = getHeight() / 7;
    double col = getWidth() / 4;
    double sliderW = getWidth() / 8;
    //set relative locations for all buttons in the deck
    rwButton.setBounds(0, 0, col, rowH);
    playButton.setBounds(col, 0, col, rowH);
    stopButton.setBounds(col*2, 0, col, rowH);
    ffButton.setBounds(col * 3, 0, col, rowH);

    //set relative locations for all sliders in the deck
    volSlider.setBounds(sliderW, rowH, sliderW*7, rowH);
    speedSlider.setBounds(sliderW, rowH * 2, sliderW*7, rowH);
    posSlider.setBounds(sliderW, rowH * 3, sliderW*7, rowH);
    //set location for waveform display and load button
    waveformDisplay.setBounds(0, rowH * 4, getWidth(), rowH * 2);
    loadButton.setBounds(0, rowH * 6, getWidth(), rowH);
}

void DeckGui::buttonClicked(juce::Button* button)
{
    //if play button clicked, begin playing music
    if (button == &playButton) {
        DBG("play button clicked");
        player->start();
    }
    //if stop button clicked, stop playing music
    if (button == &stopButton) {
        DBG("stop button was clicked!");
        player->stop();
    }
    //if rewind button clicked, restart song
    if (button == &rwButton) {
        DBG("rw button clicked");
        player->setPosition(0);
    }
    //if forward button clicked, move position 0.1 from current position 
    if (button == &ffButton) {
        DBG("ff button clicked");
        double currPos = player->getPositionRelative();
        DBG("currPos");
        DBG(currPos);
        player->setPositionRelative(currPos + 0.1);
        
    }
    //if load button clicked, load a file and create waveform
    if (button == &loadButton) {
        juce::FileChooser chooser{ "Select a file" };
        if (chooser.browseForFileToOpen()) {
            player->loadURL(juce::URL{ chooser.getResult() });
            waveformDisplay.loadURL(juce::URL{ chooser.getResult() });
        }
    }
}

void DeckGui::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volSlider) {
        //if volume slider changed, change music volume by that value
        DBG("volslidr called value is ");
        DBG(slider->getValue());
        player->setGain(slider->getValue());
    }
    //if speed slider changed, change the music speed by that value
    if (slider == &speedSlider) {
        player->setSpeed(slider->getValue());
    }
    if (slider == &posSlider) {
        //change position according to value 
        player->setPositionRelative(slider->getValue());
    }
}

bool DeckGui::isInterestedInFileDrag(const juce::StringArray& files)
{
    return true;
}

void DeckGui::filesDropped(const juce::StringArray& files, int x, int y)
{
    DBG("files dropped");
    if (files.size() == 1)
    {
        //load files into deck after being dropped, load waveform and get ready to play music 
        player->loadURL(juce::URL{juce::File{files[0]}});
        waveformDisplay.loadURL(juce::URL{ juce::File{files[0]} });
    }
}

void DeckGui::timerCallback()
{
    //DBG("deckgui timercallback working");
    //set relative position for waveform display to create rectangle showing position 
    waveformDisplay.setPositionRelative(player->getPositionRelative());
}

void DeckGui::sendToLoad(juce::URL audioURL)
{
    //takes URL passed to DeckGui from PlaylistComponent 
    //loads music from playlist and creates waveform 
    DBG("ok deckgui sendtoload works");
    player->loadURL(audioURL);
    waveformDisplay.loadURL(audioURL);
}


