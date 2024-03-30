#include "view.h"
#include "ui_view.h"
#include "model.h"
#include "button.h"

View::View(Model &model, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::View)
{
    ui->setupUi(this);
    this->setStyleSheet("background-color: rgb(60, 60, 60)");
    ui->canvas->setVisible(false);

    //Set Color Preview to default value
    ui->colorPreview->setStyleSheet("background-color: rgb(255, 0, 255)");
    connect(&model, &Model::updateColorPreview, this, &View::updateColorPreview);

    //Set Tool Icons
    ui->brushButton->setIcon(QIcon(":/toolBarIcons/paintBrushIcon"));
    ui->eraseButton->setIcon(QIcon(":/toolBarIcons/eraseIcon"));
    ui->dropperButton->setIcon(QIcon(":/toolBarIcons/dropperIcon"));
    ui->cloneButton->setIcon(QIcon(":/toolBarIcons/cloneIcon"));
    ui->undoButton->setIcon(QIcon(":/toolBarIcons/undoIcon"));

    //Set brush to active tool visually
    ui->brushButton->setStyleSheet("background-color: rgba(255, 255, 255, 255);\nwidth: 40px;\nheight: 40px;\nmargin-left: auto;\nmargin-right: auto;");

    //Tool Signal/Slots
    connect(&model, &Model::toggleBrush, this, &View::setBrush);
    connect(&model, &Model::toggleEraser, this, &View::setEraser);
    connect(&model, &Model::togglePicker, this, &View::setPicker);

    //set all swatches to black on initialization
    ui->swatch1->setStyleSheet("background-color: rgb(255, 0, 255)");
    ui->swatch2->setStyleSheet("background-color: rgb(0, 0, 0)");
    ui->swatch3->setStyleSheet("background-color: rgb(0, 0, 0)");
    ui->swatch4->setStyleSheet("background-color: rgb(0, 0, 0)");
    ui->swatch5->setStyleSheet("background-color: rgb(0, 0, 0)");
    ui->swatch6->setStyleSheet("background-color: rgb(0, 0, 0)");

    //Canvas Signals/Slots
    connect(ui->canvas, &Canvas::canvasClickSignal, &model, &Model::canvasClick);
    connect(&model, &Model::sendFrameToCanvas, ui->canvas, &Canvas::updateCanvas);
    connect(ui->canvas, &Canvas::canvasMoveSignal, &model, &Model::canvasMovement);

    //Update Color signal/slot
    connect(this, &View::colorValueChanged, &model, &Model::colorChanged);

    //Toolbar buttons signals/slots
    connect(ui->brushButton, &QPushButton::clicked, &model, &Model::toolToPaint);
    connect(ui->eraseButton, &QPushButton::clicked, &model, &Model::toolToEraser);
    connect(ui->dropperButton, &QPushButton::clicked, &model, &Model::toolToDropper);
    connect(ui->undoButton, &QPushButton::clicked, &model, &Model::undoAction);

    //json connections
    connect(ui->SaveToFile, &QAction::triggered, this, &View::savePressed);
    connect(this, &View::saveModel, &model, &Model::savePressed);
    connect(ui->LoadFromFile, &QAction::triggered, this, &View::loadPressed);
    connect(this, &View::loadModel, &model, &Model::loadPressed);
    connect(&model, &Model::resizeCanvas,this, &View::resizeCanvas);

    //new project signal
    connect(ui->NewProject, &QAction::triggered, &model, &Model::newProjectPressed);
    connect(&model, &Model::projectReset, this, &View::projectReset);

    //RGBA Slider signals/slots
    connect(ui->redSlider, &QSlider::valueChanged, this, &View::redSliderValueChanged);
    connect(ui->greenSlider, &QSlider::valueChanged, this, &View::greenSliderValueChanged);
    connect(ui->blueSlider, &QSlider::valueChanged, this, &View::blueSliderValueChanged);
    connect(ui->alphaSlider, &QSlider::valueChanged, this, &View::alphaSliderValueChanged);

    //RGBA Spin box signals/slots
    connect(ui->redSpinBox, &QSpinBox::valueChanged, this, &View::redSpinBoxChanged);
    connect(ui->greenSpinBox, &QSpinBox::valueChanged, this, &View::greenSpinBoxChanged);
    connect(ui->blueSpinBox, &QSpinBox::valueChanged, this, &View::blueSpinBoxChanged);
    connect(ui->alphaSpinBox, &QSpinBox::valueChanged, this, &View::alphaSpinBoxChanged);

    //Pixel dimension slider + confirm button signals/slots
    connect(ui->pixelDimensionSlider, &QSlider::valueChanged, this, &View::pixelDimensionSliderChanged);
    connect(ui->confirmDimensionButton, &QAbstractButton::clicked, this, &View::canvasSizeSelected);
    connect(this, &View::canvasSizeSignal, &model, &Model::newCanvas);

    //Connect swatch signals and slots
    connect(&model, &Model::updateColorSliders, this, &View::updateColorSliders);
    connect(&model, &Model::fillSwatch, this, &View::updateSwatchColor);
    //connect swatches 1-6
    connect(ui->swatch1, &QAbstractButton::clicked, &model, &Model::swatch1Clicked);
    connect(ui->swatch2, &QAbstractButton::clicked, &model, &Model::swatch2Clicked);
    connect(ui->swatch3, &QAbstractButton::clicked, &model, &Model::swatch3Clicked);
    connect(ui->swatch4, &QAbstractButton::clicked, &model, &Model::swatch4Clicked);
    connect(ui->swatch5, &QAbstractButton::clicked, &model, &Model::swatch5Clicked);
    connect(ui->swatch6, &QAbstractButton::clicked, &model, &Model::swatch6Clicked);


    //Connect image preview signal/slot
    connect(&model, &Model::sendImage, this, &View::setImagePreview);
    connect(ui->playbackSpeedSlider, &QSlider::valueChanged, this, &View::imagePreviewSliderChanged);
    connect(ui->playbackSpeedSpinBox, &QSpinBox::valueChanged, this, &View::imagePreviewSpinBoxChanged);
    connect(this, &View::fpsChanged, &model, &Model::updateFPS);
}

