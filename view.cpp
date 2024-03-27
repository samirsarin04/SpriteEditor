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
    connect(ui->redSlider, &QSlider::sliderMoved, this, &View::redSliderValueChanged);
    connect(ui->greenSlider, &QSlider::sliderMoved, this, &View::greenSliderValueChanged);
    connect(ui->blueSlider, &QSlider::sliderMoved, this, &View::blueSliderValueChanged);
    connect(ui->alphaSlider, &QSlider::sliderMoved, this, &View::alphaSliderValueChanged);
    connect(this, &View::sliderValueChanged, &model, &Model::sliderChanged);
}

View::~View()
{
    delete ui;
}

void View::redSliderValueChanged(){
    QString sliderValue = QString::number(ui->redSlider->value());
    QString labelText = "Red: " + sliderValue;
    ui->redSliderLabel->setText(labelText);
    emit sliderValueChanged("red", ui->redSlider->value());
}

void View::greenSliderValueChanged(){
    QString sliderValue = QString::number(ui->greenSlider->value());
    QString labelText = "Green: " + sliderValue;
    ui->greenSliderLabel->setText(labelText);
    emit sliderValueChanged("green", ui->greenSlider->value());
}

void View::blueSliderValueChanged(){
    QString sliderValue = QString::number(ui->blueSlider->value());
    QString labelText = "Blue: " + sliderValue;
    ui->blueSliderLabel->setText(labelText);
    emit sliderValueChanged("blue", ui->blueSlider->value());
}

void View::alphaSliderValueChanged(){
    QString sliderValue = QString::number(ui->alphaSlider->value());
    QString labelText = "Alpha: " + sliderValue;
    ui->alphaSliderLabel->setText(labelText);
    emit sliderValueChanged("alpha", ui->alphaSlider->value());
}
