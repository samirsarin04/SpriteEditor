#include "frame.h"


int Frame::globalID = 0;
Frame::Frame(int pixelDimension) : pixelDimension(pixelDimension), frameImage(pixelDimension, pixelDimension, QImage::Format_RGBA64) {
    //Default initializes everything
    ID = globalID;
    ++globalID;
    pixelSize = (640 / pixelDimension);
    canvasSize = pixelSize * pixelDimension;
    for(int i = 0; i < (pixelDimension * pixelDimension); i++){
        pixels.push_back(QColor(0, 0, 0, 0));
    }
    addToHistory(pixels);
    generateImage();
}

bool Frame::operator==(const Frame& other)const{
    return ID == other.ID;
}

QVector<QColor> Frame::getPixels() {
    return pixels;
}

QVector<QColor> Frame::addNewPixel(int x, int y, QColor color){
    modifyPixel(pixels, x, y, color);
    generateImage();
    return pixels;
}

QVector<QColor> Frame::addTemporaryPixel(int x, int y, QColor color){
    QVector<QColor> tempPixels = pixels;
    modifyPixel(tempPixels, x, y, color);
    return tempPixels;
}

QImage Frame::getImage(){
    return frameImage;
}

QImage Frame::generateImage(){
    int currentX = 0;
    int currentY = 0;
    //loop through each pixel and set the pixel color
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
    //get the yPixel location on the canvas
    int xPixel = ((double)pixelDimension * ((double)x / canvasSize));

    //get the xPixel location on the canvas
    int yPixel = ((double)pixelDimension * ((double)y / canvasSize));

    //get the index of the pixel
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

void Frame::clearHistory(){
    while (history.size() > 0){
        history.pop();
    }
}

QVector<QColor> Frame::undoAction(){
    if(history.size() > 1){
        //if the history is up to date undo the action
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

void Frame::setPixels(QVector<QColor> newPixels) {
    if (newPixels.size() == pixelDimension * pixelDimension) {
        pixels = newPixels;
        return;
    }
}
