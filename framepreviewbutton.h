///@brief The header file for the frame preview button for this application. gives forward declarations for all of the members
///Reviewed by: Arjun Sarkar

#ifndef FRAMEPREVIEWBUTTON_H
#define FRAMEPREVIEWBUTTON_H

#include <QLabel>
#include <QMouseEvent>

class FramePreviewButton : public QLabel
{
    Q_OBJECT

protected:
    ///@brief Emits signal when left-clicked
    ///@param event
    void mousePressEvent(QMouseEvent *event) override;

public:
    ///@brief Constructor for FramePreviewButton
    ///@param ID: id of corresponding frame
    ///@param parent: qwidget that it resides in
    explicit FramePreviewButton(int ID,QWidget *parent = nullptr);

private:
    int ID;

signals:
    void frameClicked(int ID);
};

#endif // FRAMEPREVIEWBUTTON_H
