#include "model.h"
#include <QDebug>
#include<iostream>


Model::Model(QObject *parent)
    : QObject{parent}, drawing(false), currentColor(255, 0, 255, 255), activeSwatch(0)
{
    // initial tool
    currentTool = paint;
    QColor defaultSwatch(0,0,0);
    for(int i = 0; i < 6; i++) {
        swatches[i] = defaultSwatch;
    }

    imageIndex = 0;
    size = 0;
    swatches[activeSwatch] = currentColor;
    fps = 0;
    //Frame::ID = 0;

    connect(&tick, &QTimer::timeout, this, &Model::generatePreview);
}

void Model::canvasClick(int x, int y, bool click){
    lock.lock();
    drawing = click;
    //if we are not drawing and the first brush stroke is waiting to be made
    if(drawing == false && currentFrame->getFirstStroke()){
        currentFrame->addToHistory(currentFrame->getPixels());
        currentFrame->setFirstStroke(false);
    }
    //add the frame as we start drawing to the frame's history stack
    else if(drawing == true){
        currentFrame->addToHistory(currentFrame->getPixels());
    }
    switch(currentTool){
    case paint:
        emit sendFrameToCanvas(currentFrame->addNewPixel(x, y, currentColor));
        images = frames;
        emit setImageIcon(currentFrame->getImage(), currentFrame->ID, currentFrame->ID);
        lock.unlock();
        return;
    case eraser:
        emit sendFrameToCanvas(currentFrame->addNewPixel(x, y, QColor(0, 0, 0, 0)));
        images = frames;
        emit setImageIcon(currentFrame->getImage(), currentFrame->ID, currentFrame->ID);
        lock.unlock();
        return;
    case dropper:
        currentColor = currentFrame->getPixelColor(x, y);
        emit updateColorPreview(getStyleString(currentColor));
        emit updateColorSliders(currentColor);
        swatches[activeSwatch] = currentColor;
        emit fillSwatch(activeSwatch+1, getStyleString(currentColor));
        toolToPaint();
        lock.unlock();
        return;
    default:
      lock.unlock();
        return;
    }
   lock.unlock();
}

void Model::canvasMovement(int x, int y, bool offCanvas){
    if(offCanvas) {
        emit sendFrameToCanvas(currentFrame->getPixels());
        return;
    }

    if (drawing){
        switch(currentTool){
        case paint:
            emit sendFrameToCanvas(currentFrame->addNewPixel(x, y, currentColor));
           // images.insert(currentFrame->ID, currentFrame->getImage());
            images = frames;
            emit setImageIcon(currentFrame->getImage(), currentFrame->ID, currentFrame->ID);
            return;
        case eraser:
            emit sendFrameToCanvas(currentFrame->addNewPixel(x, y, QColor(0, 0, 0, 0)));
            images = frames;
            emit setImageIcon(currentFrame->getImage(), currentFrame->ID, currentFrame->ID);
           // images.insert(currentFrame->ID, currentFrame->getImage());
             return;
        case dropper:
            return;
        default:
            emit errorOccurred("no tool selected");
            return;
        }
    }

    if(currentTool == dropper){
        emit updateColorSliders(currentFrame->getPixelColor(x, y));
        emit updateColorPreview(getStyleString(currentFrame->getPixelColor(x, y)));
        emit sendFrameToCanvas(currentFrame->getPixels());
        return;
    } else if (currentTool == eraser){
        emit sendFrameToCanvas(currentFrame->addTemporaryPixel(x, y, QColor(0, 0, 0, 0)));
        return;
    }

    emit sendFrameToCanvas(currentFrame->addTemporaryPixel(x, y, currentColor));
}

void Model::colorChanged(QString color, int value) {
    if(currentTool == dropper || currentTool == eraser){
        return;
    }

    if (color == "red"){
        currentColor.setRed(value);
    }
    else if (color == "green"){
        currentColor.setGreen(value);
    }
    else if (color == "blue"){
        currentColor.setBlue(value);
    }
    else if (color == "alpha"){
        currentColor.setAlpha(value);
    }
    swatches[activeSwatch] = currentColor;
    emit fillSwatch(activeSwatch+1, getStyleString(currentColor));
    emit updateColorPreview(getStyleString(currentColor));
}

void Model::newCanvas(int size){
    frames.clear();
    this->size = size;
    addFrame();
    updateFPS(0);

}

void Model::updateFPS(int fps){
    tick.stop();
    this->fps = fps;
    if (fps == 0){
        tick.setInterval(double(100));
        tick.start();
        return;
    }
    tick.setInterval(double(1000 / fps));
    tick.start();
}

