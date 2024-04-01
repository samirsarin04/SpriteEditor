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
    void projectReset();
    void resizeCanvas(int size);

    void fillSwatch(int swatch, QString styleString);
    void updateColorSliders(QColor currentColor);
    void updateColorPreview(QString styleString);

    void toggleBrush(bool enabled);
    void toggleEraser(bool enabled);
    void togglePicker(bool enabled);

    void sendFrameToCanvas(QVector<QColor>);
    void sendImageToPreview(QImage image, bool trueSizePreview, int size);

    void createFrameSelectorButton(int ID);
    void deleteFrameSelectorButton(int ID);
    void setFrameSelectorIcon(QImage image, int ID, int lastFrame);

    void messageBox();
    void errorOccurred(const QString &message);

public slots:
    void newCanvas(int size);

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
    void fullSizePlayback();

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
    bool playbackSize;

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
