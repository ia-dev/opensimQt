#ifndef VSSCALETOOLUI_H
#define VSSCALETOOLUI_H

#include <QWidget>
#include <OpenSim.h>
namespace Ui {
class vsScaleToolUI;
}

class vsScaleToolUI : public QWidget
{
    Q_OBJECT

public:
    explicit vsScaleToolUI(QWidget *parent = nullptr);
    ~vsScaleToolUI();

    OpenSim::Model *currentModel() const;
    void setCurrentModel(OpenSim::Model *currentModel);

private:
    Ui::vsScaleToolUI *ui;

    OpenSim::ScaleTool *m_sclaeTool;
    OpenSim::Model* m_currentModel;
};

#endif // VSSCALETOOLUI_H
