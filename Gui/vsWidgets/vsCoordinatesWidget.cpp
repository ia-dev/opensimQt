/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020 by VisSim                                          *
 *                                                                         *
 *   Authors: Ibraheem Aldhamari, Yasser Grimes                            *
 *                                                                         *
 ***************************************************************************/
#include "vsCoordinatesWidget.h"
#include "ui_vsCoordinatesWidget.h"

#include <QDebug>

vsCoordinatesWidget::vsCoordinatesWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::vsCoordinatesWidget)
{
    ui->setupUi(this);

    QAction *newAction = new QAction("New...",this);
    QAction *setDefaultAction = new QAction("Set Default",this);
    QAction *deleteAction = new QAction("Delete...",this);
    m_posesActions << newAction << setDefaultAction << deleteAction;
}

vsCoordinatesWidget::~vsCoordinatesWidget()
{
    delete ui;
}

void vsCoordinatesWidget::addCoordinateDelegate(vsCoordinateDelegate *delegate)
{
    ui->listLayout->addWidget(delegate);
    delegates << delegate;
}

void vsCoordinatesWidget::removeCoordinateDelegate(vsCoordinateDelegate *delegate)
{
    ui->listLayout->removeWidget(delegate);
    delegates.removeOne(delegate);
}

void vsCoordinatesWidget::initializeWidgetForNewModel()
{
    //creating the default pose
    poses.clear();
    QList<CoordinateInfo> defaultValues;
    foreach (auto delegate, delegates) {

        defaultValues << delegate->getDefaultCoordinateInfo();
    }
    poses.insert("default",new QList<CoordinateInfo>(defaultValues));
}

void vsCoordinatesWidget::loadPose(QString k)
{
    qDebug() << k;
    auto coordinatesList = poses.value(k);
    if(coordinatesList->count()!= delegates.count()) qDebug() << "there is a missmatch between delegates and coordinates";
    for(int i=0 ; i < coordinatesList->count();i++) {
        delegates.at(i)->setCoordinateInfo(coordinatesList->at(i));
    }
}

QList<QAction *> vsCoordinatesWidget::getPosesActions()
{
    return m_posesActions;
}

QMenu* vsCoordinatesWidget::getPosesMenu()
{
    retMenu = new QMenu(nullptr);
    //add the poses
    foreach (QString k, poses.keys()) {
        QAction* poseAction = new QAction(k,retMenu);
        retMenu->addAction(poseAction);
        connect(poseAction,&QAction::triggered,[k,this](){this->loadPose(k);});
    }
    //add actions
    retMenu->addSeparator();
    retMenu->addActions(m_posesActions);
    return retMenu;
}
