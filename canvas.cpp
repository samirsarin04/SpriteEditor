#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QColor>
#include <QVector>
#include "canvas.h"

/// Reviewed by: Ethan Heathcote
Canvas::Canvas(QWidget *parent)
    : QWidget{parent}
{
    setMouseTracking(true);
    setGridSize(64);
}

void Canvas::mousePressEvent(QMouseEvent *event){
    // Reports left clicks to the model
    if (event->button() == Qt::LeftButton){
        emit canvasClickSignal(event->pos().x(), event->pos().y(), true);
    }
}

void Canvas::mouseReleaseEvent(QMouseEvent *event){
    // Reports left click releases to the model
    if (event->button() == Qt::LeftButton){
        emit canvasClickSignal(event->pos().x(), event->pos().y(), false);
    }
}

void Canvas::mouseMoveEvent(QMouseEvent *event){
    // Reports mouse movements to the model
    emit canvasMoveSignal(event->pos().x(), event->pos().y(), false);
}

void Canvas::leaveEvent(QEvent *event){
    // Reports when the user is no longer hovering over the canvas
    emit canvasMoveSignal(0, 0, true);
}

void Canvas::setGridSize(int pixelDimension){
    // Does integer math to determine the canvas size
    this->pixelDimension = pixelDimension;
    pixelSize = (640 / pixelDimension);
    canvasSize = pixelSize * pixelDimension;
    pixels.clear();
    // Initializes the canvas to be transparent
    for(int i = 0; i < (pixelDimension * pixelDimension); i++){
        pixels.push_back(QColor(0, 0, 0, 0));
    }
}


void Canvas::paintEvent(QPaintEvent *event){
    QPainter painter(this);

    int currentX = 0;
    int currentY = 0;

    //Draws the canvas based on the vector of colors
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
