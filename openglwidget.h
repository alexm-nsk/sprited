#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>
#include <QOpenGLWidget>
#include <QPainter>
#include <QPixmap>
#include <QWidget>

// TODO make a list of objects to draw on each repaint
enum fieldMarking{
    crossHair, rect, none
};

class COpenGLWidget : public QOpenGLWidget
{
public:
    COpenGLWidget(QWidget* parent) : QOpenGLWidget(parent) {
        mainPixmap = NULL;
        drawCentered = false;
        marking = fieldMarking::none;
        selectedRect = QRect(0, 0, 10, 10);
    }

    void initializeGL() override
    {
        glClearColor(0.5, 0.5, 0.5, 1);
    }

protected: void paintGL() override
    {
        int mX = (width())/(2 * scaleX);
        int mY = (height())/(2 * scaleY);
        QPainter p(this);
        p.scale(scaleX, scaleY);
        p.setPen(Qt::red);


        switch (marking){
        case fieldMarking::rect:
            p.drawRect(selectedRect);
            break;
        case fieldMarking::crossHair:
            p.drawLine(mX-2,mY,mX+2,mY);
            p.drawLine(mX,mY-2,mX,mY+2);
            break;
        case fieldMarking::none:
            //avoid a warning
            break;
        }

        if (mainPixmap){
            int X, Y;
            if ( drawCentered ){
                X = mX + offsetPoint.x() + globalOffsetPoint.x();
                Y = mY + offsetPoint.y() + globalOffsetPoint.y();
            }
            else{
                X = 0;
                Y = 0;
            }
            p.drawPixmap ( X, Y, *mainPixmap, imageRect.x(), imageRect.y(), imageRect.width(), imageRect.height());
        }
    }

private:
    QPixmap *mainPixmap;
    QRect imageRect;
    QRect selectedRect;
    QPoint offsetPoint;
    QPoint globalOffsetPoint;
    bool drawCentered;
    int scaleX = 2;
    int scaleY = 2;
    fieldMarking marking;

public:
    void setDrawCentered(bool drawCentered_){
        drawCentered = drawCentered_;
    }

    void setFieldMarking(fieldMarking fieldMarking_){
        marking = fieldMarking_;
    }

    void setOffsetPoint(QPoint point, bool do_repaint = true){
        offsetPoint = point;
        if (do_repaint)
            repaint();
    }

    void setGlobalOffsetPoint(QPoint point, bool do_repaint = true){
        globalOffsetPoint = point;
        if (do_repaint)
            repaint();
    }

    void setSelectedRect(QRect rect, bool do_repaint = true)
    {
        selectedRect = rect;
        if (do_repaint)
            repaint();
    }

    void setSelectedRect(int x, int y, int width, int height, bool do_repaint = true)
    {
        selectedRect.setX(x);
        selectedRect.setY(y);
        selectedRect.setWidth(width);
        selectedRect.setHeight(height);
        if (do_repaint)
            repaint();
    }

    void setImageRect(QRect &rect, bool do_repaint = true)
    {
        imageRect = rect;
        if (do_repaint)
            repaint();
    }

    void setPixmap(QPixmap &pixmap)
    {
        mainPixmap = &pixmap;
    }

    void drawSegment(int x, int y, QPixmap &pixmap,int sx, int sy, int width, int height )
    {
        QPainter p(this);
        p.scale(2,2);
        p.drawPixmap ( x, y, pixmap, sx, sy, width, height);

    }

    void drawSegment(int x, int y, QPixmap &pixmap, QRect &rect)
    {
        QPainter p(this);
        p.scale(2,2);
        p.drawPixmap ( x,y, pixmap, rect.x(), rect.y(), rect.width(), rect.height());
    }

    void resizeGL(int w, int h) override
    {
        glViewport(0, 0, w, h);
    }

    ~COpenGLWidget()
    {
        //sprite->~QPixmap();
    }

    protected:
    void mouseMoveEvent(QMouseEvent *event) override
    {
        int dx = event->x();
        int dy = event->y();

        QPainter p(this);
        //p.scale(2,2);
        p.setPen(Qt::red);
        p.drawRect(QRect(dx,dy,100,100));
        paintGL();

        update();
    }

    void wheelEvent(QWheelEvent *ev) override
    {
        scaleX += ev->angleDelta().y()/100.0;
        scaleY += ev->angleDelta().y()/100.0;
        if (scaleX < 1) scaleX = 1;
        if (scaleY < 1) scaleY = 1;
        repaint();
    }
};

#endif // OPENGLWIDGET_H

