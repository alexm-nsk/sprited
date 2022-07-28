#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "openglwidget.h"
#include "animation.h"
#include "qint.h"
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


enum rectParameter{
    left, top, width, height
};

enum pointParameter{
    x, y
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QRect *getSelectedRect(){
        return &selectedRect;
    }

private slots:

    void connectSpinBoxToRect (QString widgetName, rectParameter parameter);
    void connectSpinBoxToPoint (QString widgetName, pointParameter parameter);

    void draw();
    virtual void paintEvent(QPaintEvent *e);

    void setFrameDuration(int newDuration);
    void setCurrentSequence(QString newName);
    void setCurrentSequenceOffsetX(int newValue);
    void setCurrentSequenceOffsetY(int newValue);
    void setCurrentSequenceLoopStart(int newValue);
    void setCurrentSequenceLoopEnd(int newValue);
    void setCurrentSequenceLoopEnabled(int newValue);

    void frameAdvance();

    void addSequence();
    void loadSpriteAtlas(QString path);
    void getFrame(size_t frame);

    void on_actionLoad_Image_triggered();
    void on_actionQuit_triggered();
    void on_spriteWindowWidget_aboutToCompose();
    void on_pushButton_3_clicked();
    void on_addFrameButton_clicked();
    void on_animationNameEdit_textEdited(const QString &arg1);
    void on_addSequenceButton_clicked();
    void on_sequenceNameEdit_textEdited(const QString &arg1);
    void on_animationComboBox_currentIndexChanged(int index);
    void on_sequenceComboBox_currentTextChanged(const QString &arg1);

    void on_seekBar_sliderMoved(int position);
    void loadSequence(int index);
    void on_sequenceComboBox_currentIndexChanged(int index);

    void on_seekBar_valueChanged(int value);

    void on_sequenceNameEdit_textChanged(const QString &arg1);

    void on_checkBox_toggled(bool checked);

    void on_actionSave_triggered();

    void on_actionOpen_Sprite_triggered();

    void on_playButton_clicked();

    void on_pauseButton_clicked();

    void on_stopButton_clicked();

    void on_pushButton_7_clicked();

private:

    void attachWidgets();
    QTimer timer;
    // these are the fields regarding editing process (which rectangle and frame are selected etc.):
    // TODO remove this when corresponding methods are directly tied to slots
    QRect selectedRect;


    // TODO connect slider and animation selector to those
    // TODO replace "brush" with a Frame
    // TODO draw big crosshair in sprite window
    // TODO keep selected atlas area in a "brush" frame
    // TODO add modifying current frame in slots
    // TODO proper timers in QT?

    // these are the fields regarding Animation data that's being edited:
    Animation animation;
    Frame lastAddedFrame;

    // pointer to COpenGLWindow (atlas window)
    COpenGLWidget* atlasWindow;
    // pointer to COpenGLWindow (sprite window)
    COpenGLWidget* spriteWindow;

    // currently loaded spritesheet:
    QPixmap spriteAtlas;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
