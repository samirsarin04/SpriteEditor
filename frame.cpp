#include "frame.h"
#include <iostream>
#include <QDebug>

Frame::Frame(int pixelDimension) : pixelDimension(pixelDimension), frameImage(pixelDimension, pixelDimension, QImage::Format_RGBA64) {
    firstStroke = true;
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
    modifyPixel(pixels, x, y, color);
    return pixels;
}

QVector<QColor> Frame::addTemporaryPixel(int x, int y, QColor color){
    QVector<QColor> tempPixels = pixels;
    modifyPixel(tempPixels, x, y, color);
    return tempPixels;
}

QImage Frame::generateImage(){
    int currentX = 0;
    int currentY = 0;
    for (int i = 0; i < pixelDimension * pixelDimension; i++){
        frameImage.setPixelColor(QPoint(currentX, currentY), pixels.at(i));
        if (currentX >= pixelDimension - 1){
            currentX = 0;
            currentY += 1;
        } else {
            currentX += 1;
        }
    }
    return frameImage;
}

void Frame::modifyPixel(QVector<QColor> &pixels, int x, int y, QColor color){
    if (x < 0 || x > canvasSize - 1 || y < 0 || y > canvasSize - 1) {
        return;
    }
    int xPixel = ((double)pixelDimension * ((double)x / canvasSize));
    int yPixel = ((double)pixelDimension * ((double)y / canvasSize));

    int location = xPixel + (yPixel * pixelDimension);

    pixels[location] = color;
}

QColor Frame::getPixelColor(int x, int y){
    int xPixel = ((double)pixelDimension * ((double)x / canvasSize));
    int yPixel = ((double)pixelDimension * ((double)y / canvasSize));

    int location = xPixel + (yPixel * pixelDimension);

    return pixels[location];
}

void Frame::addToHistory(QVector<QColor> pixels){
    history.push(pixels);
}

bool Frame::getFirstStroke(){
    return firstStroke;
}

void Frame::setFirstStroke(bool firstStroke){
    this->firstStroke = firstStroke;
}

QVector<QColor> Frame::undoAction(){
    if(history.size() > 1){
        while(history.top() == pixels && history.size() > 1){
            history.pop();
        }
        auto lastVersion = history.top();
        pixels = lastVersion;
        return pixels;
    }
    else{
        return pixels;
    }
}

void Frame::setPixels(const QVector<QColor>& newPixels) {
    if (newPixels.size() == pixelDimension * pixelDimension) {
        pixels = newPixels;
    } else {
        qDebug() << "Error: Pixel data size mismatch.";
    }
}