View::~View()
{
    delete ui;
}

void View::setImagePreview(QImage image){
    ui->previewLabel->setPixmap(QPixmap::fromImage(image).scaled(200, 200, Qt::KeepAspectRatio));
}


void View::imagePreviewSliderChanged(){
    ui->playbackSpeedSpinBox->setValue(ui->playbackSpeedSlider->value());
    emit fpsChanged(ui->playbackSpeedSlider->value());
}

void View::imagePreviewSpinBoxChanged(){
    ui->playbackSpeedSlider->setValue(ui->playbackSpeedSpinBox->value());
    emit fpsChanged(ui->playbackSpeedSpinBox->value());
}

void View::redSliderValueChanged(){
    ui->redSpinBox->setValue(ui->redSlider->value());
    emit colorValueChanged("red", ui->redSlider->value());
}

void View::greenSliderValueChanged(){
    ui->greenSpinBox->setValue(ui->greenSlider->value());
    emit colorValueChanged("green", ui->greenSlider->value());
}

void View::blueSliderValueChanged(){
    ui->blueSpinBox->setValue(ui->blueSlider->value());
    emit colorValueChanged("blue", ui->blueSlider->value());
}

void View::alphaSliderValueChanged(){
    ui->alphaSpinBox->setValue(ui->alphaSlider->value());
    emit colorValueChanged("alpha", ui->alphaSlider->value());
}

void View::redSpinBoxChanged(){
    ui->redSlider->setValue(ui->redSpinBox->value());
    emit colorValueChanged("red", ui->redSpinBox->value());
}

void View::greenSpinBoxChanged(){
    ui->greenSlider->setValue(ui->greenSpinBox->value());
    emit colorValueChanged("green", ui->greenSpinBox->value());
}

void View::blueSpinBoxChanged(){
    ui->blueSlider->setValue(ui->blueSpinBox->value());
    emit colorValueChanged("blue", ui->blueSpinBox->value());
}

void View::alphaSpinBoxChanged(){
    ui->alphaSlider->setValue(ui->alphaSpinBox->value());
    emit colorValueChanged("alpha", ui->alphaSpinBox->value());
}