void Model::generatePreview(){
  // imgLock.lock();
    if(images.length() == 0){
       // qDebug() << "empty images reported";
      //  imgLock.unlock();
        return;
    }


   // lock.lock();
    // SOME LOGIC TO TRACK WHICH IMAGE TO SHOW
    if(fps == 0 || images.length() < 2){
        // qDebug() << "NO FPS";
       //  qDebug() << "0 val triggered/ reported";
      //  QImage temp = currentFrame->generateImage();
       // lock.unlock();
        emit sendImage(currentFrame->getImage());
     //   imgLock.unlock();
        return;
    }

    emit sendImage(images[imageIndex++].getImage());
    //qDebug() << images.size();
    if (imageIndex >= images.size()){
        //qDebug() << "RESET";
        imageIndex = 0;
    }
   // lock.unlock();
    //images
    //imageIndex++;
    //QImage temp = frames[imageIndex%frames.size()].generateImage();
   // lock.unlock();
    //emit sendImage(temp);
   // imgLock.unlock();
}

void Model::toolToPaint(){
    detoggleActiveButton(paint);
    currentTool = paint;
    emit updateColorPreview(getStyleString(currentColor));
    emit updateColorSliders(currentColor);
    swatches[activeSwatch] = currentColor;
    emit fillSwatch(activeSwatch+1, getStyleString(currentColor));
    emit toggleBrush(true);
}

void Model::toolToEraser(){
    detoggleActiveButton(eraser);
    currentTool = eraser;
    emit updateColorSliders(QColor(0, 0, 0, 0));
    emit toggleEraser(true);
}

void Model::toolToDropper(){
    detoggleActiveButton(dropper);
    currentTool = dropper;
    emit togglePicker(true);
}

void Model::swatch1Clicked(){
    addSwatch(0);
}

void Model::swatch2Clicked(){
    addSwatch(1);
}

void Model::swatch3Clicked(){
    addSwatch(2);
}

void Model::swatch4Clicked(){
    addSwatch(3);
}

void Model::swatch5Clicked(){
    addSwatch(4);
}

void Model::swatch6Clicked(){
    addSwatch(5);
}

void Model::addSwatch(int swatchNumber) {
    detoggleActiveButton(paint);
    currentTool = paint;
    activeSwatch = swatchNumber;
    currentColor = swatches[activeSwatch];
    emit toggleBrush(true);
    emit updateColorSliders(swatches[activeSwatch]);
    emit updateColorPreview(getStyleString(swatches[activeSwatch]));
    emit fillSwatch(activeSwatch+1, getStyleString(currentColor));
}

QString Model::getStyleString(QColor color){
    return QString("background-color: rgba("
                   + QString::number(color.red()) + ","
                   + QString::number(color.green()) + ","
                   + QString::number(color.blue()) + ","
                   + QString::number(color.alpha()) + ");");
}

void Model::undoAction(){
    emit sendFrameToCanvas(currentFrame->undoAction());
    emit setImageIcon(currentFrame->generateImage(), currentFrame->ID, currentFrame->ID);
    images = frames;
}
void Model::detoggleActiveButton(Tool tool){
    if (tool == currentTool){
        return;
    }
    switch(currentTool){
    case paint:
        emit toggleBrush(false);
        return;
    case eraser:
        emit toggleEraser(false);
        return;
    case dropper:
        emit togglePicker(false);
        return;
    default:
        break;
    }
}

void Model::savePressed(QString& filename) {
    if (size < 2){
        return;
    }
    QJsonObject json;
    json["pixelDimension"] = size;
    QJsonArray framesArray;
    for (Frame& frame : frames) {
        QJsonObject frameObject;
        QJsonArray rgbaValues;
        for (const QColor& color : frame.getPixels()) {
            rgbaValues.append(color.red());
            rgbaValues.append(color.green());
            rgbaValues.append(color.blue());
            rgbaValues.append(color.alpha());
        }
        frameObject["RGBAValues"] = rgbaValues;
        framesArray.append(frameObject);
    }
    json["frames"] = framesArray;

    QFile file(filename);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(json).toJson());
        file.close();
    } else {
        emit errorOccurred("File failed to save");
    }
}


