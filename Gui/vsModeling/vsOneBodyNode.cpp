#include "vsNavigatorModel.h"
#include "vsOneBodyNode.h"
#include "vsOneWrapObjectNode.h"

#include <QDebug>
#include <QInputDialog>

#include <vsTools/vsXmlUtils.h>

vsOneBodyNode::vsOneBodyNode(OpenSim::Body *body,vsNavigatorNode *parentNode,QObject *parent):
    vsOneFrameNode(body,parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/bodyNode.png";

    //wrap Objects
    OpenSim::WrapObjectSet wrapObjectSet = body->upd_WrapObjectSet();
    for (int j = 0; j < wrapObjectSet.getSize() ; ++j) {
        OpenSim::WrapObject *wrapObject = &wrapObjectSet.get(j);
        vsOneWrapObjectNode *wrapObjectNode = new vsOneWrapObjectNode(wrapObject,this,this);
    }
}

void vsOneBodyNode::setupNodeActions(QMenu *rootMenu)
{
    vsOneFrameNode::setupNodeActions(rootMenu);
    QAction *showMassCenterAction = new QAction("Show Mass Center",rootMenu);
    QAction *renameAction = new QAction("Rename...",rootMenu);
    QAction *removeAction = new QAction("Remove",rootMenu);

    connect(renameAction,&QAction::triggered,this,&vsOneBodyNode::onRenameActionTriggered);

    rootMenu->addActions(QList<QAction*>() << showMassCenterAction << renameAction << removeAction);

}

void vsOneBodyNode::onRenameActionTriggered()
{
    qDebug() << "renaming body";
    auto bodyObj = OpenSim::Body::safeDownCast(openSimObject);
    if(!bodyObj) return;
    QString newName = QInputDialog::getText(nullptr,"Rename","New Body Name: ");
    //vsXmlUtils::changeBodyName(bodyObj->getModel().getInputFileName(),bodyObj->getName(),newName.toStdString());
    bodyObj->setName(newName.toStdString());
    //TODO: BUG not all related objects are changed
    bodyObj->updModel().finalizeConnections();
    //NOTE! finalizeConnections work just fine
    //bodyObj->updModel().finalizeConnections();
    //bodyObj->updModel().print("newNameTEst.osim");
    displayName = newName;
    //TODO reload the model
    //modelObj->readObjectFromXMLNodeOrFile(modelElement,modelObj->getDocumentFileVersion());
    emit connectedModel()->layoutChanged();
    //reload the model afterword
}
