#ifndef VIEW_H
#define VIEW_H

#include <QMainWindow>
#include <QFileDialog>
#include "model.h"
#include "canvas.h"
#include "button.h"
#include <QMessageBox>

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
    void updateSwatchColor(int swatch, QString styleString);
    void updateColorSliders(QColor currentColor);
    void updateColorPreview(QString styleString);
    void setBrush(bool enabled);
    void setEraser(bool enabled);
    void setPicker(bool enabled);
    void setImagePreview(QImage image);
    void imagePreviewSliderChanged();
    void imagePreviewSpinBoxChanged();
    void savePressed();
    void loadPressed();
    void resizeCanvas(int size);
    void addPressed();
    void projectReset();
    void errorOccurred(const QString &message);

signals:
    void colorValueChanged(QString color, int value);
    void canvasSizeSignal(int size);
    void fpsChanged(int fps);
    void saveModel(QString& filename);
    void loadModel(QString& filename);
    void addFrame();
    void newProjectPressed();


private:
    Ui::View *ui;
    QVector<QPushButton*> frameButtons;
};
#endif // VIEW_H
