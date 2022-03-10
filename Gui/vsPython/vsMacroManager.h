#ifndef VSMACRORECORDER_H
#define VSMACRORECORDER_H

/*************************************************************************************************************************************************************************************
 * GUI Macro Recording Functionality
 *
 *
 * openSimQt provides Macro recording functionality which allows users to record the GUI actions (commands) in form of a script.
 * By this way users can record a script for repeating steps and automate the process.
 * Such script could be saved to disk and loaded at any time through the openSimQt GUI, to run.
 *
 * Caution: The script is a python script and uses openSimQt python api. Hence it's usage outside the openSimQt GUI is not supported.
 *
 * Below are the features provided by GUI Macro Recording Module:
 *
 * - Record Macro (Start Recording)
 *     - Menu: Scripts > Record Macro
 *     - Toolbar: Record icon on Scripts Toolbar
 *     - Starts the a recording session. All GUI Actions and commands in Python Scripting Console will be recorded.
 * - Pause Recording
 *     - Menu: Scripts > Pause Recording
 *     - Toolbar: Pause icon on Scripts Toolbar
 *     - Pauses the recording session.
 * - Stop Recording
 *     - Menu: Scripts > Stop Recording
 *     - Toolbar: Stop icon on Scripts Toolbar
 *     - Stops the recording session and prompts for file storage location and file name. By default the ".py" extension will be added. Users don't have to type the extension.
 * - Run Current Script
 *     - Menu: Scripts > Run Current Script
 *     - Toolbar: Run Current Script
 *     - Runs the last recorded macro in active session of openSimQt.
 * - Run Macro
 *     - Menu: Scripts > Run
 *     - Toolbar: Run on the Scripts Toolbar
 *     - Presents the open file dialog and prompts user to open the script file of desired macro. Runs the file so selected by the user.
 *************************************************************************************************************************************************************************************/

#include <QObject>
#include <QAction>
#include <QStack>
#include <mutex>
#include <shared_mutex>
#include <iostream>

class vsMacroManager : public QObject {
    Q_OBJECT

private:
    explicit vsMacroManager(QObject* parent = nullptr) {}
    vsMacroManager(const vsMacroManager&) = delete;
    vsMacroManager(vsMacroManager&&) = delete;
    vsMacroManager& operator=(const vsMacroManager&) = delete;
    vsMacroManager& operator=(vsMacroManager&&) = delete;

    virtual ~vsMacroManager(){}

public:
    static vsMacroManager& instance();
    void startRecording();
    void stopRecording();
    void pauseRecording();
    const QString getCurrentScript();
    void registerAction(const QString& actionCommand, QAction* action);

    enum class RecordingState { NotStarted, Started,Paused,Stopped };

public slots:
    void actionTriggered();
    void getCommandsFromConsole(const QString pyCommand);
private:
    void writeFile();

private:
    static std::mutex _mutex;
    QStringList _command_list;
    QStack<QString> _recents;
    RecordingState _recording;
};

class WriteMacroFileException : public std::exception{
public:
    WriteMacroFileException(){}
    ~WriteMacroFileException() noexcept {}
    virtual const char* what() const noexcept {return "Macro file write error";}
};

#endif // VSMACRORECORDER_H
