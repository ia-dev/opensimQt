#include "vsScaleToolUi.h"
#include "ui_vsScaleToolUi.h"

vsScaleToolUI::vsScaleToolUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::vsScaleToolUI)
{
    ui->setupUi(this);
}

vsScaleToolUI::~vsScaleToolUI()
{
    delete ui;
}
