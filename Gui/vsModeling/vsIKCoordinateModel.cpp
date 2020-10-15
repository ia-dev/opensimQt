#include "vsIkCoordinateModel.h"


#include <QColor>
#include <QDebug>
#include <math.h>
#include <vsTools/vsOpenSimTools.h>

#define PI 3.141592653589793238463

vsIKCoordinateModel::vsIKCoordinateModel():QAbstractTableModel(),
    selectedValueType(OpenSim::IKCoordinateTask::DefaultValue),
    m_currentModel(nullptr),
    m_coordinateData(new OpenSim::Storage())
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
        coordinateTask->setWeight(0);
        coordinateTask->setValueType(OpenSim::IKCoordinateTask::DefaultValue);
        m_ikCoordinateTasks << coordinateTask;
        m_presentInFileMap[coordinate.getName()] = false;
    }

    emit layoutChanged();

}

void vsIKCoordinateModel::loadFromIKTool(OpenSim::InverseKinematicsTool *tool)
{

    setIkTool(tool);
    try {
        m_coordinateData = new OpenSim::Storage(tool->getCoordinateFileName());
        vsOpenSimTools::tools->log(" mot,sto file loaded, number of coordinates : "+QString::number(m_coordinateData->getColumnLabels().getSize()),"vsIKCoordinateModel");
    } catch (...) {
        vsOpenSimTools::tools->log("no coordinate file was specified while loading IKTool ","vsIKCoordinateModel");
    }

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
            m_ikCoordinateTasks.replace(taskIndex,new OpenSim::IKCoordinateTask(*task));
        }
    }
    updatePresentInFileMap();

}

void vsIKCoordinateModel::toTaskSet(OpenSim::IKTaskSet &taskSet)
{
    //first clean all the existing tasks (coordinates)
    for (int i = 0; i < taskSet.getSize(); ++i) {
        auto coordinateTask = OpenSim::IKCoordinateTask::safeDownCast(&taskSet.get(i));
        if(coordinateTask) taskSet.remove(i);
    }

    //next load the task present in the model

    foreach (auto task, m_ikCoordinateTasks) {
        if(!task->getApply() || task->getWeight()== 0) continue;
        taskSet.cloneAndAppend(*task);
    }

}

void vsIKCoordinateModel::enableAllSelected()
{
    foreach (auto ikCoordinate, m_selectedTasks) {
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

void vsIKCoordinateModel::setValueTypeForSelectedRow(OpenSim::IKCoordinateTask::ValueType valueType)
{
    foreach (auto selected, m_selectedTasks) {
        selected->setValueType(valueType);
    }
    allSelectedHaveSameValueType = true;
    emit layoutChanged();
}

double vsIKCoordinateModel::getDefaultValue(int index ) const
{
    auto motionType = m_currentModel->getCoordinateSet().get(index).getMotionType();

    auto motionFactor  = (motionType==OpenSim::Coordinate::Rotational)?180.0/PI:1.0;

    return (motionFactor * m_currentModel->getCoordinateSet().get(index).getDefaultValue());

}

void vsIKCoordinateModel::updateValuesManualy(double newValue)
{
//    auto firstItemIndex = m_currentModel->getCoordinateSet().getIndex(m_selectedTasks[0]->getName());
//    auto motionType = m_currentModel->getCoordinateSet().get(firstItemIndex).getMotionType();
//    auto motionFactor  = (motionType==OpenSim::Coordinate::Rotational)?180.0/PI:1.0;

    foreach (auto task, m_selectedTasks) {
        //perform right conversions or(in java) noticed that the value will be inserted without any regard for the MotionType
        task->setValue(newValue);
    }
    emit layoutChanged();

}

bool vsIKCoordinateModel::areAllValuesSet()
{
    foreach (auto coordinateTask, m_ikCoordinateTasks) {
        switch (coordinateTask->getValueType()) {
        case OpenSim::IKCoordinateTask::DefaultValue:{
            //restore the default value
            auto index  =  m_currentModel->getCoordinateSet().getIndex(coordinateTask->getName());
            // get the factor
            auto motionType = m_currentModel->getCoordinateSet().get(index).getMotionType();
            auto motionFactor  = (motionType==OpenSim::Coordinate::Rotational)?180.0/PI:1.0;

            coordinateTask->setValue(getDefaultValue(index)/motionFactor);
            }
            break;
        case OpenSim::IKCoordinateTask::FromFile:{
            if(!coordinateTask->getApply()) continue;
            if(!m_presentInFileMap.value(coordinateTask->getName(),false)){
                vsOpenSimTools::tools->log("Not all the files in the coordinate model are set","vsIKCoordinateModel",vsOpenSimTools::Error);
                return false;
            }
        }break;

        default:
            break;
        }
    }
    return true;
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

        switch (coordinateTask->getValueType()) {
        case OpenSim::IKCoordinateTask::FromFile:{
                bool taskValuePresent = m_presentInFileMap.value(coordinateTask->getName(),false);
                if(taskValuePresent)
                    return "from file";
                else
                    return "from file-- NOT FOUND";
            }
        case OpenSim::IKCoordinateTask::DefaultValue:
            return getDefaultValue(rowNumber);
            break;
        case OpenSim::IKCoordinateTask::ManualValue:
            return coordinateTask->getValue();
            break;
        default:
            return coordinateTask->getValue();
            break;
        }

    }
    else if (columnNumber ==2 && role == Qt::BackgroundRole){
        auto coordinateTask = m_ikCoordinateTasks[index.row()];
        if(coordinateTask->getValueType() == OpenSim::IKCoordinateTask::FromFile){
            bool taskValuePresent = m_presentInFileMap.value(coordinateTask->getName(),false);
            if(taskValuePresent)
                return  QColor("white");
            else
                return QColor("red");
        }else{
            return QColor("white");
        }
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
    //test if all the selected rows have the same value type
    qDebug() << "the size of the coordinates selected items > " << selected.size();
    m_selectedTasks.clear();
    bool allHaveSameType = true;
    //first we check only the type
    bool allHaveSameValue = true;
    OpenSim::IKCoordinateTask::ValueType firstType = m_ikCoordinateTasks[selected.first().row()]->getValueType();
    auto firstValue  = m_ikCoordinateTasks[selected.first().row()]->getValue();
    foreach (auto index, selected) {
        if(!index.isValid()) continue;
        auto ikCoordinateTask = m_ikCoordinateTasks[index.row()];
        m_selectedTasks << ikCoordinateTask;
        if(firstType != ikCoordinateTask->getValueType()) allHaveSameType = false;
        if(firstValue != ikCoordinateTask->getValue()) allHaveSameValue = false;
    }

    allSelectedHaveSameValueType = allHaveSameType;
    allSelectedHaveSameValue  =allHaveSameValue;
    selectedValueType = firstType;
    selectedValue = QString::number(m_ikCoordinateTasks[selected.first().row()]->getValue());
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
    if(!m_coordinateData) return;
    for (int i = 0; i < m_coordinateData->getColumnLabels().getSize(); ++i) {

        m_presentInFileMap[m_coordinateData->getColumnLabels().get(i)] = true;
    }

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

