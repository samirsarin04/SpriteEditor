///@brief The header file for the model for this application. gives forward declarations for all of the members
///Reviewed by: Peyton Jensen

#ifndef MODEL_H
#define MODEL_H
#include <QObject>
#include "frame.h"
#include <QTimer>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "color.h"


class Model : public QObject
{
    Q_OBJECT
public:
    ///@brief constructs a model object
    ///@param parent set to nullptr for intitialization
    explicit Model(QObject *parent = nullptr);

signals:
    ///@brief indicates that the new project button has been pressed
    void projectReset();

    ///@brief indicates that the canvas needs to be resized
    ///@param size the size to resize to
    void resizeCanvas(int size);

    ///@brief indicates the swatch color needs to update in the view
    ///@param swatch the swatch number (1 through 6)
    ///@param styleString the string used to update the styleSheet
    void fillSwatch(int swatch, QString styleString);

    ///@brief indicates that the color sliders need to be updated
    ///@param currentColor the color that contains all 4 new slider values
    void updateColorSliders(QColor currentColor);

    ///@brief indicates that the color preview needs to be updated
    ///@param styleString the string used to update the styleSheet
    void updateColorPreview(QString styleString);

    ///@brief indicates that the brush needs to be toggled
    ///@param enabled the flag indicating if the brush is to be toggled
    void toggleBrush(bool enabled);

    ///@brief indicates that the eraser needs to be toggled
    ///@param enabled the flag indicating if the eraser is to be toggled
    void toggleEraser(bool enabled);

    ///@brief indicates that the color picker needs to be toggled
    ///@param enabled the flag indicating if the color picker is to be toggled
    void togglePicker(bool enabled);

    ///@brief indicates that a frame needs to be sent to the canvas
    ///@param the frame to send to the canvas
    void sendFrameToCanvas(QVector<QColor>);

    ///@brief indicates that the preview window needs to be updated
    ///@param image the image to send to the preview
    ///@param trueSizePreview the flag indicating if the trueSizePreview is toggled
    ///@param size the size of the image
    void sendImageToPreview(QImage image, bool trueSizePreview, int size);

    ///@brief indicates that the create frame button has been pressed
    ///@param ID the id of the frame
    void createFrameSelectorButton(int ID);

    ///@brief indicates that the delete frame button has been pressed
    ///@param ID the id of the frame
    void deleteFrameSelectorButton(int ID);

    ///@brief indicates what frame has been selected
    ///@param image the image of the frame selected
    ///@param ID the ID of the frame
    ///@param lastFrame the last frame that was selected
    void setFrameSelectorIcon(QImage image, int ID, int lastFrame);

    ///@brief indicates that the message box needs to be displayed
    void messageBox();

    ///@brief indicates that an error message should be displayed
    ///@param message the message to be displayed
    void errorOccurred(const QString &message);

public slots:
    ///@brief used to reset the project when new project is pressed
    ///@param size the size of the new canvas
    void newCanvas(int size);

    ///@brief used to handle a canvas int
    ///@param x the x location of the interaction
    ///@param y the y location of the interaction
    ///@param click indicates whether the canvas
    void canvasClick(int x, int y, bool click);

    ///@brief used to handle movement across the canvas
    ///@param x the x location of the movement
    ///@param y the y location of the movement
    ///@param offCanvas the flag indicating if the cursor is off canvas
    void canvasMovement(int x, int y, bool offCanvas);

    ///@brief used to set a color when it changes
    ///@param value the new value of the color
    void colorChanged(Color color, int value);

    ///@brief used to add a new frame
    void addFrame();

    ///@brief used to remove a frame
    void removeFrame();

    ///@brief used to change frames when a new one is selected
    ///@param ID the id of the frame to change to
    void changeFrame(int ID);

    ///@brief used to update the fps of the program
    ///@param fps the new fps of the program
    void updateFPS(int fps);

    ///@brief sets the tool to the paint
    void toolToPaint();

    ///@brief sets the tool to eraser
    void toolToEraser();

    ///@brief sets the tool to dropper
    void toolToDropper();

    ///@brief undoes the last action made
    void undoAction();

    ///@brief clones the current frame to a new one
    void cloneButton();

    ///@brief generates the preview each tick
    void generatePreview();

    ///@brief toggles between true size and larger size playback
    void togglePreviewSize();

    ///@brief emits the "are you sure" message when new project is pressed
    void newProjectPressed();

    ///@brief resets the project when "yes" is selected in the dialog after pressing new project
    void messageYesSelected();

    ///@brief calls addSwatch with swatch 1's index
    void swatch1Clicked();

    ///@brief calls addSwatch with swatch 2's index
    void swatch2Clicked();

    ///@brief calls addSwatch with swatch 3's index
    void swatch3Clicked();

    ///@brief calls addSwatch with swatch 4's index
    void swatch4Clicked();

    ///@brief calls addSwatch with swatch 5's index
    void swatch5Clicked();

    ///@brief calls addSwatch with swatch 6's index
    void swatch6Clicked();

    ///@brief saves the current project to the filename using json
    ///@param filename the filename to save to
    void savePressed(QString& filename);

    ///@brief loads a project from the file
    ///@param filename the filename to load from
    void loadPressed(QString& filename);

private:
    ///@brief the size of the sprite project
    int size;

    ///@brief the fps of the preview animation
    int fps;

    ///@brief the swatch that is currently selected
    int activeSwatch;

    ///@brief used to send images in order to preview
    int imageIndex;

    ///@brief the current image index
    int imageIndexCurrent;

    ///@brief flag indicating if the user is currently drawing
    bool drawing;

    ///@brief the flag indicating if the true size preview is enabled
    bool trueSizePreview;

    ///@brief lock used to avoid concurrency issues with things like frame playback
    std::mutex lock;

    ///@brief timer used to emit frames to the playback using fps
    QTimer tick;

    ///@brief the frames vector containing all the frames
    QVector<Frame> frames;

    ///@brief the images vector containing all the generated images
    QVector<Frame> images;

    ///@brief pointer to the current frame that is selected
    Frame* currentFrame;

    ///@brief the current selected color
    QColor currentColor;

    ///@brief the eraser color used to erase pixels
    QColor eraserColor;

    ///@brief the list of colors stored in the swatches
    QColor swatches[6];

    ///@brief the enum used to represent the tools
    enum Tool {paint, eraser, dropper};

    ///@brief the current tool selected
    Tool currentTool;

    ///@brief used to draw on the current frame
    ///@param x the x location to draw at
    ///@param y the y location to draw at
    ///@param color the color to draw with
    void draw(int x, int y, QColor color);

    ///@brief used to update the image vector when editing frames & images
    void updateImageVector();

    ///@brief used to detoggle the active tool
    ///@param tool the tool to detoggle
    void detoggleActiveButton(Tool tool);

    ///@brief used to add a swatch to the swatches
    ///@param swatchNumber the swatch to be added
    void addSwatch(int swatchNumber);

    ///@brief used to generate the string for stylesheets in the view based on the color to be set
    ///@param color the color to set the view element's stylesheet to
    QString getStyleString(QColor color);
};
#endif // MODEL_H
