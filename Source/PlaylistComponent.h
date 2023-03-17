/*
  ==============================================================================

    PlaylistComponent.h
    Created: 20 Feb 2022 3:55:38pm
    Author:  Narmeen

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "DeckGui.h"
#include <vector>
#include <string>
#include <algorithm>

//==============================================================================
/*
*/
class PlaylistComponent : public juce::Component,
                          public juce::TableListBoxModel,
                          public juce::Button::Listener,
                          public juce::TextEditor::Listener,
                          public juce::FileDragAndDropTarget
{
public:
    PlaylistComponent(DeckGui* _deckGui1, DeckGui* _deckGui2);


    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    int getNumRows() override; 

    void paintRowBackground(juce::Graphics&,
        int rowNumber, int width,
        int height, bool rowIsSelected) override;

    void paintCell(juce::Graphics&,
        int rowNumber, int columnId, int width,
        int height, bool rowIsSelected) override;

    juce::Component* refreshComponentForCell(int rowNumber,
        int columnId, bool isRowSelected,
        Component* existingComponentToUpdate) override;

    void buttonClicked(juce::Button* button) override;

    bool isInterestedInFileDrag(const juce::StringArray& files) override;

    void filesDropped(const juce::StringArray& files, int x, int y);

    void findFiles(juce::String findFileText);

private:
    
    juce::TableListBox tableComponent;
    //create vectors to store file information (URL and name)
    std::vector<std::string> trackTitles;
    std::vector<juce::URL> trackURL;

    //create upload button and texteditor for searchbox
    juce::TextButton uploadButton{ "UPLOAD TRACK TO PLAYLIST" };
    juce::TextEditor findFileBox;

    //reference deckGui1 and 2 so the audioURL can be passed to them 
    DeckGui* deckGui1;
    DeckGui* deckGui2;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
