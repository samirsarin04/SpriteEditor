#ifndef VIEW_H
#define VIEW_H

///@brief The header file for the view for this application. Gives forward declarations for all of the members
/// Reviewed by: Dawson Jenkins

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
    /// @brief constructor for the view
    /// @param model The model object that this application is using
    /// @param parent The parent widget which is defaulted to nullptr
    View(Model& model, QWidget *parent = nullptr);

    /// @brief the deconstructor whihc deleted the ui
    ~View();

public slots:
    /// @brief Handles when the red slider's values has changed updating the color
    void redSliderValueChanged();

    /// @brief Handles when the green slider's values has changed updating the color
    void greenSliderValueChanged();

    /// @brief Handles when the blue slider's value has changed updating the color
    void blueSliderValueChanged();

    /// @brief Handles when the alpha slider's value has changed updating the color
    void alphaSliderValueChanged();

    /// @brief Handles when the red spinbox's value has changed updating the color
    void redSpinBoxChanged();

    /// @brief Handles when the green spinbox's value has changed updating the color
    void greenSpinBoxChanged();

    /// @brief Handles when the blue spinbox's value has changed updating the color
    void blueSpinBoxChanged();

    /// @brief Handles when the alpha spinbox's value has changed updating the color
    void alphaSpinBoxChanged();

    /// @brief Handles when the canvas size has been confirmed for a new project
    void canvasSizeSelected();

    /// @brief Handles when the pixel dimensions slider has been changed
    void pixelDimensionSliderChanged();

    /// @brief Handles when the view wants the model to update a swatch color
    /// @param swatch The number of the swatch to update
    /// @param the style to add to the swatch
    void updateSwatchColor(int swatch, QString styleString);

    /// @brief Updates the current sliders positions to the given color
    /// @param The color to base from
    void updateColorSliders(QColor currentColor);

    /// @brief Updates the color preview box to the given styling
    /// @param The styling to update the color preview to
    void updateColorPreview(QString styleString);

    /// @brief Updates brush button
    /// @param Whether or not this button is selected
    void setBrush(bool enabled);

    /// @brief Updates eraser button
    /// @param Whether or not this button is selected
    void setEraser(bool enabled);

    /// @brief Updates dropper/picker button
    /// @param Whether or not this button is selected
    void setPicker(bool enabled);

    /// @brief sets the preview box to the given image
    /// @param image The QImage to show
    /// @param trueSizePreview Whether or not to show it in true to resolution
    /// @param size the size of the preview
    void setImagePreview(QImage image, bool trueSizePreview, int size);

    /// @brief sets preview image to the given image (animates)
    /// @param ID the current image id
    /// @param lastID the last image id animated
    void setImageIcon(QImage image, int ID, int lastID);

    /// @brief Handles the preview slider changing
    void imagePreviewSliderChanged();

    /// @brief Handles the preview spin box changing
    void imagePreviewSpinBoxChanged();

    /// @brief Handles the save button being pressed
    void savePressed();

    /// @brief Handles the load button being pressed
    void loadPressed();

    /// @brief resizes the canvas
    /// @param the size to change to
    void resizeCanvas(int size);

    /// @brief handling when the add frame is pressed
    /// @param ID id of frame
    void addPressed(int ID);

    /// @brief handling when the delete frame is pressed
    /// @param ID id of frame
    void deletePressed(int ID);

    /// @brief Handling when the user wants to reset the project
    void projectReset();

    /// @brief Handing errors
    /// @param message error message
    void errorOccurred(const QString &message);

    /// @brief handles message box asking if the project should be cleared
    void messageBox();

    /// @brief handles when help is pressed to show the app description
    void helpPressed();

signals:
    /// @brief signal sent when the color value has been changed
    /// @param color Color changed too
    /// @param value Value changed too
    void colorValueChanged(Color color, int value);

    /// @brief send canvas size
    /// @param size canvas size
    void canvasSizeSignal(int size);

    /// @brief display speed changed
    /// @param fps new frames per second
    void fpsChanged(int fps);

    /// @brief signals to model to save
    /// @param filename to save too
    void saveModel(QString& filename);

    /// @brief signals to model to load
    /// @param filename of file to load
    void loadModel(QString& filename);

    /// @brief tells model to add frame to model
    void sendAddFrame();

    /// @brief tells model to delete frame from model
    void deleteFrame();

    /// @brief signals when a new project is pressed
    void newProjectPressed();

    /// @brief signals when the new project is to be made
    void messageYesSelected();


private:
    // The UI file for this app
    Ui::View *ui;
    // pointer to the model
    Model *modelPtr;
    /// @brief enabled the color sliders
    /// @param enabled whether sliders are active
    void enableColorSliders(bool enabled);
};
#endif // VIEW_H
