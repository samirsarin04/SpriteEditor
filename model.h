#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include "frame.h"

class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = nullptr);

signals:
    void sendFrameToCanvas(QVector<QColor>);
    void fillSwatch(int swatch, QColor color);
    void updateColorSliders(QColor currentColor);

public slots:
    void canvasClick(int x, int y, bool click);
    void canvasMovement(int x, int y, bool offCanvas);
    void colorChanged(QString color, int value);
    void newCanvas(int size);
    void toolToPaint();
    void toolToEraser();
    void toolToDropper();
    void swatch1Clicked();
    void swatch2Clicked();
    void swatch3Clicked();
    void swatch4Clicked();
    void swatch5Clicked();
    void swatch6Clicked();
private:
    enum Tool {paint, eraser, dropper};
    Tool currentTool;
    int size;
    QVector<Frame> frames;
    Frame* currentFrame;
    bool drawing;
    QColor currentColor;
    QVector<QColor> swatches;
    void addSwatch(int swatchNumber);
};

#endif // MODEL_H
