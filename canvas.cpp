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
    emit canvasMoveSignal(event->pos().x(), event->pos().y(), false);
}

void Canvas::leaveEvent(QEvent *event){
    emit canvasMoveSignal(0, 0, true);
}

void Canvas::setGridSize(int pixelDimension){
    this->pixelDimension = pixelDimension;
    pixelSize = (640 / pixelDimension);
    canvasSize = pixelSize * pixelDimension;
    for(int i = 0; i < (pixelDimension * pixelDimension); i++){
        pixels.push_back(QColor(0, 0, 0, 0));
    }
}


void Canvas::paintEvent(QPaintEvent *event){
    QPainter painter(this);

    int currentX = 0;
    int currentY = 0;

    for (int i = 0; i < pixelDimension * pixelDimension; i++){
        painter.fillRect(QRect(currentX, currentY, pixelSize, pixelSize), pixels.at(i));
        if (currentX >= canvasSize - pixelSize){
            currentX = 0;
            currentY += pixelSize;
        } else {
            currentX += pixelSize;
        }
    }
}

void Canvas::updateCanvas(QVector<QColor> pixels){
    this->pixels = pixels;
    update();
}
