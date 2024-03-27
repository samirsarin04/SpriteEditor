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

    connect(ui->canvas, &Canvas::canvasClickSignal, &model, &Model::canvasClick);
    connect(&model, &Model::sendFrameToCanvas, ui->canvas, &Canvas::updateCanvas);
    connect(ui->canvas, &Canvas::canvasMoveSignal, &model, &Model::canvasMovement);
    connect(ui->redSlider, &QSlider::sliderMoved, this, &View::updateRedSliderLabel);
    connect(ui->redSlider, &QSlider::sliderReleased, this, &View::updateRedSliderLabel);
    connect(ui->greenSlider, &QSlider::sliderMoved, this, &View::updateGreenSliderLabel);
    connect(ui->greenSlider, &QSlider::sliderReleased, this, &View::updateGreenSliderLabel);
    connect(ui->blueSlider, &QSlider::sliderMoved, this, &View::updateBlueSliderLabel);
    connect(ui->blueSlider, &QSlider::sliderReleased, this, &View::updateBlueSliderLabel);
    connect(ui->alphaSlider, &QSlider::sliderMoved, this, &View::updateAlphaSliderLabel);
    connect(ui->alphaSlider, &QSlider::sliderReleased, this, &View::updateAlphaSliderLabel);
}

View::~View()
{
    delete ui;
}

void View::updateRedSliderLabel(){
    QString sliderValue = QString::number(ui->redSlider->value());
    QString labelText = "Red: " + sliderValue;
    ui->redSliderLabel->setText(labelText);
}

void View::updateGreenSliderLabel(){
    QString sliderValue = QString::number(ui->greenSlider->value());
    QString labelText = "Green: " + sliderValue;
    ui->greenSliderLabel->setText(labelText);
}

void View::updateBlueSliderLabel(){
    QString sliderValue = QString::number(ui->blueSlider->value());
    QString labelText = "Blue: " + sliderValue;
    ui->blueSliderLabel->setText(labelText);
}

void View::updateAlphaSliderLabel(){
    QString sliderValue = QString::number(ui->alphaSlider->value());
    QString labelText = "Alpha: " + sliderValue;
    ui->alphaSliderLabel->setText(labelText);
}
