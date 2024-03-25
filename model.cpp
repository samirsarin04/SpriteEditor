#include "model.h"

#include<iostream>

Model::Model(QObject *parent)
    : QObject{parent}, currentFrame(64), drawing(false)
{

}

void Model::canvasClick(int x, int y, bool click){
    drawing = click;

    // WILL PASS IN CURRENT COLOR DEFAULT FOR NOW
    currentFrame.addNewPixel(x, y, 255, 0, 255, 255);
    emit sendFrameToCanvas(currentFrame.getPixels());
}

void Model::canvasMovement(int x, int y){
    if (drawing){
        currentFrame.addNewPixel(x, y, 255, 0, 255, 255);
        // WILL PASS IN CURRENT COLOR DEFAULT FOR NOW
        emit sendFrameToCanvas(currentFrame.getPixels());
    } else {
        // WILL PASS IN CURRENT COLOR DEFAULT FOR NOW
        emit sendFrameToCanvas(currentFrame.getTemporaryPixels(x, y, 255, 0, 255, 255));
    }
}
