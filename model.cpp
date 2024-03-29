#include "model.h"

#include<iostream>

Model::Model(QObject *parent)
    : QObject{parent}, drawing(false), currentColor(255, 0, 255, 255)
{
    // initial tool
    currentTool = paint;
    QColor defaultSwatch(0,0,0);
    swatches = {defaultSwatch, defaultSwatch, defaultSwatch, defaultSwatch, defaultSwatch, defaultSwatch};
}

void Model::canvasClick(int x, int y, bool click){
    drawing = click;
    switch(currentTool){
    case paint:
        emit sendFrameToCanvas(currentFrame->addNewPixel(x, y, currentColor));
        break;
    case eraser:
        emit sendFrameToCanvas(currentFrame->addNewPixel(x, y, QColor(0, 0, 0, 0)));
        break;
    case dropper:
        // Dropper logic here
        currentColor = currentFrame->getPixelColor(x, y);
        currentTool = paint;
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
        case eraser:
            emit sendFrameToCanvas(currentFrame->addNewPixel(x, y, QColor(0, 0, 0, 0)));
            break;
        case dropper:
            // Dropper logic here
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

void Model::toolToPaint(){
    currentTool = paint;
}

void Model::toolToEraser(){
    currentTool = eraser;
}

void Model::toolToDropper(){
    currentTool = dropper;
}

void Model::swatch1Clicked(){
    addSwatch(0);
}

void Model::swatch2Clicked(){
    addSwatch(1);
}

void Model::swatch3Clicked(){
    addSwatch(2);
}

void Model::swatch4Clicked(){
    addSwatch(3);
}

void Model::swatch5Clicked(){
    addSwatch(4);
}

void Model::swatch6Clicked(){
    addSwatch(5);
}


void Model::addSwatch(int swatchNumber) {
    if (swatches.at(swatchNumber)!=currentColor && swatches.at(swatchNumber) == QColor(0,0,0)){
        swatches.replace(swatchNumber,currentColor);
        emit fillSwatch(swatchNumber + 1, currentColor);
    }
    else if (swatches.at(swatchNumber) == currentColor)
    {
        swatches.replace(swatchNumber, QColor(0,0,0));
        emit fillSwatch(swatchNumber + 1, swatches.at(swatchNumber));
    }
    else if (swatches.at(swatchNumber) != currentColor)
    {
        currentColor = swatches.at(swatchNumber);
        emit fillSwatch(swatchNumber, currentColor);
        emit updateColorSliders(currentColor);
    }


    /*
    if(swatches.size() > 0){
        if(swatches.at(swatchNumber) != currentColor){
            currentColor = swatches.at(swatchNumber);
            emit updateColorSliders(currentColor);
            emit fillSwatch(swatchNumber, currentColor);
        }
        else{
            if(swatches.at(swatchNumber)==currentColor){
                swatches.remove(swatchNumber);
                emit fillSwatch(swatchNumber, QColor(0,0,0,0));
            }
        }
    }
    else{
        swatches.push_back(currentColor);
        emit fillSwatch(swatchNumber, currentColor);
    }
    */
}



