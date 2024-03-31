#ifndef FRAMEPREVIEWBUTTON_H
#define FRAMEPREVIEWBUTTON_H

#include <QLabel>
#include <QMouseEvent>

class FramePreviewButton : public QLabel
{
    Q_OBJECT

protected:
    void mousePressEvent(QMouseEvent *event) override;

public:
    explicit FramePreviewButton(int ID,QWidget *parent = nullptr);

private:
    int ID;

signals:
    void frameClicked(int ID);
};

#endif // FRAMEPREVIEWBUTTON_H
