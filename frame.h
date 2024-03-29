#ifndef FRAME_H
#define FRAME_H

#include <QVector>
#include <QColor>
#include <stack>
#include <QImage>

class Frame
{
private:
    bool firstStroke;
    QVector<QColor> pixels;
    int canvasSize;
    int pixelSize;
    int pixelDimension;
    QVector<QColor> modifyPixel(QVector<QColor> &pixels, int x, int y, QColor color);
    std::stack<QVector<QColor>> history;
    QImage frameImage;

public:
    Frame(int pixelDimension);
    QVector<QColor> addNewPixel(int x, int y, QColor color);
    QVector<QColor> getPixels();
    QVector<QColor> addTemporaryPixel(int x, int y, QColor color);
    QColor getPixelColor(int x, int y);
    QVector<QColor> undoAction();
    void addToHistory(QVector<QColor> pixels);
    bool getFirstStroke();
    void setFirstStroke(bool firstStroke);
    QImage generateImage();
};

#endif // FRAME_H
