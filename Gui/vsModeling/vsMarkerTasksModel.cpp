#include "vsMarkerTasksModel.h"

#include <QDebug>

vsMarkerTasksModel::vsMarkerTasksModel():QAbstractTableModel()
{

}

void vsMarkerTasksModel::updateTasks(OpenSim::Model *model)
{

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
    }

    emit layoutChanged();

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
        return Qt::Unchecked;
    }
    else if (columnNumber == 1 && role == Qt::DisplayRole){
        //for now we will just use a temporary name

        return QString::fromStdString(m_ikMarkerTasks[rowNumber]->getName());
    }
    return  QVariant();
}

bool vsMarkerTasksModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return false;
}

Qt::ItemFlags vsMarkerTasksModel::flags(const QModelIndex &index) const
{
    return QAbstractTableModel::flags(index);
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
