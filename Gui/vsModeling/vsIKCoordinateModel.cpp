#include "vsIkCoordinateModel.h"


#include <QColor>
#include <QDebug>

#include <vsTools/vsOpenSimTools.h>

vsIKCoordinateModel::vsIKCoordinateModel():QAbstractTableModel(),
    m_currentModel(nullptr)
    //m_markerData(new OpenSim::MarkerData())
{
}

void vsIKCoordinateModel::updateTasks(OpenSim::Model *model)
{

    setCurrentModel(model);

    auto modelCoordinateSet  =model->updCoordinateSet();


    qDebug() << "the IK Coordinate size : " << modelCoordinateSet.getSize();

    for (int i = 0; i < modelCoordinateSet.getSize(); ++i) {
        auto coordinate = modelCoordinateSet.get(i);
        OpenSim::IKCoordinateTask* coordinateTask = new OpenSim::IKCoordinateTask();
        coordinateTask->setName(coordinate.getName());
        //set the apply to true and the weight to one temporarly
        coordinateTask->setApply(true);
        coordinateTask->setWeight(1);

        m_ikCoordinateTasks << coordinateTask;
        m_presentInFileMap[coordinate.getName()] = false;
    }

    emit layoutChanged();

}

void vsIKCoordinateModel::loadFromIKTool(OpenSim::InverseKinematicsTool *tool)
{

    setIkTool(tool);

    //loop over the tasks of the tool and extract only the MarkerTasks

    qDebug() << "IK taskset size : " << tool->getIKTaskSet().getSize();
    if(tool->getIKTaskSet().getSize()>0){
        for (int i = 0; i < tool->getIKTaskSet().getSize(); ++i) {

            // convert the abstract task to a Marker task
            auto task = OpenSim::IKCoordinateTask::safeDownCast(&(tool->getIKTaskSet().get(i)));

            if(!task) continue;

            // get the task index in the OpenSim Model
            auto taskIndex = m_currentModel->updCoordinateSet().getIndex(task->getName());
            // replace the temporary model with the one from the IK tool
            m_ikCoordinateTasks.replace(taskIndex,task);

        }
    }
    else{
        //m_Data = new OpenSim::MarkerData(tool->getMarkerDataFileName());
        vsOpenSimTools::tools->log(" mot,sto file loaded, number of coordinates : "/*+QString::number(m_->getMarkerNames().getSize())*/,"vsIKCoordinateModel");
        updatePresentInFileMap();
    }

}

void vsIKCoordinateModel::enableAllSelected()
{
    foreach (auto ikCoordinate, m_ikCoordinateTasks) {
        ikCoordinate->setApply(true);
    }
    updateIKUI();
    // to update the UI table
    emit layoutChanged();

}

void vsIKCoordinateModel::disableAllSelected()
{
    foreach (auto ikCoordinate, m_ikCoordinateTasks) {
        ikCoordinate->setApply(false);
    }
    updateIKUI();
    // to update the UI table
    emit layoutChanged();

}


int vsIKCoordinateModel::rowCount(const QModelIndex &parent) const
{
    return m_ikCoordinateTasks.size();
}

int vsIKCoordinateModel::columnCount(const QModelIndex &parent) const
{
    return 4;
}

