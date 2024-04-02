#include "view.h"
#include "ui_view.h"
#include <QMainWindow>
#include <QFileDialog>
#include "model.h"
#include "canvas.h"
#include <QMessageBox>
#include "framepreviewbutton.h"
#include "color.h"

///@brief The cpp file for the view for this application. Provides implementation for the slots and methods.
/// Reviewed by: Hudson Bowman

View::View(Model &model, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::View)
    , modelPtr(&model)
{
    ui->setupUi(this);
    this->setStyleSheet("background-color: rgb(200, 200, 200)");
    ui->canvas->setVisible(false);
    ui->transparentBackdrop->setVisible(false);

    //Set Color Preview to default value
    ui->colorPreview->setStyleSheet("background-color: rgb(0, 0, 255)");
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
    ui->swatch1->setStyleSheet("background-color: rgb(0, 0, 255)");
    ui->swatch2->setStyleSheet("background-color: rgb(0, 0, 0)");
    ui->swatch3->setStyleSheet("background-color: rgb(0, 0, 0)");
    ui->swatch4->setStyleSheet("background-color: rgb(0, 0, 0)");
    ui->swatch5->setStyleSheet("background-color: rgb(0, 0, 0)");
    ui->swatch6->setStyleSheet("background-color: rgb(0, 0, 0)");

    //Canvas Signals/Slots
    connect(ui->canvas, &Canvas::canvasClickSignal, &model, &Model::canvasClick);
    connect(&model, &Model::sendFrameToCanvas, ui->canvas, &Canvas::updateCanvas);
    connect(ui->canvas, &Canvas::canvasMoveSignal, &model, &Model::canvasMovement);

    //connect error signal/slot
    connect(&model, &Model::errorOccurred, this, &View::errorOccurred);


    //Update Color signal/slot
    connect(this, &View::colorValueChanged, &model, &Model::colorChanged);

    //Toolbar buttons signals/slots
    connect(ui->brushButton, &QPushButton::clicked, &model, &Model::toolToPaint);
    connect(ui->eraseButton, &QPushButton::clicked, &model, &Model::toolToEraser);
    connect(ui->dropperButton, &QPushButton::clicked, &model, &Model::toolToDropper);
    connect(ui->undoButton, &QPushButton::clicked, &model, &Model::undoAction);

    //clone
    connect(ui->cloneButton, &QPushButton::clicked, &model, &Model::cloneButton);

    //help menu
    connect(ui->helpDropDown, &QAction::triggered, this, &View::helpPressed);

    //json connections
    connect(ui->SaveToFile, &QAction::triggered, this, &View::savePressed);
    connect(this, &View::saveModel, &model, &Model::savePressed);
    connect(ui->LoadFromFile, &QAction::triggered, this, &View::loadPressed);
    connect(this, &View::loadModel, &model, &Model::loadPressed);
    connect(&model, &Model::resizeCanvas,this, &View::resizeCanvas);

    //new project signal
    connect(ui->NewProject, &QAction::triggered, &model, &Model::newProjectPressed);
    connect(&model, &Model::projectReset, this, &View::projectReset);
    connect(&model, &Model::messageBox, this, &View::messageBox);
    connect(this, &View::messageYesSelected, &model, &Model::messageYesSelected);

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
    connect(&model, &Model::sendImageToPreview, this, &View::setImagePreview);
    connect(ui->playbackSpeedSlider, &QSlider::valueChanged, this, &View::imagePreviewSliderChanged);
    connect(ui->playbackSpeedSpinBox, &QSpinBox::valueChanged, this, &View::imagePreviewSpinBoxChanged);
    connect(this, &View::fpsChanged, &model, &Model::updateFPS);

    //Connect frame preview signals/slots
    ui->frameLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
    connect(ui->addFrameButton, &QPushButton::clicked, &model, &Model::addFrame);
    connect(ui->removeFrameButton,&QPushButton::clicked, &model, &Model::removeFrame);
    connect(&model, &Model::setFrameSelectorIcon, this, &View::setImageIcon);
    connect(&model,&Model::createFrameSelectorButton, this, &View::addPressed);
    connect(&model,&Model::deleteFrameSelectorButton, this, &View::deletePressed);


    //Disable buttons on startup
    ui->addFrameButton->setEnabled(false);
    ui->removeFrameButton->setEnabled(false);
    ui->fullSizePlaybackButton->setEnabled(false);
    ui->cloneButton->setEnabled(false);
    ui->undoButton->setEnabled(false);
    ui->playbackSpeedSlider->setEnabled(false);
    ui->playbackSpeedSpinBox->setEnabled(false);

    //Toggle playback size signal/slot
    connect(ui->fullSizePlaybackButton,&QPushButton::clicked, &model, &Model::togglePreviewSize);
}

