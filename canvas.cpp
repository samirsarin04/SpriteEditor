#include "canvas.h"

#include <iostream>

Canvas::Canvas(QWidget *parent)
    : QWidget{parent}, canvasSize(0)
{
    // pixelArray.push_back(255);
    // pixelArray.push_back(0);
    // pixelArray.push_back(0);
    // pixelArray.push_back(255);

    // pixelArray.push_back(255);
    // pixelArray.push_back(255);
    // pixelArray.push_back(0);
    // pixelArray.push_back(255);

    // pixelArray.push_back(255);
    // pixelArray.push_back(0);
    // pixelArray.push_back(255);
    // pixelArray.push_back(255);

    // pixelArray.push_back(255);
    // pixelArray.push_back(255);
    // pixelArray.push_back(255);
    // pixelArray.push_back(255);
    // setGridSize(2);
     canvasSize = 0;
}

void Canvas::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton){

       // std::cout << "x " << event->pos().x() << std::endl;
      //  std::cout << "y " << event->pos().y() << std::endl;
       //QVector<int> blank;


        //pixelArray = blank;
       // updateCanvas(blank);
        emit canvasClickSignal(event->pos().x(), event->pos().y(), true);
    }
}

void Canvas::mouseReleaseEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton){
       // std::cout << "x " << event->pos().x() << std::endl;
       // std::cout << "y " << event->pos().y() << std::endl;
        emit canvasClickSignal(event->pos().x(), event->pos().y(), false);
    }
}

void Canvas::setGridSize(int size){
    this->size = size;
    canvasSize = size * size * 4;
    pixelSize = 640 / size;
}


void Canvas::paintEvent(QPaintEvent *event){
    QPainter painter(this);
   // QRect rec(0, 0, 640, 640);
 std::cout << "PAINT EVENT" << std::endl;
   // painter.fillRect(rec, QColor(255, 0, 255, 255));


    int currentX = 0;
    int currentY = 0;

   // std::cout << "UdPATE" << std::endl;
    //std::cout << pixelArray[0] << std::endl;

    for (int i = 0; i < canvasSize; i+=4){
        //painter.fillRect(QRect(currentX, currentY, pixelSize, pixelSize), QColor(pixelArray.at(i), pixelArray.at(i + 1), pixelArray.at(i + 2), pixelArray.at(i + 3)));
       // currentX = currentX >= (640 - pixelSize) ? 0 : currentX + pixelSize;
      //  currentY = currentX >= (640 - pixelSize) ? currentY + pixelSize : currentY;
    }
}

void Canvas::updateCanvas(QVector<int> pixels){
   // pixelArray = pixels;
   // canvasSize = 16;
    update();
}
