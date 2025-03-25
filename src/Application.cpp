/******************************************************************************
 * @file   Application.h
 * @author Tim Brewis (@t-bre, tab1g19@soton.ac.uk)
 * @brief  Main JUCE application
 *****************************************************************************/

#include "Application.h"

//==============================================================================

/**
 * @brief       Initialises the application
 *
 * @param[in]   commandLine     Command line arguments string
 */
void Application::initialise(const juce::String& /*commandLine*/)
{
    handleCommandLine(getCommandLineParameterArray());

    commandManager = std::make_shared<CommandManager>();
    commandManager->registerAllCommandsForTarget(this);

    mainWindow = std::make_unique<gui::MainWindow>(getApplicationName(),
                                                   configData,
                                                   commandManager);

    LookAndFeel::setDefaultLookAndFeel(&lookAndFeel);
}

//==============================================================================

/**
 * @brief Shuts down the application
 */
void Application::shutdown()
{
    // this prevents components from referencing the sufst::LookAndFeel instance
    // after it is destroyed
    LookAndFeel::setDefaultLookAndFeel(nullptr);

    mainWindow.reset();
}

/**
 * @brief Called when the application is being asked to quit
 */
void Application::systemRequestedQuit()
{
    quit(); // accept request to quit
}

/**
 * @brief       Called when another instance of the application is launched
 *              while this instance is running
 *
 * @param[in]   commandLine     Command line arguments string of other instance
 */
void Application::anotherInstanceStarted(const juce::String& /*commandLine*/)
{
}

/**
 * @brief   Handles
 *
 * @param commandLineArguments
 */
void Application::handleCommandLine(
    const juce::StringArray& commandLineArguments)
{
    juce::ArgumentList args(ProjectInfo::projectName, commandLineArguments);
#if UNIT_TEST
    // quick way to run unit tests
    juce::String testOption = "--test|-t";

    if (args.containsOption(testOption))
    {
        juce::UnitTestRunner unitTestRunner;
        unitTestRunner.setAssertOnFailure(false);

        auto value = args.getValueForOption(testOption);

        if (value.length() != 0)
        {
            unitTestRunner.runTestsInCategory(value);
        }
        else
        {
            unitTestRunner.runAllTests();
        }

        quit();
    }
#else
    ignoreUnused(commandLineArguments);
#endif

    if (args.containsOption("--help|-h"))
    {
        juce::String helpMessage
            = "Usage: " + getApplicationName()
              + " [options]\n\n"
                "Options:\n"
                "  --help, -h\t\tShow this help message\n"
                "  --file, -f [path]\tLoad config file from file path\n";
        std::cout << helpMessage;
        quit();
    }
    else if (args.containsOption("--file|-f"))
    {
        auto value = args.getValueForOption("--file|-f");
        if (value.length() != 0)
        {
            configData.loadFromFile(juce::File(value));
        }
    }
}

//==============================================================================

/**
 * @brief Returns the name of the application
 */
const juce::String Application::getApplicationName()
{
    return ProjectInfo::projectName;
}

/**
 * @brief Returns the version string of the application
 */
const juce::String Application::getApplicationVersion()
{
    return ProjectInfo::versionString;
}

/**
 * @brief   Sets whether or not multiple instances of the application can run
 *          at the same time
 */
bool Application::moreThanOneInstanceAllowed()
{
    return false;
}

//==============================================================================

/**
 * @brief   Implements juce::ApplicationCommandTarget::getNextCommandTarget()
 *
 * @details This ensures that the main window is found as a target by the
 *          CommandManager when the app launches. Without this, if the user does
 *          not interact with a component inside the main window, menu bar items
 *          will be greyed out (though keyboard shortcuts will still work).
 */
juce::ApplicationCommandTarget* Application::getNextCommandTarget()
{
    return mainWindow.get();
}