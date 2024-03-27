#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include "frame.h"

class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = nullptr);

signals:
    void sendFrameToCanvas(QVector<QColor>);

public slots:
    void canvasClick(int x, int y, bool click);
    void canvasMovement(int x, int y, bool offCanvas);
    void colorChanged(QString color, int value);
private:
    Frame currentFrame;
    bool drawing;
    QColor currentColor;
};

#endif // MODEL_H