///@brief Deconstructs the view
View::~View()
{
    delete ui;
}

///@brief Changes the image in the preview window to the given image
///@param image the image to display
///@param trueSizePreview true to display the image as its true size, false to scale it to the size of the preview window
///@param size the size of the image
void View::setImagePreview(QImage image, bool trueSizePreview, int size){

    if (trueSizePreview){
        ui->previewLabel->setPixmap(QPixmap::fromImage(image).scaled(size, size, Qt::KeepAspectRatio));
        return;
    }


    ui->previewLabel->setPixmap(QPixmap::fromImage(image).scaled(200, 200, Qt::KeepAspectRatio));
}

///@brief changes the image preview for a frame
///@param image the image to put in the frame preview
///@param ID the id of the frame
///@param lastID the id of the last frame
void View::setImageIcon(QImage image, int ID, int lastID){
    if (lastID != ID){
        //Sets the color of the last selected frame button to black (inactive)
        FramePreviewButton* frameButton = this->findChild<FramePreviewButton*>(QString::number(lastID));
        if (frameButton){
            frameButton->setStyleSheet("border: 4px solid black;");
        }
    }
    // sets the color of currently selected frame button to red (active)
    // also sets the icon with the appropriate image
    FramePreviewButton* frameButton = this->findChild<FramePreviewButton*>(QString::number(ID));
    if (frameButton){
        frameButton->setPixmap(QPixmap::fromImage(image).scaled(200, 200, Qt::KeepAspectRatio));
        frameButton->setStyleSheet("border: 4px solid red;");
    }
}

///@brief updates the fps when the preview fps slider is changed
void View::imagePreviewSliderChanged(){
    ui->playbackSpeedSpinBox->setValue(ui->playbackSpeedSlider->value());
    emit fpsChanged(ui->playbackSpeedSlider->value());
}

///@brief updates the fps when the preview fps text box is changed
void View::imagePreviewSpinBoxChanged(){
    ui->playbackSpeedSlider->setValue(ui->playbackSpeedSpinBox->value());
    emit fpsChanged(ui->playbackSpeedSpinBox->value());
}

///@brief updates the red value when the red slider is changed
void View::redSliderValueChanged(){
    ui->redSpinBox->setValue(ui->redSlider->value());
    emit colorValueChanged(Color::red, ui->redSlider->value());
}

///@brief updates the green value when the green slider is changed
void View::greenSliderValueChanged(){
    ui->greenSpinBox->setValue(ui->greenSlider->value());
    emit colorValueChanged(Color::green, ui->greenSlider->value());
}

///@brief updates the green value when the green slider is changed
void View::blueSliderValueChanged(){
    ui->blueSpinBox->setValue(ui->blueSlider->value());
    emit colorValueChanged(Color::blue, ui->blueSlider->value());
}

///@brief updates the alpha value when the alpha slider is changed
void View::alphaSliderValueChanged(){
    ui->alphaSpinBox->setValue(ui->alphaSlider->value());
    emit colorValueChanged(Color::alpha, ui->alphaSlider->value());
}

