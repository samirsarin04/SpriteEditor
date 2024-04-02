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
    explicit Model(QObject *parent = nullptr);

signals:
    ///@brief indicates that the new project button has been pressed
    void projectReset();

    ///@brief indicates that the canvas needs to be resized
    ///@param size the size to resize to
    void resizeCanvas(int size);

    ///@brief indicates the swatch color needs to update in the view
    ///@param swatch the swatch number (1 through 8)
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
    void canvasMovement(int x, int y, bool offCanvas);
    void colorChanged(Color color, int value);

    void addFrame();
    void removeFrame();
    void changeFrame(int ID);
    void updateFPS(int fps);

    void toolToPaint();
    void toolToEraser();
    void toolToDropper();
    void undoAction();
    void cloneButton();

    void generatePreview();
    void togglePreviewSize();

    void newProjectPressed();
    void messageYesSelected();

    void swatch1Clicked();
    void swatch2Clicked();
    void swatch3Clicked();
    void swatch4Clicked();
    void swatch5Clicked();
    void swatch6Clicked();

    void savePressed(QString& filename);
    void loadPressed(QString& filename);

private:
    int size;
    int fps;
    int activeSwatch;
    int imageIndex;
    int imageIndexCurrent;
    bool drawing;
    bool trueSizePreview;

    std::mutex lock;
    QTimer tick;

    QVector<Frame> frames;
    QVector<Frame> images;
    Frame* currentFrame;

    QColor currentColor;
    QColor eraserColor;
    QColor swatches[6];

    // Enum for tool selection
    enum Tool {paint, eraser, dropper};
    Tool currentTool;

    //Private  Methods
    void draw(int x, int y, QColor color);
    void updateImageVector();
    void detoggleActiveButton(Tool tool);
    void addSwatch(int swatchNumber);
    QString getStyleString(QColor color);
    void save(const QString& filename);

};

#endif // MODEL_H