void View::pixelDimensionSliderChanged(){
    QString pixelLabel;
    pixelLabel += std::to_string(ui->pixelDimensionSlider->value()) + " x " + std::to_string(ui->pixelDimensionSlider->value()) + " Pixels";
    ui->pixelDimensionLabel->setText(pixelLabel);
}

void View::canvasSizeSelected(){
    ui->canvas->setVisible(true);
    ui->canvas->setGridSize(ui->pixelDimensionSlider->value());
    ui->pixelDimensionSlider->setVisible(false);
    ui->pixelDimensionLabel->setVisible(false);
    ui->confirmDimensionButton->setVisible(false);
    ui->previewLabel->setVisible(true);
    emit canvasSizeSignal(ui->pixelDimensionSlider->value());
}

void View::updateColorSliders(QColor currentColor){
    ui->redSlider->setValue(currentColor.red());
    ui->greenSlider->setValue(currentColor.green());
    ui->blueSlider->setValue(currentColor.blue());
    ui->alphaSlider->setValue(currentColor.alpha());
}



void View::updateSwatchColor(int swatch, QString styleString){
    switch(swatch){
    case 1:
        ui->swatch1->setStyleSheet(styleString);
        break;
    case 2:
        ui->swatch2->setStyleSheet(styleString);
        break;
    case 3:
        ui->swatch3->setStyleSheet(styleString);
        break;
    case 4:
        ui->swatch4->setStyleSheet(styleString);
        break;
    case 5:
        ui->swatch5->setStyleSheet(styleString);
        break;
    case 6:
        ui->swatch6->setStyleSheet(styleString);
    }
}

void View::savePressed() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                    "/home/untitled.ssp",
                                                    tr("JSON (*.ssp)"));
    if (!fileName.isEmpty()) {
        emit saveModel(fileName);
    }
}

void View::loadPressed(){
    QString fileName = QFileDialog::getOpenFileName(this, tr("Save File"),
                                                    "/home/untitled.ssp",
                                                    tr("JSON (*.ssp)"));
    if (!fileName.isEmpty()) {
        emit loadModel(fileName);
    }
}

void View::resizeCanvas(int size) {
    ui->canvas->setGridSize(size);
}

void View::projectReset(){
    ui->canvas->setVisible(false);
    ui->previewLabel->setVisible(false);
    ui->pixelDimensionLabel->setVisible(true);
    ui->pixelDimensionSlider->setVisible(true);
    ui->confirmDimensionButton->setVisible(true);
}

void View::updateColorPreview(QString styleString){
    ui->colorPreview->setStyleSheet(styleString);
}

void View::setBrush(bool enabled){
    // WE MAY NEED TO ADD A PORTION TO STYLE SHEET THAT EXPLICITLY SETS THE DIMENSIONS OF BUTTON EACH TIME
    if (enabled){
        ui->brushButton->setStyleSheet("background-color: rgba(255, 255, 255, 255);\nwidth: 40px;\nheight: 40px;\nmargin-left: auto;\nmargin-right: auto;");
        return;
    }

    ui->brushButton->setStyleSheet("background-color: rgba(60, 60, 60, 255);\nwidth: 40px;\nheight: 40px;\nmargin-left: auto;\nmargin-right: auto;");
}

void View::setEraser(bool enabled){
    if (enabled){
        ui->eraseButton->setStyleSheet("background-color: rgba(255, 255, 255, 255);\nwidth: 40px;\nheight: 40px;\nmargin-left: auto;\nmargin-right: auto;");
        return;
    }

    ui->eraseButton->setStyleSheet("background-color: rgba(60, 60, 60, 255);\nwidth: 40px;\nheight: 40px;\nmargin-left: auto;\nmargin-right: auto;");
}

void View::setPicker(bool enabled){
    if (enabled){
        ui->dropperButton->setStyleSheet("background-color: rgba(255, 255, 255, 255);\nwidth: 40px;\nheight: 40px;\nmargin-left: auto;\nmargin-right: auto;");
        return;
    }

    ui->dropperButton->setStyleSheet("background-color: rgba(60, 60, 60, 255);\nwidth: 40px;\nheight: 40px;\nmargin-left: auto;\nmargin-right: auto;");
}
