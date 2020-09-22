#include "vsInverseKinematicsUI.h"
#include "ui_vsInverseKinematicsUI.h"

vsInverseKinematicsUI::vsInverseKinematicsUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::vsInverseKinematicsUI)
{
    ui->setupUi(this);
}

vsInverseKinematicsUI::~vsInverseKinematicsUI()
{
    delete ui;
}