///@brief updates the red value when the red text box is changed
void View::redSpinBoxChanged(){
    ui->redSlider->setValue(ui->redSpinBox->value());
    emit colorValueChanged(Color::red, ui->redSpinBox->value());
}

///@brief updates the green value when the green text box is changed
void View::greenSpinBoxChanged(){
    ui->greenSlider->setValue(ui->greenSpinBox->value());
    emit colorValueChanged(Color::green, ui->greenSpinBox->value());
}

///@brief updates the blue value when the blue text box is changed
void View::blueSpinBoxChanged(){
    ui->blueSlider->setValue(ui->blueSpinBox->value());
    emit colorValueChanged(Color::blue, ui->blueSpinBox->value());
}

///@brief updates the alpha value when the alpha text box is changed
void View::alphaSpinBoxChanged(){
    ui->alphaSlider->setValue(ui->alphaSpinBox->value());
    emit colorValueChanged(Color::alpha, ui->alphaSpinBox->value());
}

///@brief updates the canvas/frame dimension when the slider is changed
void View::pixelDimensionSliderChanged(){
    QString pixelLabel;
    pixelLabel += std::to_string(ui->pixelDimensionSlider->value()) + " x " + std::to_string(ui->pixelDimensionSlider->value()) + " Pixels";
    ui->pixelDimensionLabel->setText(pixelLabel);
}

///@brief sets up canvas and buttons when the size is selected
void View::canvasSizeSelected(){
    ui->addFrameButton->setEnabled(true);
    ui->removeFrameButton->setEnabled(true);
    ui->fullSizePlaybackButton->setEnabled(true);


    ui->cloneButton->setEnabled(true);
    ui->undoButton->setEnabled(true);


    ui->playbackSpeedSlider->setEnabled(true);
    ui->playbackSpeedSpinBox->setEnabled(true);

    ui->transparentBackdrop->setVisible(true);
    ui->canvas->setVisible(true);
    int size = ui->pixelDimensionSlider->value();
    int pixelSize = (640 / size);
    int canvasSize = pixelSize * size;
    ui->transparentBackdrop->setFixedSize(canvasSize, canvasSize);
    ui->canvas->setGridSize(ui->pixelDimensionSlider->value());
    ui->pixelDimensionSlider->setVisible(false);
    ui->pixelDimensionLabel->setVisible(false);
    ui->confirmDimensionButton->setVisible(false);
    ui->previewLabel->setVisible(true);
    emit canvasSizeSignal(ui->pixelDimensionSlider->value());
}

///@brief updates the values of the sliders to the RGBA values of a given color
///@param currentColor the color to change the RGBA sliders to
void View::updateColorSliders(QColor currentColor){
    ui->redSlider->setValue(currentColor.red());
    ui->greenSlider->setValue(currentColor.green());
    ui->blueSlider->setValue(currentColor.blue());
    ui->alphaSlider->setValue(currentColor.alpha());
}

///@brief changes the qss of the given swatch to the given stleString
///@param swatch the swatch number to change
///@param styleString the qss to put in the swatch
void View::updateSwatchColor(int swatch, QString styleString){
    switch(swatch){
    case 0:
        ui->swatch1->setStyleSheet(styleString);
        break;
    case 1:
        ui->swatch2->setStyleSheet(styleString);
        break;
    case 2:
        ui->swatch3->setStyleSheet(styleString);
        break;
    case 3:
        ui->swatch4->setStyleSheet(styleString);
        break;
    case 4:
        ui->swatch5->setStyleSheet(styleString);
        break;
    case 5:
        ui->swatch6->setStyleSheet(styleString);
    }
}

///@brief saves the sprite when the save button is pressed
void View::savePressed() {
    if (ui->pixelDimensionSlider->isVisible()){
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                    "/home/untitled.ssp",
                                                    tr("JSON (*.ssp)"));
    if (!fileName.isEmpty()) {
        emit saveModel(fileName);
    }
}

