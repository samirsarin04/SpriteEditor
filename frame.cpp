#include "frame.h"
#include <iostream>

Frame::Frame(int pixelDimension) : pixelDimension(pixelDimension) {
    pixelSize = (640 / pixelDimension);
    canvasSize = pixelSize * pixelDimension;
    for(int i = 0; i < (pixelDimension * pixelDimension * 4); i++){
        pixels.push_back(255);
    }
}

QVector<int> Frame::getPixels() {
    return pixels;
}

void Frame::addNewPixel(int x, int y, int r, int g, int b, int a){

    if (x < 0 || x > canvasSize - 1 || y < 0 || y > canvasSize - 1) {
        return;
    }

    int xPixel = (double)((double)pixelDimension * ((double)x / canvasSize));
    int yPixel = (double)((double)pixelDimension * ((double)y / canvasSize));

    int location = xPixel + (yPixel * pixelDimension);
    location *= 4;

    pixels[location++] = r;
    pixels[location++] = g;
    pixels[location++] = b;
    pixels[location] = a;

}

QVector<int> Frame::getTemporaryPixels(int x, int y, int r, int g, int b, int a){
    if (x < 0 || x > canvasSize - 1 || y < 0 || y > canvasSize - 1) {
        return pixels;
    }

    QVector<int> tempPixels = pixels;

    int xPixel = (double)((double)pixelDimension * ((double)x / canvasSize));
    int yPixel = (double)((double)pixelDimension * ((double)y / canvasSize));

    int location = xPixel + (yPixel * pixelDimension);
    location *= 4;

    tempPixels[location++] = r;
    tempPixels[location++] = g;
    tempPixels[location++] = b;
    tempPixels[location] = a;

    return tempPixels;
}
