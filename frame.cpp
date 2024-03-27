#include "frame.h"
#include <iostream>

Frame::Frame(int pixelDimension) : pixelDimension(pixelDimension) {
    pixelSize = (640 / pixelDimension);
    canvasSize = pixelSize * pixelDimension;
    for(int i = 0; i < (pixelDimension * pixelDimension); i++){
        pixels.push_back(QColor(255, 255, 255, 255));
    }
}

QVector<QColor> Frame::getPixels() {
    return pixels;
}

void Frame::addNewPixel(int x, int y, QColor color){

    if (x < 0 || x > canvasSize - 1 || y < 0 || y > canvasSize - 1) {
        return;
    }

    int xPixel = ((double)pixelDimension * ((double)x / canvasSize));
    int yPixel = ((double)pixelDimension * ((double)y / canvasSize));

    int location = xPixel + (yPixel * pixelDimension);

    pixels[location] = color;
}

QVector<QColor> Frame::getTemporaryPixels(int x, int y, QColor color){
    if (x < 0 || x > canvasSize - 1 || y < 0 || y > canvasSize - 1) {
        return pixels;
    }

    QVector<QColor> tempPixels = pixels;

    int xPixel = ((double)pixelDimension * ((double)x / canvasSize));
    int yPixel = ((double)pixelDimension * ((double)y / canvasSize));

    int location = xPixel + (yPixel * pixelDimension);

    tempPixels[location] = color;
    return tempPixels;
}