///@brief loads a sprite from json when the load button is pressed
void View::loadPressed(){
    QString fileName = QFileDialog::getOpenFileName(this, tr("Save File"),
                                                    "/home/untitled.ssp",
                                                    tr("JSON (*.ssp)"));
    if (!fileName.isEmpty()) {
        canvasSizeSelected();

        while (QLayoutItem *previewButton = ui->frameLayout->takeAt(0)){
            if (QWidget *button = previewButton->widget()){
                ui->frameLayout->removeWidget(button);
                delete button;
            }
        }

        ui->playbackSpeedSpinBox->setValue(0);
        ui->playbackSpeedSlider->setValue(0);

        emit loadModel(fileName);
    }
}

///@brief displays a text popup when the help button is pressed
void View::helpPressed(){
    QString message = "To start your project, choose and confirm your pixel dimensions.\nTools:\nColor Sliders- Slide the red, green, blue and alpha sliders to change the current color's values."
                      "\nPaintbrush- Allows drawing with the color set by the rgba sliders.\nEraser- Allows for erasing of pixels drawn by the paintbrush, making them transparent."
                      "\nDropper- Allows selecting of a pixel to set the current color to the same color as the pixel selected.\n"
                      "Clone- Clones the current frame's contents onto a new frame.\nUndo- Reverts a frame's most recent change.\nColor Swatches- Allows for easy switching between presets of colors. The current"
                      " selected swatch changes as you change the color values. To save the current swatch and set a new one, select a different swatch.\nAnimations: \nPreview- Small box in the top right that shows"
                      " the preview of your current animation.\nPlayback Speed(FPS) Slider- Used to adjust the playback speed of the preview.\nFrames Window- Shows each of your frames in desecending order. To add a frame,"
                      " press the add frame button (or clone). To delete a frame, press the delete frame button. To edit a frame, select it in the frames window.\nTrue Size Playback- Shows your frames playback relative to the true"
                      " size of the image.\nFile Menu: \nSave to file- Allows saving of your project to a custom .ssp file, which can then be loaded and edited.\nLoad from file- Allows loading and editing from a saved custom .ssp"
                      " file.\nNew Project- Used to start a new project, allowing for resetting and resizing, if selected the current project will be completely deleted.";
    QMessageBox::about(this, "Help", message);
}

///@brief resizes the canvas to a given size
///@param size the new size of the canvas
void View::resizeCanvas(int size) {
    ui->canvas->setGridSize(size);
    int pixelSize = (640 / size);
    int canvasSize = pixelSize * size;
    ui->transparentBackdrop->setFixedSize(canvasSize, canvasSize);
}

///@brief displays a message box prompting the user to confirm if they want to reset the project
void View::messageBox(){
    auto reply = QMessageBox::question(nullptr, "New Project", "Are you sure you want to reset the project?",
                                       QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        emit messageYesSelected();
    }
}

///@brief resets the sprite editor
void View::projectReset(){

    while (QLayoutItem *previewButton = ui->frameLayout->takeAt(0)){
        if (QWidget *button = previewButton->widget()){
            ui->frameLayout->removeWidget(button);
            delete button;
        }
    }
    ui->playbackSpeedSpinBox->setValue(0);
    ui->playbackSpeedSlider->setValue(0);
    emit fpsChanged(0);

    ui->cloneButton->setEnabled(false);
    ui->undoButton->setEnabled(false);

    ui->playbackSpeedSlider->setEnabled(false);
    ui->playbackSpeedSpinBox->setEnabled(false);

    ui->transparentBackdrop->setVisible(false);
    ui->canvas->setVisible(false);
    ui->previewLabel->setVisible(false);
    ui->pixelDimensionLabel->setVisible(true);
    ui->pixelDimensionSlider->setVisible(true);
    ui->confirmDimensionButton->setVisible(true);

    ui->addFrameButton->setEnabled(false);
    ui->removeFrameButton->setEnabled(false);
    ui->fullSizePlaybackButton->setEnabled(false);
}

