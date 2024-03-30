#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include "frame.h"
#include <QTimer>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = nullptr);

signals:
    void sendFrameToCanvas(QVector<QColor>);
    void fillSwatch(int swatch, QString styleString);
    void updateColorSliders(QColor currentColor);
    void updateColorPreview(QString styleString);
    void toggleBrush(bool enabled);
    void toggleEraser(bool enabled);
    void togglePicker(bool enabled);
    void sendImage(QImage image);
    void resizeCanvas(int size);

public slots:
    void canvasClick(int x, int y, bool click);
    void canvasMovement(int x, int y, bool offCanvas);
    void colorChanged(QString color, int value);
    void newCanvas(int size);
    void updateFPS(int fps);
    void toolToPaint();
    void toolToEraser();
    void toolToDropper();
    void swatch1Clicked();
    void swatch2Clicked();
    void swatch3Clicked();
    void swatch4Clicked();
    void swatch5Clicked();
    void swatch6Clicked();
    void undoAction();
    void generatePreview();
    void savePressed(QString& filename);
    void loadPressed(QString& filename);
private:
    enum Tool {paint, eraser, dropper};
    Tool currentTool;
    int size;
    int fps;
    QVector<Frame> frames;
    Frame* currentFrame;
    bool drawing;
    QColor currentColor;
    QColor swatches[6];
    void addSwatch(int swatchNumber);
    void detoggleActiveButton(Tool tool);
    QString getStyleString(QColor color);
    int activeSwatch;
    QTimer tick;
    int imageIndex;
    std::mutex lock;
    void save(const QString& filename);
};

#endif // MODEL_H
