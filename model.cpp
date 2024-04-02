#include "model.h"

Model::Model(QObject *parent)
    : QObject{parent}, size(0), fps(0), activeSwatch(0), imageIndex(0), imageIndexCurrent(0), drawing(false), trueSizePreview(false),
    currentColor(0, 0, 255, 255), eraserColor(0, 0, 0 ,0), currentTool(paint)

{
    QColor defaultSwatch(0,0,0);
    for(int i = 0; i < 6; i++) {
        swatches[i] = defaultSwatch;
    }
    swatches[activeSwatch] = currentColor;
    connect(&tick, &QTimer::timeout, this, &Model::generatePreview);
}

void Model::newCanvas(int size){
    lock.lock();
    frames.clear();
    this->size = size;
    updateFPS(0);
    lock.unlock();
    addFrame();
}

void Model::newProjectPressed(){
    emit messageBox();
}

void Model::messageYesSelected(){
    tick.stop();
    frames.clear();
    imageIndex = 0;
    images = frames;
    emit projectReset();
}

void Model::toolToPaint(){
    detoggleActiveButton(paint);
    emit updateColorPreview(getStyleString(currentColor));
    emit updateColorSliders(currentColor);
    swatches[activeSwatch] = currentColor;
    emit fillSwatch(activeSwatch, getStyleString(currentColor));
    emit toggleBrush(true);
}

void Model::toolToEraser(){
    detoggleActiveButton(eraser);
    emit updateColorSliders(QColor(0, 0, 0, 0));
    emit toggleEraser(true);
}

void Model::toolToDropper(){
    detoggleActiveButton(dropper);
    emit togglePicker(true);
}

void Model::detoggleActiveButton(Tool tool){
    if (tool == currentTool){
        return;
    }
    switch(currentTool){
    case paint:
        emit toggleBrush(false);
        break;
    case eraser:
        emit toggleEraser(false);
        break;
    case dropper:
        emit togglePicker(false);
        break;
    default:
        break;
    }

    currentTool = tool;
}

void Model::undoAction(){
    lock.lock();
    emit sendFrameToCanvas(currentFrame->undoAction());
    emit setFrameSelectorIcon(currentFrame->generateImage(), currentFrame->ID, currentFrame->ID);
    updateImageVector();
    lock.unlock();
}

void Model::cloneButton(){
    lock.lock();
    Frame clonedFrame(size);
    clonedFrame.setPixels(currentFrame->getPixels());
    frames.push_back(clonedFrame);
    int newFrameIndex = frames.indexOf(clonedFrame);
    //Used to track which box is highlighted and unhighlighted
    int lastFrameID = currentFrame->ID;
    currentFrame = &frames[newFrameIndex];
    updateImageVector();
    emit createFrameSelectorButton(currentFrame->ID);
    emit sendFrameToCanvas(currentFrame->getPixels());
    emit setFrameSelectorIcon(currentFrame->generateImage(), currentFrame->ID, lastFrameID);
    lock.unlock();
}

void Model::addFrame(){
    lock.lock();
    Frame newFrame(size);
    frames.push_back(newFrame);
    int newFrameIndex = frames.indexOf(newFrame);
    int lastFrameID = frames.size() > 1 ? currentFrame->ID: 0;
    currentFrame = &frames[newFrameIndex];
    updateImageVector();

    emit createFrameSelectorButton(currentFrame->ID);
    emit sendFrameToCanvas(currentFrame->getPixels());
    emit setFrameSelectorIcon(currentFrame->getImage(), currentFrame->ID, lastFrameID);
    lock.unlock();
}

void Model::removeFrame(){
    lock.lock();
    int toBeDeletedID = currentFrame->ID;
    int toBeDeletedIndex = frames.indexOf(*currentFrame);

    // Resets the index of the image being previewed to avoid out of bounds rendering occurring
    frames.removeAt(toBeDeletedIndex);

    if (frames.size() == 0) {
        lock.unlock();
        addFrame();
        emit deleteFrameSelectorButton(toBeDeletedID);
        emit setFrameSelectorIcon(currentFrame->getImage(), currentFrame->ID, toBeDeletedID);
        return;
    }

    // Default action is advance to next available frame when a frame is deleted
    // If the last frame is deleted, the first frame in the sequence is toggled
    if (toBeDeletedIndex >= frames.size()){
        currentFrame = &frames[0];
    } else {
        currentFrame = &frames[toBeDeletedIndex];
    }

    emit deleteFrameSelectorButton(toBeDeletedID);
    emit setFrameSelectorIcon(currentFrame->getImage(), currentFrame->ID, toBeDeletedID);
    emit sendFrameToCanvas(currentFrame->getPixels());
    updateImageVector();
    lock.unlock();
}

void Model::changeFrame(int ID){
    //  Used for highlighting active frame in frame selector preview
    lock.lock();
    int lastFrameID = currentFrame->ID;

    for (int i = 0; i < frames.size(); i++){
        if (ID == frames[i].ID){
            currentFrame = &frames[i];
            updateImageVector();
            emit sendFrameToCanvas(currentFrame->getPixels());
            emit setFrameSelectorIcon(currentFrame->generateImage(), currentFrame->ID, lastFrameID);
            lock.unlock();
            return;
        }
    }
    lock.unlock();
}

void Model::updateFPS(int fps){
    tick.stop();
    this->fps = fps;
    if (fps == 0){
        tick.setInterval(double(50));
        tick.start();
        return;
    }
    tick.setInterval(double(1000 / fps));
    tick.start();
}

