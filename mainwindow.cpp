#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <QFileDialog>
#include <QPixmap>
#include <QPoint>
#include <QPainter>

// TODO convert animation class to QObject-derivative
// TODO find if I can add ticks to seek bar
// TODO replace these with regular setter methods:
// TODO stop timer when editing frames
// TODO make "Set to this in all frames" in duration
// TODO stop if no filename was provided when loading animation
// TODO add support to mirror frames and fix going out of orders (negative coordinates and bigger than width or height coordnates)
// TODO at key commands to move arround, add and delete frames, etc.
// TODO add deletion of sequences, changing their order in the combobox
// TODO add copy duration
// TODO reset timer when selecting another animation

bool fileExists(QString fileName){
    QFileInfo fileInfo(fileName);
    if (fileInfo.exists() && fileInfo.isFile()){
        return true;
    }
    else
    {
        return false;
    }
}

void MainWindow::connectSpinBoxToRect(QString widgetName, rectParameter parameter){
    QObject::connect(
        findChild<QSpinBox*>(widgetName), QOverload<int>::of(&QSpinBox::valueChanged),
        [=]( const int &newValue ) {
                switch (parameter){
                    case rectParameter::left:
                        animation.setLeft(newValue);                        
                    break;
                    case rectParameter::top:
                        animation.setTop(newValue);                        
                    break;
                    case rectParameter::width:
                        animation.setWidth(newValue);                        
                    break;
                    case rectParameter::height:
                        animation.setHeight(newValue);                        
                    break;
                    }
                    size_t currentFrame = ui->seekBar->value();
                    QRect currentRect = animation.getFrame(currentFrame).getRect();
                    spriteWindow->setImageRect(currentRect);
                    atlasWindow->setSelectedRect(currentRect);
                }
    );
}

void MainWindow::connectSpinBoxToPoint(QString widgetName, pointParameter parameter){
    QObject::connect(
        findChild<QSpinBox*>(widgetName), QOverload<int>::of(&QSpinBox::valueChanged),
        [=]( const int &newValue ) {
                switch (parameter){
                    case pointParameter::x:
                        animation.setFrameOffsetX ( newValue );
                    break;
                    case pointParameter::y:
                        animation.setFrameOffsetY ( newValue );
                    break;
                    }
                    spriteWindow->setOffsetPoint(animation.getFrameOffset());
                }
    );
}

void MainWindow::loadSpriteAtlas(QString path){
    //spriteAtlas.~QPixmap();
    spriteAtlas.load(path);
    atlasWindow->setPixmap(spriteAtlas);
    spriteWindow->setPixmap(spriteAtlas);
}

void MainWindow::attachWidgets()
{
    //connect widgets to editor values here:
    connectSpinBoxToRect("frameXBox", rectParameter::left);
    connectSpinBoxToRect("frameYBox", rectParameter::top);
    connectSpinBoxToRect("frameWidthBox",   rectParameter::width);
    connectSpinBoxToRect("frameHeightBox",  rectParameter::height);

    connectSpinBoxToPoint("frameXOffsetBox", pointParameter::x);
    connectSpinBoxToPoint("frameYOffsetBox", pointParameter::y);

    QObject::connect(this->findChild<QSpinBox*>("frameDurationBox"), SIGNAL(valueChanged(int)),
                     this, SLOT(setFrameDuration(int)));

    QObject::connect(this->findChild<QComboBox*>("animationComboBox"), SIGNAL(currentTextChanged(QString)),
                     this, SLOT(setCurrentSequence(QString)));

    QObject::connect(this->findChild<QSpinBox*>("seqXOffsetBox"), SIGNAL(valueChanged(int)),
                     this, SLOT(setCurrentSequenceOffsetX(int)));
    QObject::connect(this->findChild<QSpinBox*>("seqYOffsetBox"), SIGNAL(valueChanged(int)),
                     this, SLOT(setCurrentSequenceOffsetY(int)));

    QObject::connect(this->findChild<QSpinBox*>("seqLoopStartBox"), SIGNAL(valueChanged(int)),
                     this, SLOT(setCurrentSequenceLoopStart(int)));
    QObject::connect(this->findChild<QSpinBox*>("seqLoopEndBox"), SIGNAL(valueChanged(int)),
                     this, SLOT(setCurrentSequenceLoopEnd(int)));

    QObject::connect(this->findChild<QCheckBox*>("loopCheckBox"), SIGNAL(stateChanged(int)),
                     this, SLOT(setCurrentSequenceLoopEnabled(int)));

}

void MainWindow::setCurrentSequenceLoopStart(int newValue){
    animation.setLoopStart(newValue);
}

void MainWindow::setCurrentSequenceLoopEnd(int newValue){
    animation.setLoopEnd(newValue);
}

