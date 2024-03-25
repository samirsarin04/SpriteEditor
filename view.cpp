#include "view.h"
#include "ui_view.h"
#include "model.h"
#include "button.h"

View::View(Model &model, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::View)
{
    ui->setupUi(this);
    ui->test->setStyleSheet("background-color: rgb(255, 0, 0)");

    connect(ui->canvas, &Canvas::canvasClickSignal, &model, &Model::canvasClick);


}

View::~View()
{
    delete ui;
}
