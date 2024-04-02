#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QColor>
#include <QVector>

///@brief The header file for the view for this application. gives forward declarations for all of the members
/// Reviewed by: Ethan Heathcote
class Canvas : public QWidget
{
    Q_OBJECT
private:
    // The vector that holds the pixel values
    QVector<QColor> pixels;
    // The adjusted size of the canvas that fits to the selected pixel dimension
    int canvasSize;
    // The number pixels that each drawn pixel takes up
    int pixelSize;
    // The dimension selected by the user i.e. 64 x 64 px, would hold a value of 64
    int pixelDimension;

public:
    ///@brief The header file for the view for this application. gives forward declarations for all of the members
    /// @param parent set to null ptr for initialization
    explicit Canvas(QWidget *parent = nullptr);
    ///@brief Allows the canvas to dynamically resize itself
    /// @param size represents the width and height of the new dimension
    void setGridSize(int size);
protected:
    ///@brief Sends a signal to the model with the x and y coordinates when a click occurs
    /// @param event the mouse event passed to the method
    void mousePressEvent(QMouseEvent *event) override;
    ///@brief Sends a signal to the model with the x and y coordinates when a click release occurs
    /// @param event the mouse event passed to the method
    void mouseReleaseEvent(QMouseEvent *event) override;
    ///@brief redraws the canvas when a QPaintEvent occurs
    /// @param event the mouse event passed to the method
    void paintEvent(QPaintEvent *event) override;
    ///@brief Sends a signal to the model with the x and y coordinates when a mouse movement occurs on the canvas
    /// @param event the mouse event passed to the method
    void mouseMoveEvent(QMouseEvent *event) override;
    ///@brief Sends a signal to the model the mouse leaves the canvas
    /// @param event the mouse event passed to the method
    void leaveEvent(QEvent *event) override;
signals:
    ///@brief Signal for reporting canvas clicks to the model
    /// @param x the x coordinate send to the model
    /// @param y the y coordinate send to the model
    /// @param click bool that represents a click (T) or a click release (F)
    void canvasClickSignal(int x, int y, bool click);
    ///@brief Signal for reporting canvas mouse movements to the model
    /// @param x the mouse event passed to the method
    /// @param y the mouse event passed to the method
    /// @param offCanvas bool that represents when the user is hovering over the canvas or not
    void canvasMoveSignal(int x, int y, bool offCanvas);
public slots:
    ///@brief updates the color of the pixels that the canvas will draw
    /// @param pixels The vector of colors that the canvas will draw
    void updateCanvas(QVector<QColor> pixels);
};

#endif // CANVAS_H
