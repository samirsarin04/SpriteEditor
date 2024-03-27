#ifndef FRAME_H
#define FRAME_H

#include <QVector>
#include <QColor>

class Frame
{
private:
    QVector<QColor> pixels;
    int canvasSize;
    int pixelSize;
    int pixelDimension;
    QVector<QColor> modifyPixel(QVector<QColor> &pixels, int x, int y, QColor color);

public:
    Frame(int pixelDimension);
    QVector<QColor> addNewPixel(int x, int y, QColor color);
    QVector<QColor> getPixels();
    QVector<QColor> addTemporaryPixel(int x, int y, QColor color);
};

#endif // FRAME_H
