#ifndef FRAME_H
#define FRAME_H

#include <QVector>

class Frame
{
private:
    QVector<int> pixels;
    int canvasSize;
    int pixelSize;
    int pixelDimension;
public:
    Frame(int pixelDimension);
    void addNewPixel(int x, int y, int r, int g, int b, int a);
    QVector<int> getPixels();
    QVector<int> getTemporaryPixels(int x, int y, int r, int g, int b, int a);
};

#endif // FRAME_H