///@brief changes the qss of the color preview
///@param styleString the new qss for the colorPreview
void View::updateColorPreview(QString styleString){
    ui->colorPreview->setStyleSheet(styleString);
}

///@brief changes if the color sliders are enabled or disabled
///@param enabled true to enable, false to disable
void View::enableColorSliders(bool enabled){

    ui->redSlider->setEnabled(enabled);
    ui->redSpinBox->setEnabled(enabled);

    ui->greenSlider->setEnabled(enabled);
    ui->greenSpinBox->setEnabled(enabled);

    ui->blueSlider->setEnabled(enabled);
    ui->blueSpinBox->setEnabled(enabled);

    ui->alphaSpinBox->setEnabled(enabled);
    ui->alphaSlider->setEnabled(enabled);
}

///@brief highlights or un-highlights the brush tool
///@param enabled true to highlight the button, false to un-highlight
void View::setBrush(bool enabled){
    if (enabled){
        ui->brushButton->setStyleSheet("background-color: rgba(255, 255, 255, 255);\nwidth: 40px;\nheight: 40px;\nmargin-left: auto;\nmargin-right: auto;");
        enableColorSliders(true);
        return;
    }

    ui->brushButton->setStyleSheet("background-color: rgba(200, 200, 200, 255);\nwidth: 40px;\nheight: 40px;\nmargin-left: auto;\nmargin-right: auto;");
}

///@brief highlights or un-highlights the eraser tool
///@param enabled true to highlight the button, false to un-highlight
void View::setEraser(bool enabled){
    if (enabled){
        ui->eraseButton->setStyleSheet("background-color: rgba(255, 255, 255, 255);\nwidth: 40px;\nheight: 40px;\nmargin-left: auto;\nmargin-right: auto;");
        enableColorSliders(false);
        return;
    }

    ui->eraseButton->setStyleSheet("background-color: rgba(200, 200, 200, 255);\nwidth: 40px;\nheight: 40px;\nmargin-left: auto;\nmargin-right: auto;");
}

///@brief highlights or un-highlights the dropper tool
///@param enabled true to highlight the button, false to un-highlight
void View::setPicker(bool enabled){
    if (enabled){
        ui->dropperButton->setStyleSheet("background-color: rgba(255, 255, 255, 255);\nwidth: 40px;\nheight: 40px;\nmargin-left: auto;\nmargin-right: auto;");
        enableColorSliders(false);
        return;
    }

    ui->dropperButton->setStyleSheet("background-color: rgba(200, 200, 200, 255);\nwidth: 40px;\nheight: 40px;\nmargin-left: auto;\nmargin-right: auto;");
}

///@brief adds a new frame button to the frame preview buttons
///@param ID the id of the frame
void View::addPressed(int ID) {

    FramePreviewButton *frame = new FramePreviewButton(ID);
    frame->setObjectName(QString::number(ID));

    ui->frameLayout->addWidget(frame, Qt::AlignHCenter);
    frame->setFixedSize(200,200);
    frame->setStyleSheet("border: 4px solid black;");
    frame->show();

    connect(frame, &FramePreviewButton::frameClicked, modelPtr, &Model::changeFrame);
}

///@brief deletes a frame button from the frame preview buttons
void View::deletePressed(int ID){
    FramePreviewButton* frameButton = this->findChild<FramePreviewButton*>(QString::number(ID));
    if (frameButton){
        ui->frameLayout->removeWidget(frameButton);
        delete frameButton;
    }
}

///@brief displays a message box with a given error message
///@param message the erro message to display
void View::errorOccurred(const QString &message){
    QMessageBox::critical(this, "Error", message);
}
