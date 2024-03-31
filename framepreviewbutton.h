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
    explicit FramePreviewButton(QWidget *parent = nullptr);

signals:
};

#endif // FRAMEPREVIEWBUTTON_H
