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

public slots:
    void onCurrentModelChanged();

private:
    Ui::vsInverseKinematicsUI *ui;
};

#endif // VSINVERSEKINEMATICSUI_H
