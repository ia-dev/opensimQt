#ifndef VSSCALETOOLUI_H
#define VSSCALETOOLUI_H

#include <QWidget>

namespace Ui {
class vsScaleToolUI;
}

class vsScaleToolUI : public QWidget
{
    Q_OBJECT

public:
    explicit vsScaleToolUI(QWidget *parent = nullptr);
    ~vsScaleToolUI();

private:
    Ui::vsScaleToolUI *ui;
};

#endif // VSSCALETOOLUI_H
