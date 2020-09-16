
#include <vsPython/vsMacroManager.h>
#include <QCoreApplication>
#include <QDebug>
#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>
#include <vsTools/vsOpenSimTools.h>

std::mutex vsMacroManager::_mutex;
vsMacroManager::RecordingState _recording = vsMacroManager::RecordingState::NotStarted;

vsMacroManager& vsMacroManager::instance()
{
    static vsMacroManager _instance;
    return _instance;
}

void vsMacroManager::startRecording()
{
    try {
        std::lock_guard<std::mutex> startLock(_mutex);
        //need to have a clean command container
        if(_recording != vsMacroManager::RecordingState::Paused){
            _command_list.clear();
        }
        _recording = vsMacroManager::RecordingState::Started;
    } catch (std::exception& e) {
        vsOpenSimTools::tools->log("Macro recording start error",e.what(),vsOpenSimTools::Error,true);
        throw;
    }

}

void vsMacroManager::stopRecording()
{
    try {
        std::lock_guard<std::mutex> stopLock(_mutex);
        _recording = vsMacroManager::RecordingState::Stopped;;
        //write to file
        writeFile();
        //clear the command list
        _command_list.clear();
    } catch(WriteMacroFileException& we) {
      throw we;
    } catch (std::exception& e) {
        vsOpenSimTools::tools->log("Macro recording start error",e.what(),vsOpenSimTools::Error,true);
        throw e;
    }

}

void vsMacroManager::pauseRecording()
{
    _recording = vsMacroManager::RecordingState::Paused;
}

const QString vsMacroManager::getCurrentScript()
{
    if(_recents.empty()){
        return NULL;
    }
    return _recents.pop();
}


void vsMacroManager::registerAction(const QString &actionCommand, QAction *action)
{
    try {
        if(action){

            action->setData(actionCommand);
            connect(action,&QAction::triggered,&vsMacroManager::instance(),&vsMacroManager::actionTriggered);

        }

    } catch (std::exception& e) {
        vsOpenSimTools::tools->log("Macro action register error",e.what(),vsOpenSimTools::Error,true);
    }
}

void vsMacroManager::actionTriggered()
{
    try {
        std::lock_guard<std::mutex> actionLock(_mutex);
        if(_recording == vsMacroManager::RecordingState::Started){

            QAction* currentAction = static_cast<QAction*>(sender());
            _command_list.append(currentAction->data().toString());
        }
    } catch (std::exception& e) {
        vsOpenSimTools::tools->log("Macro action trigger error",e.what(),vsOpenSimTools::Error,true);
    }

}

void vsMacroManager::getCommandsFromConsole(const QString pyCommand)
{
    try {
        std::lock_guard<std::mutex> pyCommandLock(_mutex);
        if(_recording == vsMacroManager::RecordingState::Started){
            //push the python command to bottom of list
            _command_list.append(pyCommand);
        }
    } catch (std::exception& e) {
        vsOpenSimTools::tools->log("Macro action trigger error",e.what(),vsOpenSimTools::Error,true);
    }
}

void vsMacroManager::writeFile()
{
    try {
        auto documentPaths = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation);
        QString dirPath = QCoreApplication::applicationDirPath();
        if(!documentPaths.empty()){
            dirPath = documentPaths[0];
        }
        QString fileName = QFileDialog::getSaveFileName(nullptr,tr("Save Macro Script"),dirPath,tr("Python Files (*.py)"));

        if(fileName.isEmpty()){
            return;
        }
        else{
            QFile file(fileName);
               if (!file.open(QIODevice::WriteOnly)) {
                   QMessageBox::information(nullptr,tr("Unable to open file"),file.errorString());
                   return;
               }
               QTextStream out(&file);
               out << "#################################################################################################" << Qt::endl;
               out << "## OpensimQt                                                                                   ##" << Qt::endl;
               out << "## This is automatically recored macro script from the OpensimQt interface                     ##" << Qt::endl;
               out << "## Use the menu option Scripts > Run to open and run this macro script                         ##" << Qt::endl;
               out << "#################################################################################################" << Qt::endl << Qt::endl << Qt::endl;
               for(const QString& line:_command_list){
                   out << line.trimmed() << Qt::endl;
               }
               file.close();
               //add to the top of recent stack
             _recents.push(fileName);
        }


    } catch (WriteMacroFileException& e) {
        vsOpenSimTools::tools->log("Macro File Write Error",e.what(),vsOpenSimTools::Error,true);
    }
}
