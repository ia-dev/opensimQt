#include "navigatormodel.h"
#include <QDebug>
#include <QIcon>
NavigatorModel::NavigatorModel()
{
    m_rootNNode = new NavigatorNode(nullptr,"the root",nullptr,this);
}

void NavigatorModel::loadOpenSimModel(OpenSim::Model *openSimModel)
{
    m_activeModel = openSimModel;
    //loading the model it self
    m_openModels.append(openSimModel);
    //m_rootOpenSimModel->initSystem();
    //m_rootOpenSimModel.getn

    NavigatorNode *modelNNode = new NavigatorNode(m_activeModel,"Model",m_rootNNode,this);
    //loading ground
    NavigatorNode *groundNode  = new NavigatorNode(&(m_activeModel->updGround()),"",modelNNode,this);

    //loading bodies
    NavigatorNode *bodySetNode = new NavigatorNode(nullptr,"Bodies",modelNNode,this);
    OpenSim::BodySet bodySet = m_activeModel->updBodySet();
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

    //loading joints
    NavigatorNode *jointsSetNode = new NavigatorNode(nullptr,"Joints",modelNNode,this);
    OpenSim::JointSet jointSet  = m_activeModel->updJointSet();
    for (int i = 0; i < jointSet.getSize(); ++i) {
        OpenSim::Joint *joint = &jointSet.get(i);
        NavigatorNode *jointNode = new NavigatorNode(joint,"",jointsSetNode,this);

        //loading transforms
        OpenSim::CustomJoint *cj = OpenSim::CustomJoint::safeDownCast(joint);
        if(cj != nullptr)
        {
            OpenSim::SpatialTransform jointTransform = cj->updSpatialTransform();
            for (int j = 0; j < 6; ++j) {
                OpenSim::TransformAxis *transformAxis = &jointTransform.updTransformAxis(j);
                NavigatorNode *axisNode = new NavigatorNode(transformAxis,"",jointNode,this);
            }
        }

        //loading frames
        for (int j = 0; j < joint->getProperty_frames().size(); ++j) {
            OpenSim::PhysicalOffsetFrame *pyOfstFrame =  &joint->upd_frames(j);
            NavigatorNode *frameNode = new NavigatorNode(pyOfstFrame,"",jointNode,this);
        }

    }
    emit layoutChanged();

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
    if (index.isValid() && role==Qt::DecorationRole) {
        NavigatorNode *nNode = nodeForIndex(index);
        return QVariant::fromValue(QPixmap(nNode->iconPath));
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
