#ifndef VIEW_H
#define VIEW_H

#include <QMainWindow>
#include <QFileDialog>
#include "model.h"
#include "canvas.h"
#include <QMessageBox>
#include "framepreviewbutton.h"
#include "color.h"

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
    ///@brief Handles when the red slider's values has changed updating the color
    void redSliderValueChanged();
    ///@brief Handles when the green slider's values has changed updating the color
    void greenSliderValueChanged();
    ///@brief Handles when the blue slider's value has changed updating the color
    void blueSliderValueChanged();
    ///@brief Handles when the alpha slider's value has changed updating the color
    void alphaSliderValueChanged();
    ///@brief Handles when the red spinbox's value has changed updating the color
    void redSpinBoxChanged();
    ///@brief Handles when the green spinbox's value has changed updating the color
    void greenSpinBoxChanged();
    ///@brief Handles when the blue spinbox's value has changed updating the color
    void blueSpinBoxChanged();
    ///@brief Handles when the alpha spinbox's value has changed updating the color
    void alphaSpinBoxChanged();
    ///@brief Handles when the canvas size has been confirmed for a new project
    void canvasSizeSelected();
    ///@brief Handles when the pixel dimensions slider has been changed
    void pixelDimensionSliderChanged();
    void updateSwatchColor(int swatch, QString styleString);
    void updateColorSliders(QColor currentColor);
    void updateColorPreview(QString styleString);
    void setBrush(bool enabled);
    void setEraser(bool enabled);
    void setPicker(bool enabled);
    void setImagePreview(QImage image, bool trueSizePreview, int size);
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
    void helpPressed();

signals:
    void colorValueChanged(Color color, int value);
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
