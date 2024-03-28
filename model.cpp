#include "model.h"

#include<iostream>

Model::Model(QObject *parent)
    : QObject{parent}, drawing(false), currentColor(255, 0, 255, 255)
{
    currentTool = dropper;
}

void Model::canvasClick(int x, int y, bool click){
    drawing = click;
    switch(currentTool){
    case paint:
        emit sendFrameToCanvas(currentFrame->addNewPixel(x, y, currentColor));
        break;
    case erase:
        emit sendFrameToCanvas(currentFrame->addNewPixel(x, y, QColor(0, 0, 0, 0)));
        break;
    case dropper:
        currentColor = currentFrame->getPixelColor(x, y);
        break;
    default:

        break;
    }
}

void Model::canvasMovement(int x, int y, bool offCanvas){
    if(offCanvas) {
        emit sendFrameToCanvas(currentFrame->getPixels());
        return;
    }

    if (drawing){
        switch(currentTool){
        case paint:
            emit sendFrameToCanvas(currentFrame->addNewPixel(x, y, currentColor));
            break;
        case erase:
            emit sendFrameToCanvas(currentFrame->addNewPixel(x, y, QColor(0, 0, 0, 0)));
            break;
        case dropper:
            //set current color to the color of what is clicked on
            currentColor = currentFrame->getPixelColor(x, y);
            break;
        default:
            // throw _exception("No tool selected");
            break;

        }

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