void Model::loadPressed(QString& filename) {
   // tick.stop();

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        emit errorOccurred("File failed to open");
        return;
    }
    QByteArray fileContent = file.readAll();
    file.close();
    QJsonDocument doc = QJsonDocument::fromJson(fileContent);
    if (doc.isNull() || !doc.isObject()) {
        emit errorOccurred("Invalid JSON format");
        return;
    }
    QJsonObject json = doc.object();
    size = json["pixelDimension"].toInt();
    QVector<Frame> newFrames;
    QJsonArray framesArray = json["frames"].toArray();
    for (const auto& frameValue : framesArray) {
        Frame frame(size);
        QVector<QColor> framePixels;
        QJsonArray rgbaValues = frameValue.toObject()["RGBAValues"].toArray();
        for (int i = 0; i < rgbaValues.size(); i += 4) {
            int red = rgbaValues[i].toInt();
            int green = rgbaValues[i + 1].toInt();
            int blue = rgbaValues[i + 2].toInt();
            int alpha = rgbaValues[i + 3].toInt();
            QColor color(red, green, blue, alpha);
            framePixels.append(color);
        }
        frame.setPixels(framePixels);
        newFrames.append(frame);
    }
   // lock.lock();
    frames.clear();
    int i = 0;
    int prevFrame = 0;
    for(Frame frame: newFrames){
        frames.insert(i, frame);
        frames[i].generateImage();
        emit createPreviewButton(frames[i].ID);
        emit setImageIcon(frames[i].generateImage(), frame.ID, prevFrame);
        prevFrame = frame.ID;
        i++;
    }

    emit resizeCanvas(size);
    //imgLock.lock();
    images = frames;
    imageIndex = 0;
  //  imgLock.unlock();

    currentFrame = &frames[0];
    emit setImageIcon(frames[0].generateImage(), currentFrame->ID, prevFrame);
  //  lock.unlock();
   // tick.start();
}

void Model::newProjectPressed(){
    emit messageBox();
}

void Model::messageYesSelected(){
    tick.stop();
    //clear QImages once implemented
    frames.clear();
    imageIndex = 0;
    images = frames;
    emit projectReset();
}

void Model::displayFrame(){
    // qDebug() << "SHOULD NOT BE CALLED YET";
    // bool flag;
    // int frameIndex = sender()->objectName().toInt(&flag);
    // if(!flag){
    //     //conversion failed
    // }
    // //set current frame to frameIndex

    // currentFrame = &frames[frameIndex];

}

void Model::cloneButton(){
   Frame temp(size);
   temp.setPixels(currentFrame->getPixels());
   frames.push_back(temp);
   int tempID = frames.indexOf(temp);
  // qDebug() << tempID << ": TEMP ID CREATED";
  //  lock.unlock();
   int prevID = currentFrame->ID;
   currentFrame = &frames[tempID];
  images = frames;
  emit createPreviewButton(temp.ID);
  emit sendFrameToCanvas(currentFrame->getPixels());
  emit setImageIcon(currentFrame->generateImage(), currentFrame->ID, prevID);

}

void Model::addFrame(){
    //add lock
  //  lock.lock();
    Frame temp(size);
    frames.push_back(temp);
    int tempID = frames.indexOf(temp);
    int prevID = frames.size() > 1 ? currentFrame->ID: 0;
    currentFrame = &frames[tempID];
   //imgLock.lock();
    images = frames;
  // imgLock.unlock();
    emit createPreviewButton(temp.ID);
    emit sendFrameToCanvas(currentFrame->getPixels());
    emit setImageIcon(currentFrame->getImage(), currentFrame->ID, prevID);
  //  lock.unlock();
}

void Model::removeFrame(){
    tick.stop();
   int tempID = currentFrame->ID;
   int start = frames.indexOf(*currentFrame);
   imageIndex = 0;

   frames.removeAt(start);
  // lock.lock();
   if (frames.size() == 0) {
       emit deleteFrame(tempID);
       addFrame();
       tick.start();
       emit setImageIcon(currentFrame->getImage(), currentFrame->ID, tempID);
       return;
   }
   //lock.unlock();

   if (start >= frames.size()){
       currentFrame = &frames[0];
   } else {
       currentFrame = &frames[start];
   }
   images = frames;
   emit deleteFrame(tempID);
   emit setImageIcon(currentFrame->getImage(), currentFrame->ID, tempID);
   emit sendFrameToCanvas(currentFrame->getPixels());
   tick.start();
}

void Model::changeFrame(int ID){
    int tempID = currentFrame->ID;
    for (int i = 0; i < frames.size(); i++){
        if (ID == frames[i].ID){
            currentFrame = &frames[i];
            emit sendFrameToCanvas(currentFrame->getPixels());
            emit setImageIcon(currentFrame->generateImage(), currentFrame->ID, tempID);
            return;
        }
    }
   // qDebug() << "Frame Request";
}
