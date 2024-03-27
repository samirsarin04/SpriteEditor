#include "model.h"

#include<iostream>

Model::Model(QObject *parent)
    : QObject{parent}, drawing(false), currentColor(255, 0, 255, 255)
{

}

void Model::canvasClick(int x, int y, bool click){
    drawing = click;
    emit sendFrameToCanvas(currentFrame->addNewPixel(x, y, currentColor));
}

void Model::canvasMovement(int x, int y, bool offCanvas){
    if(offCanvas) {
        emit sendFrameToCanvas(currentFrame->getPixels());
        return;
    }

    if (drawing){
        emit sendFrameToCanvas(currentFrame->addNewPixel(x, y, currentColor));
        return;
    }

    emit sendFrameToCanvas(currentFrame->addTemporaryPixel(x, y, currentColor));
}


void Model::colorChanged(QString color, int value) {
    if (color == "red"){
        currentColor.setRed(value);
    }
    else if (color == "green"){
        currentColor.setGreen(value);
    }
    else if (color == "blue"){
        currentColor.setBlue(value);
    }
    else if (color == "alpha"){
        currentColor.setAlpha(value);
    }
}

void Model::newCanvas(int size){
    frames.clear();
    this->size = size;
    frames.push_back(Frame(size));
    currentFrame = &frames[0];
}
