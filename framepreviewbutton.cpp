#include "framepreviewbutton.h"

FramePreviewButton::FramePreviewButton(int ID,QWidget *parent)
    : QLabel{parent}
{
    this->ID = ID;
   // qDebug() << ID;
}

void FramePreviewButton::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton){
        emit frameClicked(ID);
    }
}