void Model::updateImageVector(){
    tick.stop();
    images = frames;
    imageIndexCurrent = images.indexOf(*currentFrame);

    for(int i = 0; i < images.size(); i++){
        images[i].generateImage();
    }

    tick.start();
}

void Model::generatePreview(){
    if(images.length() == 0){
        return;
    }

    if(fps == 0){
        emit sendImageToPreview(images[imageIndexCurrent].getImage(), trueSizePreview, size);
        return;
    }

    emit sendImageToPreview(images[imageIndex++].getImage(), trueSizePreview, size);

    imageIndex = imageIndex >= images.size() ? 0 : imageIndex;
}

void Model::togglePreviewSize(){
    //Toggles between true size and larger size playback
    tick.stop();
    trueSizePreview = !trueSizePreview;
    tick.start();
}

void Model::canvasClick(int x, int y, bool click){
    lock.lock();
    drawing = click;

    currentFrame->addToHistory(currentFrame->getPixels());

    if(!drawing){
        lock.unlock();
        return;
    }


    switch(currentTool){
    case paint:
        draw(x, y, currentColor);
        lock.unlock();
        return;
    case eraser:
        draw(x, y, eraserColor);
        lock.unlock();
        return;
    case dropper:
        currentColor = currentFrame->getPixelColor(x, y);
        emit updateColorPreview(getStyleString(currentColor));
        emit updateColorSliders(currentColor);
        swatches[activeSwatch] = currentColor;
        emit fillSwatch(activeSwatch, getStyleString(currentColor));
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
    lock.lock();

    if(offCanvas) {
        emit sendFrameToCanvas(currentFrame->getPixels());
        lock.unlock();
        return;
    }

    switch(currentTool){
    case paint:
        if (drawing){
            draw(x, y, currentColor);
            lock.unlock();
            return;
        }
        emit sendFrameToCanvas(currentFrame->addTemporaryPixel(x, y, currentColor));
        lock.unlock();
        return;
    case eraser:
        if (drawing){
            draw(x, y, eraserColor);
            lock.unlock();
            return;
        }
        emit sendFrameToCanvas(currentFrame->addTemporaryPixel(x, y, QColor(0, 0, 0, 0)));
        lock.unlock();
        return;
    case dropper:
        if (drawing){
            lock.unlock();
            return;
        }
        emit updateColorSliders(currentFrame->getPixelColor(x, y));
        emit updateColorPreview(getStyleString(currentFrame->getPixelColor(x, y)));
        emit sendFrameToCanvas(currentFrame->getPixels());
        lock.unlock();
        return;
    default:
        emit errorOccurred("no tool selected");
        lock.unlock();
        return;
    }
    lock.unlock();
}

void Model::draw(int x, int y, QColor color){
    emit sendFrameToCanvas(currentFrame->addNewPixel(x, y, color));
    updateImageVector();
    emit setFrameSelectorIcon(currentFrame->getImage(), currentFrame->ID, currentFrame->ID);
}

void Model::addSwatch(int swatchNumber) {
    detoggleActiveButton(paint);
    activeSwatch = swatchNumber;
    currentColor = swatches[activeSwatch];
    emit toggleBrush(true);
    emit updateColorSliders(swatches[activeSwatch]);
    emit updateColorPreview(getStyleString(swatches[activeSwatch]));
    emit fillSwatch(activeSwatch, getStyleString(currentColor));
}

void Model::colorChanged(Color color, int value) {
    if(currentTool == dropper || currentTool == eraser){
        return;
    }

    if (color == Color::red){
        currentColor.setRed(value);
    }
    else if (color == Color::green){
        currentColor.setGreen(value);
    }
    else if (color == Color::blue){
        currentColor.setBlue(value);
    }
    else if (color == Color::alpha){
        currentColor.setAlpha(value);
    }
    swatches[activeSwatch] = currentColor;
    emit fillSwatch(activeSwatch, getStyleString(currentColor));
    emit updateColorPreview(getStyleString(currentColor));
}

QString Model::getStyleString(QColor color){
    return QString("background-color: rgba("
                   + QString::number(color.red()) + ","
                   + QString::number(color.green()) + ","
                   + QString::number(color.blue()) + ","
                   + QString::number(color.alpha()) + ");");
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
    lock.lock();

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        emit errorOccurred("File failed to open");
        lock.unlock();
        return;
    }
    QByteArray fileContent = file.readAll();
    file.close();
    QJsonDocument doc = QJsonDocument::fromJson(fileContent);
    if (doc.isNull() || !doc.isObject()) {
        emit errorOccurred("Invalid JSON format");
        lock.unlock();
        return;
    }
    QJsonObject json = doc.object();
    size = json["pixelDimension"].toInt();
    QVector<Frame> newFrames;
    QJsonArray framesArray = json["frames"].toArray();
    for (const auto& frameValue : framesArray) {
        Frame frame(size);
        frame.clearHistory();
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
        frame.addToHistory(framePixels);
    }
    frames.clear();
    int i = 0;
    // Used to track which box is highlighted in frame preview icons as they are loaded in
    int prevFrame = 0;
    for(auto frame: newFrames){
        frames.insert(i, frame);
        frames[i].generateImage();
        emit createFrameSelectorButton(frames[i].ID);
        emit setFrameSelectorIcon(frames[i].generateImage(), frame.ID, prevFrame);
        prevFrame = frame.ID;
        i++;
    }

    emit resizeCanvas(size);
    images = frames;
    imageIndex = 0;
    currentFrame = &frames[0];
    emit setFrameSelectorIcon(frames[0].generateImage(), currentFrame->ID, prevFrame);
    lock.unlock();
}
