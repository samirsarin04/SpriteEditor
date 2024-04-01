#ifndef VIEW_H
#define VIEW_H

#include <QMainWindow>
#include <QFileDialog>
#include "model.h"
#include "canvas.h"
#include <QMessageBox>
#include "framepreviewbutton.h"
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
    void setImagePreview(QImage image, bool bigPreview, int size);
    void setImageIcon(QImage image, int ID, int lastID);
    void imagePreviewSliderChanged();
    void imagePreviewSpinBoxChanged();
    void savePressed();
    void loadPressed();
    void resizeCanvas(int size);
    void addPressed(int ID);
    void deletePressed(int ID);
    void projectReset();
    void errorOccurred(const QString &message);
    void messageBox();
    void handleDisplayFrame();

signals:
    void colorValueChanged(QString color, int value);
    void canvasSizeSignal(int size);
    void fpsChanged(int fps);
    void saveModel(QString& filename);
    void loadModel(QString& filename);
    void sendAddFrame();
    void deleteFrame();
    void newProjectPressed();
    void messageYesSelected();
    void displayFrame();


private:
    Ui::View *ui;
    Model *modelPtr;
    void enableColorSliders(bool enabled);


};
#endif // VIEW_H
