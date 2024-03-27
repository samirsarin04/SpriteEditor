#ifndef VIEW_H
#define VIEW_H

#include <QMainWindow>
#include "model.h"
#include "canvas.h"
#include "button.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class View;
}
QT_END_NAMESPACE

class View : public QMainWindow
{
    Q_OBJECT

public:
    View(Model& model, QWidget *parent = nullptr);
    ~View();

private:
    Ui::View *ui;

private slots:
    void updateRedSliderLabel();
    void updateGreenSliderLabel();
    void updateBlueSliderLabel();
    void updateAlphaSliderLabel();
};
#endif // VIEW_H
