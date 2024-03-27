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
public:
    Frame(int pixelDimension);
    void addNewPixel(int x, int y, QColor color);
    QVector<QColor> getPixels();
    QVector<QColor> getTemporaryPixels(int x, int y, QColor color);
};

#endif // FRAME_H
