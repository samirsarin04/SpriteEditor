#include "framepreviewbutton.h"

FramePreviewButton::FramePreviewButton(QWidget *parent)
    : QLabel{parent}
{
    qDebug() << "Hello!";
}

void FramePreviewButton::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton){
        //emit canvasClickSignal(event->pos().x(), event->pos().y(), true);
    }
}
