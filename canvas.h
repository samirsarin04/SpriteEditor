#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QColor>
#include <QVector>


class Canvas : public QWidget
{
    Q_OBJECT
private:
    int canvasSize;
    int size;
    QVector<int> pixelArray;
    int pixelSize;
public:
    explicit Canvas(QWidget *parent = nullptr);
    void setGridSize(int size);
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
   // void mouseMoveEvent(QMouseEvent *event) override;
signals:
    void canvasClickSignal(int x, int y, bool click);
public slots:
    void updateCanvas(QVector<int> pixels);
};

#endif // CANVAS_H