QVariant vsIKCoordinateModel::data(const QModelIndex &index, int role) const
{
    int rowNumber = index.row();
    int columnNumber = index.column();

    if(rowNumber >= m_ikCoordinateTasks.size() || columnNumber >= 4) return QVariant();

    if(columnNumber == 0 && role == Qt::CheckStateRole){
        //we will return unchecked for now untill we create a map for the checked rows
        return m_ikCoordinateTasks[rowNumber]->getApply()?Qt::Checked:Qt::Unchecked;
    }
    else if (columnNumber == 1 && role == Qt::DisplayRole){
        //for now we will just use a temporary name

        return QString::fromStdString(m_ikCoordinateTasks[rowNumber]->getName());
    }
    else if (columnNumber == 2 && role == Qt::DisplayRole){

        //TODO set the value depending on the methode of selection
        auto coordinateTask = m_ikCoordinateTasks[index.row()];
        bool taskValuePresent = m_presentInFileMap.value(coordinateTask->getName(),false);
        if(taskValuePresent)
            return "from file";
        else
            return "from file-- NOT FOUND";
    }
    else if (columnNumber ==2 && role == Qt::BackgroundRole){
        auto coordinateTask = m_ikCoordinateTasks[index.row()];
        bool taskValuePresent = m_presentInFileMap.value(coordinateTask->getName(),false);
        if(taskValuePresent)
            return  QColor("white");
        else
            return QColor("red");
    }
    else if (columnNumber == 3 && role == Qt::DisplayRole){
        return m_ikCoordinateTasks[rowNumber]->getWeight();
    }


    return  QVariant();
}

bool vsIKCoordinateModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid())
        return false;

    if(index.column()==0 && role == Qt::CheckStateRole){
        auto ikCoordinateTask =  m_ikCoordinateTasks[index.row()];
        if((Qt::CheckState)value.toInt() == Qt::Checked){
            ikCoordinateTask->setApply(true);
            qDebug() << "item check state changed to true";
            updateIKUI();
            return true;
        }else if ((Qt::CheckState)value.toInt() == Qt::Unchecked){
            ikCoordinateTask->setApply(false);
            qDebug() << "item check state changed to false";
            updateIKUI();
            return true;
        }

    }
    return false;
}

Qt::ItemFlags vsIKCoordinateModel::flags(const QModelIndex &index) const
{
    if(index.column() == 0) return QAbstractTableModel::flags(index) | Qt::ItemFlag::ItemIsUserCheckable;
    return QAbstractTableModel::flags(index) ;
}



QVariant vsIKCoordinateModel::headerData(int section, Qt::Orientation orientation, int role) const
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

void vsIKCoordinateModel::selectionModelChanged(QModelIndexList selected)
{
    qDebug() << "the size of the coordinates selected items > " << selected.size();
    m_selectedTasks.clear();
    foreach (auto index, selected) {
        if(!index.isValid()) continue;
        auto ikCoordinateTask = m_ikCoordinateTasks[index.row()];
        m_selectedTasks << ikCoordinateTask;
    }

    updateIKUI();

}

void vsIKCoordinateModel::updateIKUI()
{
    m_allEnabled = true;
    m_allDisabled = true;

    foreach (auto coordinateTask, m_selectedTasks) {
        if(coordinateTask->getApply()) m_allDisabled = false;
        else m_allEnabled = false;
    }

    emit uiUpdated();

}

void vsIKCoordinateModel::updatePresentInFileMap()
{
    m_presentInFileMap.clear();
//    if(!m_markerData) return;
//    for (int i = 0; i < m_markerData->getMarkerNames().getSize(); ++i) {

//        m_presentInFileMap[m_markerData->getMarkerNames().get(i)] = true;
//    }

}

void vsIKCoordinateModel::updateSelectedRowsWeight(double newWeight)
{
    foreach (auto task, m_selectedTasks) {
        task->setWeight(newWeight);
    }
    emit layoutChanged();
}

OpenSim::InverseKinematicsTool *vsIKCoordinateModel::getIkTool() const
{
    return m_ikTool;
}

void vsIKCoordinateModel::setIkTool(OpenSim::InverseKinematicsTool *ikTool)
{
    m_ikTool = ikTool;
}

OpenSim::Model *vsIKCoordinateModel::getCurrentModel() const
{
    return m_currentModel;
}

void vsIKCoordinateModel::setCurrentModel(OpenSim::Model *currentModel)
{
    m_currentModel = currentModel;
}

bool vsIKCoordinateModel::getAllDisabled() const
{
    return m_allDisabled;
}

bool vsIKCoordinateModel::getAllEnabled() const
{
    return m_allEnabled;
}

