#ifndef VSINVERSEKINEMATICSUI_H
#define VSINVERSEKINEMATICSUI_H

#include <QWidget>

namespace Ui {
class vsInverseKinematicsUI;
}

class vsInverseKinematicsUI : public QWidget
{
    Q_OBJECT

public:
    explicit vsInverseKinematicsUI(QWidget *parent = nullptr);
    ~vsInverseKinematicsUI();

private:
    Ui::vsInverseKinematicsUI *ui;
};

#endif // VSINVERSEKINEMATICSUI_H
