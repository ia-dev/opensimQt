#ifndef vsNavigatorModel_H
#define vsNavigatorModel_H

#include <QAbstractItemModel>
#include <QObject>
#include "Modeling/vsNavigatorNode.h"

class vsNavigatorModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    vsNavigatorModel();

    //~vsNavigatorModel();

    void loadOpenSimModel(OpenSim::Model *openSimModel);
    // QAbstractItemModel interface
public:
    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    virtual QModelIndex parent(const QModelIndex &child) const override;
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual int columnCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;

protected:
    QModelIndex indexForNNode(vsNavigatorNode *nNode);
    vsNavigatorNode* nodeForIndex(const QModelIndex &index) const;
    int rowForNode(vsNavigatorNode *node) const;
private:
    vsNavigatorNode *m_rootNNode;
    OpenSim::Model *m_activeModel;
    QList<OpenSim::Model*>m_openModels;
    SimTK::Visualizer *m_visualiser;

};

#endif // vsNavigatorModel_H
