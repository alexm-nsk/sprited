#ifndef FRAME_H
#define FRAME_H

#include <qrect.h>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>

#define DURATION unsigned int

class Frame
{
private:
    QRect rect;
    QPoint offset;
    DURATION duration; //in ms.

public:
    Frame();

    Frame(QRect rect, DURATION duration){
        setRect(rect);
        setDuration(duration);
    }

    Frame(QJsonObject obj){
        //name = obj["name"].toString();
        offset.setX(obj["offsetX"].toInt());
        offset.setY(obj["offsetY"].toInt());
        QJsonArray jsonRectArray = obj["rect"].toArray();
        rect.moveLeft(jsonRectArray[0].toInt());
        rect.moveTop(jsonRectArray[1].toInt());
        rect.setWidth(jsonRectArray[2].toInt());
        rect.setHeight(jsonRectArray[3].toInt());
        duration = obj["duration"].toInt();
    }

    void setRect(QRect rect){
        this->rect = rect;
    }

    QRect getRect(){
        return rect;
    }

    void setDuration(DURATION duration){
        this->duration = duration;
    }

    void setLeft(int left){
        rect.moveLeft(left);
    }

    void setTop(int top){
        rect.moveTop(top);
    }

    void setWidth(int width){
        rect.setWidth(width);
    }

    void setHeight(int height){
        rect.setHeight(height);
    }

    void setOffsetX(const int &newValue){
        offset.setX(newValue);
    }

    void setOffsetY(const int &newValue){
        offset.setY(newValue);
    }

    QPoint getOffset(){
        return offset;
    }

    int getOffsetX(){
        return offset.x();
    }

    int getOffsetY(){
        return offset.y();
    }

    DURATION getDuration(){
        return duration;
    }

    QJsonObject emitJson(){
        QJsonObject jsonFrame;
        QJsonArray jsonRect{rect.left(), rect.top(), rect.width(), rect.height()};
        jsonFrame["rect"] = jsonRect;
        jsonFrame["offsetX"] = offset.x();
        jsonFrame["offsetY"] = offset.y();
        jsonFrame["duration"] = int(duration);
        return jsonFrame;
    }
};

#endif // FRAME_H
