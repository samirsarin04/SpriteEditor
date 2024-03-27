#include "model.h"

#include<iostream>

Model::Model(QObject *parent)
    : QObject{parent}, currentFrame(64), drawing(false), currentColor(255, 0, 255, 255)
{

}

void Model::canvasClick(int x, int y, bool click){
    drawing = click;

    // WILL PASS IN CURRENT COLOR DEFAULT FOR NOW
    currentFrame.addNewPixel(x, y, currentColor);
    emit sendFrameToCanvas(currentFrame.getPixels());
}

void Model::canvasMovement(int x, int y, bool offCanvas){
    if(offCanvas) {
        emit sendFrameToCanvas(currentFrame.getPixels());
        return;
    }

    if (drawing){
        currentFrame.addNewPixel(x, y, currentColor);
        // WILL PASS IN CURRENT COLOR DEFAULT FOR NOW
        emit sendFrameToCanvas(currentFrame.getPixels());
    } else {
        // WILL PASS IN CURRENT COLOR DEFAULT FOR NOW
        emit sendFrameToCanvas(currentFrame.getTemporaryPixels(x, y, currentColor));
    }
}


void Model::sliderChanged(QString slider, int value) {
    if (slider == "red"){
        currentColor.setRed(value);
    }
    else if (slider == "green"){
        currentColor.setGreen(value);
    }
    else if (slider == "blue"){
        currentColor.setBlue(value);
    }
    else if (slider == "alpha"){
        currentColor.setAlpha(value);
    }
}
