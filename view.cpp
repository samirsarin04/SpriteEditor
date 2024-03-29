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

    //set all swatches to black on initialization
    ui->swatch1->setStyleSheet("background-color: rgb(0, 0, 0)");
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
}

View::~View()
{
    delete ui;
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

void View::updateColorPreview(QString styleString){
    ui->colorPreview->setStyleSheet(styleString);
}
