/******************************************************************************
 * @file   Application.h
 * @author Tim Brewis (@t-bre, tab1g19@soton.ac.uk)
 * @brief  Main JUCE application
 *****************************************************************************/

#pragma once

#include <JuceHeader.h>
#include <limits.h>

#include "CommandManager.h"
#include "config/DataModel.h"
#include "gui/appearance/LookAndFeel.h"
#include "gui/windows/MainWindow.h"

/**
 * @brief Main JUCE application
 */
class Application : public juce::JUCEApplication
{
public:
    //==========================================================================
    Application() = default;

    //==========================================================================
    void initialise(const juce::String& commandLine) override;
    void shutdown() override;
    void systemRequestedQuit() override;
    void anotherInstanceStarted(const juce::String& commandLine) override;
    void handleCommandLine(const juce::StringArray& commandLineArguments);

    //==========================================================================
    const juce::String getApplicationName() override;
    const juce::String getApplicationVersion() override;
    bool moreThanOneInstanceAllowed() override;

    //==========================================================================
    juce::ApplicationCommandTarget* getNextCommandTarget() override;

private:
    //==========================================================================
    sufst::LookAndFeel lookAndFeel;

    std::unique_ptr<gui::MainWindow> mainWindow;
    std::shared_ptr<CommandManager> commandManager;

    config::DataModel configData;
};