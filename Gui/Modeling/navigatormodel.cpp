#include "navigatormodel.h"
#include <QDebug>
NavigatorModel::NavigatorModel(OpenSim::Model *rootModel)
{
    loadOpenSimModel(rootModel);

}

void NavigatorModel::loadOpenSimModel(OpenSim::Model *openSimModel)
{
    //loading the model it self
    m_rootOpenSimModel = openSimModel;
    //m_rootOpenSimModel->initSystem();
    //m_rootOpenSimModel.getn
    m_rootNNode = new NavigatorNode(nullptr,"the root",nullptr,this);
    NavigatorNode *modelNNode = new NavigatorNode(m_rootOpenSimModel,"Model",m_rootNNode,this);
    //loading ground
    NavigatorNode *groundNode  = new NavigatorNode(&(openSimModel->updGround()),"",modelNNode,this);

    //loading bodies
    NavigatorNode *bodySetNode = new NavigatorNode(nullptr,"Bodies",modelNNode,this);
    OpenSim::BodySet bodySet = m_rootOpenSimModel->updBodySet();
    for (int i = 0; i < bodySet.getSize(); i++) {
        OpenSim::Body *body = &(bodySet.get(i));
        NavigatorNode *bodyNode = new NavigatorNode(body,"items",bodySetNode,this);
//        qDebug() << "body display name : " <<bodyNode->displayName;
//        qDebug() << "body display name from item: " <<QString::fromStdString(bodyNode->openSimObject->getName());

        //geometry Objects

        for (int k = 0; k < body->getProperty_attached_geometry().size(); ++k) {

            OpenSim::Geometry* geo = &body->upd_attached_geometry(k);
            //qDebug()<< "first subcomponent > : "<< QString::fromStdString(geo->getName());
            NavigatorNode *geometryObjectNode = new NavigatorNode(geo,"",bodyNode,this);
        }
        //wrap Objects
        OpenSim::WrapObjectSet wrapObjectSet = body->upd_WrapObjectSet();
        for (int j = 0; j < wrapObjectSet.getSize() ; ++j) {
            OpenSim::WrapObject *wrapObject = &wrapObjectSet.get(j);
            NavigatorNode *wrapObjectNode = new NavigatorNode(wrapObject,"",bodyNode,this);
        }
//        while (true) {
            //try {
//                OpenSim::ComponentList<OpenSim::Component> componentList = frame->updComponentList();

//                OpenSim::ComponentListIterator<OpenSim::Component> componentsIterator = componentList.begin();
//                while (!componentsIterator.equals(componentList.end())) {
//                    qDebug()<< "first subcomponent > : "<< QString::fromStdString(componentsIterator.deref().getName());
//                }

//                qDebug()<<"the sizr of geometry set " << body->getProperty_attached_geometry().size() ;
//                const OpenSim::Geometry* geo = &body->get_attached_geometry(k);
//                if(geo == 0)
//                {
//                    qDebug() << "there is no geometry at index " << k;
//                    break;
//                }
//                qDebug()<< "first subcomponent > : "<< QString::fromStdString(geo->getName());
//                k++;
            //} catch (...) {
            //    break;
            //}
//        }


    }
//    foreach (OpenSim::Body *body, m_rootOpenSimModel->getBodySet()) {
//        NavigatorNode *bodyNode = new NavigatorNode(body,"",bodySetNode,this);
//    }

}

QModelIndex NavigatorModel::index(int row, int column, const QModelIndex &parent) const
{
    if(hasIndex(row,column,parent))
    {
        NavigatorNode *parentNode = nodeForIndex(parent);
        NavigatorNode *childNode = parentNode->childNodes.at(row);
        return createIndex(row,column,childNode);
    }
    return QModelIndex();
}

QModelIndex NavigatorModel::parent(const QModelIndex &child) const
{
    NavigatorNode *childNode = nodeForIndex(child);
    NavigatorNode *parentNode = childNode->parentNode;
    if(parentNode ==  m_rootNNode)
        return QModelIndex();
    int row = rowForNode(parentNode);
    int column = 0;
    return createIndex(row,column,parentNode);

}

int NavigatorModel::rowCount(const QModelIndex &parent) const
{
    NavigatorNode *parentNode = nodeForIndex(parent);
    return parentNode->childNodes.count();
}

int NavigatorModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant NavigatorModel::data(const QModelIndex &index, int role) const
{
    if(index.isValid() && role==Qt::DisplayRole)
    {
        NavigatorNode *nNode = nodeForIndex(index);
        return  nNode->displayName;
    }
    return QVariant();

}

QModelIndex NavigatorModel::indexForNNode(NavigatorNode *nNode)
{
    if(nNode == m_rootNNode)
        return QModelIndex();
    int row = rowForNode(nNode);
    int column = 0;
    return createIndex(row,column,nNode);
}

NavigatorNode* NavigatorModel::nodeForIndex(const QModelIndex &index) const
{
    if(index.isValid())
        return static_cast<NavigatorNode*>(index.internalPointer());
    return m_rootNNode;

}

int NavigatorModel::rowForNode(NavigatorNode *node) const
{
    return node->parentNode->childNodes.indexOf(node);
}
