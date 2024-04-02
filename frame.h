#ifndef FRAME_H
#define FRAME_H

#include <QVector>
#include <QColor>
#include <stack>
#include <QImage>
#include<mutex>

///@brief Header file for the Frame class. represents a frame in the view
///
/// Reviewed by: Samir Sarin
class Frame
{
private:

    ///
    /// @brief pixels holds the colors of all the current pixels
    QVector<QColor> pixels;

    ///
    /// @brief canvasSize is the size of the canvas
    int canvasSize;

    ///
    /// @brief pixelSize to be shown on the canvas
    int pixelSize;

    ///
    /// @brief pixelDimension holds the dimensions of a single pixel
    int pixelDimension;

    ///
    /// @brief modifyPixel changes the color of the pixel at x and y
    /// @param pixels
    /// @param x location in the canvas
    /// @param y location in the canvas
    /// @param color that is going to be set
    void modifyPixel(QVector<QColor> &pixels, int x, int y, QColor color);
    std::stack<QVector<QColor>> history;
    QImage frameImage;

public:
    ///
    /// @brief Frame constructor
    /// @param pixelDimension
    Frame(int pixelDimension);

    ///
    /// @brief addNewPixel to the fram
    /// @param x location on the canvas
    /// @param y location on the canvas
    /// @param color
    /// @return the color of the new pixels
    QVector<QColor> addNewPixel(int x, int y, QColor color);

    ///
    /// @brief getPixels
    /// @return returns the pixels
    QVector<QColor> getPixels();

    ///
    /// @brief addTemporaryPixel
    /// @param x
    /// @param y
    /// @param color
    /// @return
    QVector<QColor> addTemporaryPixel(int x, int y, QColor color);

    ///
    /// @brief getPixelColor returns the current color of the given pixel
    /// @param x location on the canvas
    /// @param y location on the canvas
    /// @return the current color
    QColor getPixelColor(int x, int y);

    ///
    /// @brief undoAction reverts back to the last vector in the history
    /// @return the last vector of all the pixels
    QVector<QColor> undoAction();

    ///
    /// @brief addToHistory adds the action that was just made to the history
    /// @param pixels the current vector of all the pixels
    void addToHistory(QVector<QColor> pixels);

    ///
    /// @brief clearHistory clears the history of the frame
    void clearHistory();

    ///
    /// @brief generateImage generates an image and calls set pix
    /// @return the generated image
    QImage generateImage();

    ///
    /// @brief getImage
    /// @return the image of the frame
    QImage getImage();

    ///
    /// @brief setPixels sets the pixels on the frame
    /// @param newPixels is a list of the new Pixels to be added
    void setPixels(QVector<QColor> newPixels);
    int ID;
    static int globalID;

    /// @brief operator == copy constructor
    /// @param other the other frame
    /// @return if the two frames are e
    bool operator==(const Frame& other) const;
};

#endif // FRAME_H
