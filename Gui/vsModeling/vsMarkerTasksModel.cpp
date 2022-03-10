#include "vsMarkerTasksModel.h"

#include <QColor>
#include <QDebug>

#include <vsTools/vsOpenSimTools.h>

vsMarkerTasksModel::vsMarkerTasksModel():QAbstractTableModel(),
    m_currentModel(nullptr),
    m_markerData(new OpenSim::MarkerData())
{
}

void vsMarkerTasksModel::updateTasks(OpenSim::Model *model)
{

    setCurrentModel(model);

    auto modelMarkersSet  =model->getMarkerSet();

    qDebug() << "the IK markers size : " << modelMarkersSet.getSize();

    for (int i = 0; i < modelMarkersSet.getSize(); ++i) {
        auto marker = modelMarkersSet.get(i);
        OpenSim::IKMarkerTask* markerTask = new OpenSim::IKMarkerTask();
        markerTask->setName(marker.getName());
        //set the apply to true and the weight to one temporarly
        markerTask->setApply(true);
        markerTask->setWeight(1);

        m_ikMarkerTasks << markerTask;
        m_presentInFileMap[marker.getName()] = false;
    }

    emit layoutChanged();

}

void vsMarkerTasksModel::loadFromIKTool(OpenSim::InverseKinematicsTool *tool)
{

    setIkTool(tool);

    //loop over the tasks of the tool and extract only the MarkerTasks
    m_markerData = new OpenSim::MarkerData(tool->getMarkerDataFileName());
    vsOpenSimTools::tools->log(" trc file loaded, number of markers : "+QString::number(m_markerData->getMarkerNames().getSize()),"vsMarkerTasksModel");
    updatePresentInFileMap();

    qDebug() << "IK taskset size : " << tool->getIKTaskSet().getSize();
    if(tool->getIKTaskSet().getSize()>0){
        for (int i = 0; i < tool->getIKTaskSet().getSize(); ++i) {
            // convert the abstract task to a Marker task
            auto task = OpenSim::IKMarkerTask::safeDownCast(&(tool->getIKTaskSet().get(i)));
            if(!task) continue;

            // get the task index in the OpenSim Model
            auto taskIndex = m_currentModel->updMarkerSet().getIndex(task->getName());
            // replace the temporary model with the one from the IK tool
            m_ikMarkerTasks.replace(taskIndex,new OpenSim::IKMarkerTask(*task));

        }
    }

}

void vsMarkerTasksModel::toTaskSet(OpenSim::IKTaskSet &taskSet)
{
    //first clean all the existing tasks (markers)
    for (int i = taskSet.getSize()-1; i >= 0; i--) {
        auto markerTask = OpenSim::IKMarkerTask::safeDownCast(&taskSet.get(i));
        if(markerTask) taskSet.remove(i);
    }

    //next load the task present in the model

    foreach (auto task, m_ikMarkerTasks) {
        if(!task->getApply() || task->getWeight()== 0) continue;
        taskSet.cloneAndAppend(*task);
    }

}

void vsMarkerTasksModel::enableAllSelected()
{
    foreach (auto ikMarker, m_selectedTasks) {
        ikMarker->setApply(true);
    }
    updateIKUI();
    // to update the UI table
    emit layoutChanged();

}

void vsMarkerTasksModel::disableAllSelected()
{
    foreach (auto ikMarker, m_selectedTasks) {
        ikMarker->setApply(false);
    }
    updateIKUI();
    // to update the UI table
    emit layoutChanged();

}

bool vsMarkerTasksModel::areAllValuesSet()
{
    foreach (auto task, m_ikMarkerTasks) {
        //for values that will not be applied
        if(!task->getApply()) continue;
        if(m_presentInFileMap.value(task->getName(),false)){
            continue;
        }
        else{
            vsOpenSimTools::tools->log("Not all marker tasks values are set","vsMarkerTasksModel",vsOpenSimTools::Error);
            return false;
        }
    }
    return true;
}


int vsMarkerTasksModel::rowCount(const QModelIndex &parent) const
{
    return m_ikMarkerTasks.size();
}

int vsMarkerTasksModel::columnCount(const QModelIndex &parent) const
{
    return 4;
}

