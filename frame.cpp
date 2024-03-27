#include "frame.h"
#include <iostream>

Frame::Frame(int pixelDimension) : pixelDimension(pixelDimension) {
    pixelSize = (640 / pixelDimension);
    canvasSize = pixelSize * pixelDimension;
    //Default initializes frame to white, will be changed later
    for(int i = 0; i < (pixelDimension * pixelDimension); i++){
        pixels.push_back(QColor(255, 255, 255, 255));
    }
}

QVector<QColor> Frame::getPixels() {
    return pixels;
}

QVector<QColor> Frame::addNewPixel(int x, int y, QColor color){
    return modifyPixel(pixels, x, y, color);
}

QVector<QColor> Frame::addTemporaryPixel(int x, int y, QColor color){
    QVector<QColor> tempPixels = pixels;
    return modifyPixel(tempPixels, x, y, color);
}

QVector<QColor> Frame::modifyPixel(QVector<QColor> &pixels, int x, int y, QColor color){
    if (x < 0 || x > canvasSize - 1 || y < 0 || y > canvasSize - 1) {
        return pixels;
    }

    int xPixel = ((double)pixelDimension * ((double)x / canvasSize));
    int yPixel = ((double)pixelDimension * ((double)y / canvasSize));

    int location = xPixel + (yPixel * pixelDimension);

    pixels[location] = color;
    return pixels;
}
