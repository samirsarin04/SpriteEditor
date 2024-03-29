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

public slots:
    void redSliderValueChanged();
    void greenSliderValueChanged();
    void blueSliderValueChanged();
    void alphaSliderValueChanged();
    void redSpinBoxChanged();
    void greenSpinBoxChanged();
    void blueSpinBoxChanged();
    void alphaSpinBoxChanged();
    void canvasSizeSelected();
    void pixelDimensionSliderChanged();
    void updateSwatchColor(int swatch, QColor color);
    void updateColorSliders(QColor currentColor);

signals:
    void colorValueChanged(QString color, int value);
    void canvasSizeSignal(int size);

private:
    Ui::View *ui;
};
#endif // VIEW_H
