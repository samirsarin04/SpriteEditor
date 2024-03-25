#include "canvas.h"

#include <iostream>

Canvas::Canvas(QWidget *parent)
    : QWidget{parent}
{
    setMouseTracking(true);
    setGridSize(64);
}

void Canvas::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton){
        emit canvasClickSignal(event->pos().x(), event->pos().y(), true);
    }
}

void Canvas::mouseReleaseEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton){
       emit canvasClickSignal(event->pos().x(), event->pos().y(), false);
    }
}

void Canvas::mouseMoveEvent(QMouseEvent *event){
    emit canvasMoveSignal(event->pos().x(), event->pos().y());
}

void Canvas::setGridSize(int pixelDimension){
    this->pixelDimension = pixelDimension;
    pixelSize = (640 / pixelDimension);
    canvasSize = pixelSize * pixelDimension;
    for(int i = 0; i < (pixelDimension * pixelDimension * 4); i++){
        pixels.push_back(0);
    }

    std::cout<< pixelSize << std::endl;
   // std::cout<< pixels.size() << std::endl;
}


void Canvas::paintEvent(QPaintEvent *event){
    QPainter painter(this);

    int currentX = 0;
    int currentY = 0;

    for (int i = 0; i < pixelDimension * pixelDimension * 4; i+=4){
        painter.fillRect(QRect(currentX, currentY, pixelSize, pixelSize), QColor(pixels.at(i), pixels.at(i + 1), pixels.at(i + 2), pixels.at(i + 3)));
        if (currentX >= canvasSize - pixelSize){
            currentX = 0;
            currentY += pixelSize;
        } else {
            currentX+= pixelSize;
        }
    }
}

void Canvas::updateCanvas(QVector<int> pixels){
    this->pixels = pixels;
    update();
}