void MainWindow::setCurrentSequenceLoopEnabled(int newValue){
    //unchecked = 0
    //checked = 2
    animation.setLoopEnabled(newValue == 2);
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) , ui(new Ui::MainWindow){
    ui->setupUi(this);

    // get sprite widgets' links here:
    atlasWindow = ui->atlasWindowWidget;
    spriteWindow = ui->spriteWindowWidget;
    loadSpriteAtlas("/home/alexm/disk/IMG/megaman.png");

    attachWidgets();
    lastAddedFrame = Frame();

    // configure widgets used to draw the spritesheet and the sprite:
    spriteWindow->setDrawCentered(true);
    spriteWindow->setFieldMarking(fieldMarking::crossHair);
    atlasWindow->setFieldMarking(fieldMarking::rect);

    connect(&timer, &QTimer::timeout, this, QOverload<>::of(&MainWindow::frameAdvance));

}

void MainWindow::frameAdvance(){
    size_t currentFrame = ui->seekBar->value() + 1;


    size_t start = animation.getLoopStart();
    size_t end   = animation.getLoopEnd();
    bool loop    = animation.getLoopEnabled();


    if (!loop){
        if (currentFrame >= animation.getSequenceLength()){
            currentFrame = 0;
        }
    }
    else{
        if (currentFrame >= end){
            currentFrame = start;
        }
    }

    DURATION duration = animation.getFrame(currentFrame).getDuration();
    timer.setInterval(duration);
    ui->seekBar->setValue(currentFrame);
}

void MainWindow::setCurrentSequence(QString newName){
    //animation.setCurrentSequence(newName);
    Q_UNUSED(newName);
}

void MainWindow::setCurrentSequenceOffsetX(int newValue){
    animation.setSequenceOffsetX(newValue);
    ui->spriteWindowWidget->setGlobalOffsetPoint(QPoint(newValue, animation.getOffsetY()));
}

void MainWindow::setCurrentSequenceOffsetY(int newValue){
    animation.setSequenceOffsetY(newValue);
    ui->spriteWindowWidget->setGlobalOffsetPoint(QPoint(animation.getOffsetX(), newValue));
}

void MainWindow::setFrameDuration(int newDuration){
    animation.setDuration(newDuration);
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
}

void MainWindow::draw(){

}

void MainWindow::on_actionLoad_Image_triggered(){

    QString fileName = QFileDialog::getOpenFileName( this,
                                                     tr("Open Image"),
                                                     "/home/alexm",
                                                     tr("Image Files (*.png *.jpg *.bmp)"));
    if (!fileExists(fileName))
        return;

    this->setWindowTitle(fileName);
    this->spriteAtlas.load(fileName);
}

void MainWindow::on_actionQuit_triggered(){
    this->close();
}

void MainWindow::on_spriteWindowWidget_aboutToCompose(){

}

void MainWindow::on_addFrameButton_clicked(){

    // TODO set it to last added
    if (animation.getNumSequences()){

        lastAddedFrame = animation.getFrame(animation.getSequenceLength() - 1);

        if (animation.addFrame(lastAddedFrame) != -1){
            int max = animation.getSequenceLength() - 1;
            if (max < 0) max = 0;
            ui->seekBar->setMaximum(max);
            ui->seekBar->setValue(max);
        }

    }
}

void MainWindow::on_pushButton_3_clicked(){

}

void MainWindow::on_animationNameEdit_textEdited(const QString &arg1){
    Q_UNUSED (arg1);
}

void MainWindow::addSequence(){
    // create a new sequence
    QString name = "new animation";
    animation.addSequence(name);
    ui->sequenceComboBox->addItem(name);
    // select the newly created sequence:
    int selected = ui->sequenceComboBox->count() - 1;
    ui->sequenceComboBox->setCurrentIndex(selected);
}

void MainWindow::on_addSequenceButton_clicked(){
    addSequence();
}

void MainWindow::on_sequenceNameEdit_textEdited(const QString &arg1){
    animation.setName(arg1);
    int selected = ui->sequenceComboBox->currentIndex();

    // won't work with "setCurrentText"
    // TODO try selecting with setCurrentText

    ui->sequenceComboBox->setItemText(selected, arg1);
    animation.setSequenceName(arg1);
}

void MainWindow::on_animationComboBox_currentIndexChanged(int index){
    Q_UNUSED(index);
}

void MainWindow::on_sequenceComboBox_currentTextChanged(const QString &arg1){
    Q_UNUSED(arg1);
}

