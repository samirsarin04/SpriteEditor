///@brief The .cpp file for the frame preview button for this application. gives method definitions for all of the members
///Reviewed by: Arjun Sarkar

#include "framepreviewbutton.h"

FramePreviewButton::FramePreviewButton(int ID,QWidget *parent)
    : QLabel{parent}
{
    this->ID = ID;
}

void FramePreviewButton::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton){
        emit frameClicked(ID);
    }
}
