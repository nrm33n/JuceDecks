/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 20 Feb 2022 3:55:38pm
    Author:  Narmeen

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent(DeckGui* _deckGui1, DeckGui* _deckGui2) :
                                    deckGui1(_deckGui1), deckGui2(_deckGui2) 
{
    //create the table to be used as a playlist library 
    tableComponent.getHeader().addColumn("Track title", 1, 300);
    tableComponent.getHeader().addColumn("Track length", 2, 100);
    tableComponent.getHeader().addColumn("", 3, 100);
    tableComponent.getHeader().addColumn(" ", 4, 100);

    tableComponent.setModel(this);

    //make table, upload button visible and add a listener to button
    addAndMakeVisible(tableComponent);
    addAndMakeVisible(uploadButton);
    uploadButton.addListener(this);

    //initialise search box with custom colours, text, etc 
    addAndMakeVisible(findFileBox);
    findFileBox.addListener(this);
    findFileBox.setColour(0x1000200, juce::Colours::white);
    findFileBox.applyColourToAllText(juce::Colours::black);
    findFileBox.setTextToShowWhenEmpty("Search...", juce::Colours::black);
    //start the findFiles( ) function on return 
    findFileBox.onReturnKey = [this] { findFiles(findFileBox.getText()); };
}

PlaylistComponent::~PlaylistComponent()
{
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::darkblue);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("PlaylistComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PlaylistComponent::resized()
{
    //position each part of the playlist component (determine size of buttons/search bar/ playlist area)
    double rowH = getHeight() / 7;
    double halfW = getWidth() / 2;
    uploadButton.setBounds(0, 0, halfW+halfW/4, rowH);
    findFileBox.setBounds(halfW+halfW/4, 0, getWidth(), rowH);
    tableComponent.setBounds(0, rowH, getWidth(), rowH*6);
}

int PlaylistComponent::getNumRows()
{
    //return playlist length using tracktitles vector
    return trackTitles.size();
}

void PlaylistComponent::paintRowBackground(juce::Graphics& g,
    int rowNumber, int width,
    int height, bool rowIsSelected)
{
    if (rowIsSelected) {
        //custom colour if selected
        g.fillAll(juce::Colours::lightgreen);
    }
    else {
        //custom colour if not selected
        g.fillAll(juce::Colours::lightpink);
    }
}

void PlaylistComponent::paintCell(juce::Graphics& g,
    int rowNumber, int columnId, int width,
    int height, bool rowIsSelected)
{
    if (columnId == 1) {
        if (rowNumber < getNumRows())
        {
            //automatically add track title in first column of playlist table 
            g.drawText(trackTitles[rowNumber],
                2, 0, width - 4, height,
                juce::Justification::centredLeft, true);
        }
    }
}

juce::Component* PlaylistComponent::refreshComponentForCell(int rowNumber,
    int columnId, bool isRowSelected,
    Component* existingComponentToUpdate)
{
    if (columnId == 3) {
        //create add to deck 1 button in column 3 of the playlist table 
        //also sets name, listener, and creates ID for button
        if (existingComponentToUpdate == nullptr) {
            juce::TextButton* btn = new juce::TextButton{ "Deck 1" };
            btn->setName("loadDeck1");
            btn->addListener(this);
            juce::String id{std::to_string(rowNumber)};
            btn->setComponentID(id);
            existingComponentToUpdate = btn;
        }
    }
    if (columnId == 4) {
        //create add to deck 2 button in column 4 of playlist table
        //set name, button listener, create ID for button 
        if (existingComponentToUpdate == nullptr) {
            juce::TextButton* btn = new juce::TextButton{ "Deck 2" };
            btn->setName("loadDeck2");
            btn->addListener(this);
            juce::String id{ std::to_string(rowNumber) };
            btn->setComponentID(id);
            existingComponentToUpdate = btn;
        }
    }
    //update all
    return existingComponentToUpdate;  
}

void PlaylistComponent::buttonClicked(juce::Button* button)
{
    //if upload button is clicked
    if (button == &uploadButton) {
        //file chooser allows user to browse for file 
        //URL of result is pushed back into the trackURL vector 
        //filename is parsed and displayed 
        juce::FileChooser chooser{ "select file" };
        juce::File files; 
            if (chooser.browseForFileToOpen()) {
                files = chooser.getResult();
                juce::String fileTitle = juce::String{ files.getFileNameWithoutExtension() };
                trackTitles.push_back(fileTitle.toStdString());
                trackURL.push_back(juce::URL{ chooser.getResult() });
            }
    }
    if (button->getName() == "loadDeck1") {
        //load the selected track into deck 1 by passing URL to deckGUI1
        int id = std::stoi(button->getComponentID().toStdString());
        DBG("ok loadDeck1 has been clicked");
        DBG("loadDeck1 tracktitles[id] is " << trackTitles[id]);
        deckGui1->sendToLoad(trackURL[id]);
    }
    if (button->getName() == "loadDeck2") {
        //load selected track into deck 2 by passing URL to deckGUI2
        int id = std::stoi(button->getComponentID().toStdString());
        DBG("ok loadDeck2 has been clicked");
        DBG("loadDeck2 trackTitles[id] is " << trackTitles[id]);
        deckGui2->sendToLoad(trackURL[id]);
    }
    //update all
    tableComponent.updateContent();
} 

bool PlaylistComponent::isInterestedInFileDrag(const juce::StringArray& files)
{
    DBG("Playlist Interest in File");
    return true;
}

void PlaylistComponent::filesDropped(const juce::StringArray& files, int x, int y)
{
    if (files.size() != 0)
    {
        DBG("files dropped");
        //juce::Array<juce::File, juce::DummyCriticalSection, 0> files;
        juce::File files;
        deckGui1->sendToLoad(juce::URL{ juce::File{ files } });
        deckGui2->sendToLoad(juce::URL{ files });
        trackTitles.push_back(files.getFileName().toStdString());
    }
}

void PlaylistComponent::findFiles(juce::String findFileText)
{
    DBG("ok looking for: " << findFileText);
    const auto title = trackTitles;
    const auto findtext = findFileText;
    //search through all track names for the string inputted into the search box 
    if (std::find(trackTitles.begin(), trackTitles.end(), findtext) != trackTitles.end())
    {
        //if found return true
        DBG("ok found!!");
    }
    else
    {
        //else return cannot be found 
        DBG(findFileText << "could not return any results!");
    }
    
}