QVariant vsMarkerTasksModel::data(const QModelIndex &index, int role) const
{
    int rowNumber = index.row();
    int columnNumber = index.column();

    if(rowNumber >= m_ikMarkerTasks.size() || columnNumber >= 4) return QVariant();

    if(columnNumber == 0 && role == Qt::CheckStateRole){
        //we will return unchecked for now untill we create a map for the checked rows
        return m_ikMarkerTasks[rowNumber]->getApply()?Qt::Checked:Qt::Unchecked;
    }
    else if (columnNumber == 1 && role == Qt::DisplayRole){
        //for now we will just use a temporary name

        return QString::fromStdString(m_ikMarkerTasks[rowNumber]->getName());
    }
    else if (columnNumber == 2 && role == Qt::DisplayRole){
        auto markerTask = m_ikMarkerTasks[index.row()];
        bool taskValuePresent = m_presentInFileMap.value(markerTask->getName(),false);
        if(taskValuePresent)
            return "from file";
        else
            return "from file-- NOT FOUND";
    }
    else if (columnNumber ==2 && role == Qt::BackgroundRole){
        auto markerTask = m_ikMarkerTasks[index.row()];
        bool taskValuePresent = m_presentInFileMap.value(markerTask->getName(),false);
        if(taskValuePresent)
            return  QColor("white");
        else
            return QColor("red");
    }
    else if (columnNumber == 3 && role == Qt::DisplayRole){
        return m_ikMarkerTasks[rowNumber]->getWeight();
    }


    return  QVariant();
}

bool vsMarkerTasksModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid())
        return false;

    if(index.column()==0 && role == Qt::CheckStateRole){
        auto ikMarkerTask =  m_ikMarkerTasks[index.row()];
        if((Qt::CheckState)value.toInt() == Qt::Checked){
            ikMarkerTask->setApply(true);
            qDebug() << "item check state changed to true";
            updateIKUI();
            return true;
        }else if ((Qt::CheckState)value.toInt() == Qt::Unchecked){
            ikMarkerTask->setApply(false);
            qDebug() << "item check state changed to false";
            updateIKUI();
            return true;
        }

    }
    return false;
}

Qt::ItemFlags vsMarkerTasksModel::flags(const QModelIndex &index) const
{
    if(index.column() == 0) return QAbstractTableModel::flags(index) | Qt::ItemFlag::ItemIsUserCheckable;
    return QAbstractTableModel::flags(index) ;
}



QVariant vsMarkerTasksModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if( role != Qt::DisplayRole || section >= 4 || orientation != Qt::Horizontal) return QVariant();

    switch (section) {
    case 0:
        return "enabled";
        break;
    case 1:
        return "Marker Name";
        break;
    case 2:
        return "Value";
        break;
    case 3:
        return "Weight";
        break;
    default:
        return QVariant();
        break;
    }

}

void vsMarkerTasksModel::selectionModelChanged(QModelIndexList selected)
{
    qDebug() << "the size of the markers selected items > " << selected.size();
    m_selectedTasks.clear();
    foreach (auto index, selected) {
        if(!index.isValid()) continue;
        auto ikMarkerTask = m_ikMarkerTasks[index.row()];
        m_selectedTasks << ikMarkerTask;
    }

    updateIKUI();

}

void vsMarkerTasksModel::updateIKUI()
{
    m_allEnabled = true;
    m_allDisabled = true;

    foreach (auto markerTask, m_selectedTasks) {
        if(markerTask->getApply()) m_allDisabled = false;
        else m_allEnabled = false;
    }

    emit uiUpdated();

}

void vsMarkerTasksModel::updatePresentInFileMap()
{
    m_presentInFileMap.clear();
    if(!m_markerData) return;
    for (int i = 0; i < m_markerData->getMarkerNames().getSize(); ++i) {
        m_presentInFileMap[m_markerData->getMarkerNames().get(i)] = true;
    }

    //disable the markers that are not present in the data

    foreach (auto task, m_ikMarkerTasks) {
        if(m_presentInFileMap[task->getName()]) continue;
        task->setApply(false);
    }
}

void vsMarkerTasksModel::updateSelectedRowsWeight(double newWeight)
{
    foreach (auto task, m_selectedTasks) {
        task->setWeight(newWeight);
    }
    emit layoutChanged();
}

OpenSim::InverseKinematicsTool *vsMarkerTasksModel::getIkTool() const
{
    return m_ikTool;
}

void vsMarkerTasksModel::setIkTool(OpenSim::InverseKinematicsTool *ikTool)
{
    m_ikTool = ikTool;
}

OpenSim::Model *vsMarkerTasksModel::getCurrentModel() const
{
    return m_currentModel;
}

void vsMarkerTasksModel::setCurrentModel(OpenSim::Model *currentModel)
{
    m_currentModel = currentModel;
}

bool vsMarkerTasksModel::getAllDisabled() const
{
    return m_allDisabled;
}

bool vsMarkerTasksModel::getAllEnabled() const
{
    return m_allEnabled;
}
