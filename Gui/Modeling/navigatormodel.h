#ifndef NAVIGATORMODEL_H
#define NAVIGATORMODEL_H

#include <QAbstractItemModel>
#include <QObject>
#include "Modeling/navigatornode.h"

class NavigatorModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    NavigatorModel(NavigatorNode *rootNNode);

    //~NavigatorModel();

    // QAbstractItemModel interface
public:
    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    virtual QModelIndex parent(const QModelIndex &child) const override;
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual int columnCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;

protected:
    QModelIndex indexForNNode(NavigatorNode *nNode);
    NavigatorNode* nodeForIndex(const QModelIndex &index) const;
    int rowForNode(NavigatorNode *node) const;
private:
    NavigatorNode *m_rootNNode;

};

#endif // NAVIGATORMODEL_H
