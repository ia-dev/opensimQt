#ifndef VSMARKERTASKSMODEL_H
#define VSMARKERTASKSMODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include <OpenSim.h>


class vsMarkerTasksModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    vsMarkerTasksModel();

    void updateTasks(OpenSim::Model *model);

    // QAbstractItemModel interface
public:
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual int columnCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    QList<OpenSim::IKMarkerTask*> m_ikMarkerTasks;



};

#endif // VSMARKERTASKSMODEL_H