void MainWindow::loadSequence(int index){
    QString name = this->findChild<QComboBox*>("sequenceComboBox")->currentText();
    ui->sequenceNameEdit->setText(name);
    animation.setCurrentSequence(index);

    // set up the seekbar and select the first frame:
    ui->seekBar->setMaximum(animation.getSequenceLength() - 1);
    ui->seekBar->setValue(0);
    getFrame(0);

    ui->seqXOffsetBox->blockSignals(true);
    ui->seqYOffsetBox->blockSignals(true);
    ui->seqLoopStartBox->blockSignals(true);
    ui->seqLoopEndBox->blockSignals(true);
    ui->loopCheckBox->blockSignals(true);

    ui->seqXOffsetBox->setValue(animation.getOffsetX());
    ui->seqYOffsetBox->setValue(animation.getOffsetY());

    ui->seqLoopStartBox->setValue(animation.getLoopStart());
    ui->seqLoopEndBox->setValue(animation.getLoopEnd());
    ui->loopCheckBox->setChecked(animation.getLoopEnabled());


    ui->seqXOffsetBox->blockSignals(false);
    ui->seqYOffsetBox->blockSignals(false);
    ui->seqLoopStartBox->blockSignals(false);
    ui->seqLoopEndBox->blockSignals(false);
    ui->loopCheckBox->blockSignals(false);

    ui->spriteWindowWidget->setGlobalOffsetPoint(animation.getOffset());
    // Frame currentFrame = animation.getFrame(frameIndex);
    timer.setInterval(animation.getDuration());
}

void MainWindow::on_sequenceComboBox_currentIndexChanged(int index){
    loadSequence(index);
}

void MainWindow::getFrame(size_t frameIndex){
    animation.setCurrentFrame(frameIndex);

    // TODO put these into array
    // Or do the thing using widget iterators frameGroupBox
    ui->frameXBox->blockSignals(true);
    ui->frameYBox->blockSignals(true);
    ui->frameWidthBox->blockSignals(true);
    ui->frameHeightBox->blockSignals(true);
    ui->frameDurationBox->blockSignals(true);
    ui->frameXOffsetBox->blockSignals(true);
    ui->frameYOffsetBox->blockSignals(true);

    Frame currentFrame = animation.getFrame(frameIndex);

    ui->frameXBox->setValue(currentFrame.getRect().x());
    ui->frameYBox->setValue(currentFrame.getRect().y());
    ui->frameWidthBox->setValue(currentFrame.getRect().width());
    ui->frameHeightBox->setValue(currentFrame.getRect().height());
    ui->frameXOffsetBox->setValue(currentFrame.getOffsetX());
    ui->frameYOffsetBox->setValue(currentFrame.getOffsetY());
    ui->frameDurationBox->setValue(currentFrame.getDuration());

    ui->frameXBox->blockSignals(false);
    ui->frameYBox->blockSignals(false);
    ui->frameWidthBox->blockSignals(false);
    ui->frameHeightBox->blockSignals(false);
    ui->frameDurationBox->blockSignals(false);
    ui->frameXOffsetBox->blockSignals(false);
    ui->frameYOffsetBox->blockSignals(false);

    QRect loadedRect = currentFrame.getRect();
    spriteWindow->setOffsetPoint(QPoint(currentFrame.getOffsetX(), currentFrame.getOffsetY()), false);
    spriteWindow->setImageRect(loadedRect, false);
    spriteWindow->setSelectedRect(loadedRect);
    atlasWindow->setSelectedRect(loadedRect, true); //TODO redraw only arround the selected rect
}

void MainWindow::on_seekBar_sliderMoved(int position){
    Q_UNUSED(position);
}

void MainWindow::on_seekBar_valueChanged(int value){
    getFrame(value);
}

void MainWindow::on_sequenceNameEdit_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
}

void MainWindow::on_checkBox_toggled(bool checked)
{
    Q_UNUSED(checked);
}

void MainWindow::on_actionSave_triggered()
{
    QString fileName = QFileDialog::getSaveFileName( this,
                                                     tr("Save sprite"),
                                                     "/home/alexm",
                                                     tr("JSON Files (*.json)"));
  //  if (!fileExists(fileName))
    //    return;

    animation.saveToJson(fileName);
}

void MainWindow::on_actionOpen_Sprite_triggered()
{
    QString fileName = QFileDialog::getOpenFileName( this,
                                                     tr("Load Animation"),
                                                     "/home/alexm",
                                                     tr("JSON Files (*.json)"));
    if (!fileExists(fileName))
        return;

    ui->sequenceComboBox->blockSignals(true);
    ui->sequenceComboBox->clear();
    animation.loadFromJson(fileName); // all the sets will be cleared inside loadFromJson
    for (size_t i = 0; i < animation.getNumSequences(); i++){
        QString seqName = animation.getSequenceName(i);
        ui->sequenceComboBox->addItem(seqName);
    }
    ui->sequenceComboBox->blockSignals(false);
    ui->sequenceComboBox->setCurrentIndex(0);
    loadSequence(0);
}

void MainWindow::on_playButton_clicked(){
    timer.start();
}

void MainWindow::on_pauseButton_clicked(){
    timer.stop();
}

void MainWindow::on_stopButton_clicked(){
    timer.stop();
}

void MainWindow::on_pushButton_7_clicked()
{

}

