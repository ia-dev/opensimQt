#ifndef VSIKCOORDINATEMODEL_H
#define VSIKCOORDINATEMODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include <OpenSim.h>
#include <QItemSelectionModel>
#include "OpenSim/Common/MarkerData.h"

class vsIKCoordinateModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    vsIKCoordinateModel();

    void updateTasks(OpenSim::Model *model);
    void loadFromIKTool(OpenSim::InverseKinematicsTool *tool);
    void toTaskSet(OpenSim::IKTaskSet &taskSet);
    void enableAllSelected();
    void disableAllSelected();
    void setValueTypeForSelectedRow(OpenSim::IKCoordinateTask::ValueType valueType);

    double getDefaultValue(int i) const; //< convert to radian or degress depending on the motiontype
    void updateValuesManualy(double newValue);


    bool areAllValuesSet();

    // QAbstractItemModel interface
public:
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual int columnCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    bool getAllEnabled() const;
    bool getAllDisabled() const;

    OpenSim::Model *getCurrentModel() const;
    void setCurrentModel(OpenSim::Model *currentModel);

    OpenSim::InverseKinematicsTool *getIkTool() const;
    void setIkTool(OpenSim::InverseKinematicsTool *ikTool);

public slots:

    void selectionModelChanged(QModelIndexList selected);
    void updateIKUI();
    void updatePresentInFileMap();
    void updateSelectedRowsWeight(double newWeight);

signals :
    void uiUpdated();

public:
    OpenSim::IKCoordinateTask::ValueType selectedValueType;
    bool allSelectedHaveSameValueType = false;
    bool allSelectedHaveSameValue = false;
    QString selectedValue;

private:
    QList<OpenSim::IKCoordinateTask*> m_ikCoordinateTasks;
    QList<OpenSim::IKCoordinateTask*> m_selectedTasks;
    bool m_allEnabled = false;
    bool m_allDisabled = false;

    OpenSim::Model *m_currentModel;
    OpenSim::InverseKinematicsTool *m_ikTool;
    OpenSim::Storage *m_coordinateData;

    QMap<std::string,bool> m_presentInFileMap;

};
#endif // VSIKCOORDINATEMODEL_H
