#ifndef SEQUENCE_H
#define SEQUENCE_H


#include <qstring.h>
#include <qpoint.h>
#include <vector>
#include <frame.h>
#include <json/json.h>

using namespace std;

class Sequence
{
private:
    vector<Frame> frames;
    QString name;
    QPoint offset;
    size_t currentFrameIndex;
    bool loopEnabled;
    size_t loopStart, loopEnd;
public:

    ~Sequence(){
        frames.clear();
    }

    QString getName(){
        return name;
    }

    Sequence(QJsonObject obj){
        name = obj["name"].toString();
        offset.setX(obj["offsetX"].toInt());
        offset.setY(obj["offsetY"].toInt());
        loopEnabled = obj["loopEnabled"].toBool();
        loopStart = obj["loopStart"].toInt();
        loopEnd = obj["loopEnd"].toInt();
        QJsonArray framesJsonArray = obj["frames"].toArray();
        for (int i = 0; i < framesJsonArray.size(); i++){
            frames.push_back(Frame(framesJsonArray[i].toObject()));
        }

    }

    Sequence(QString seqName){
        name = seqName;
        Frame firstFrame;
        addFrame(firstFrame);
        currentFrameIndex = 0;
        loopStart = 0;
        loopEnd = 0;
        loopEnabled = false;
    }

    void setName(QString newName){
        name = newName;
    }

    void addFrame(Frame frame){
        frames.push_back(frame);
    }

    void setLoopStart(size_t start){
        loopStart = start;
    }

    void setLoopEnd(size_t end){
        loopEnd = end;
    }

    void setLoopEnabled(bool newValue){
        loopEnabled = newValue;
    }

    size_t getLoopStart(){
        return loopStart;
    }

    size_t getLoopEnd(){
        return loopEnd;
    }

    bool getLoopEnabled(){
        return loopEnabled;
    }

    void setCurrentFrame(size_t frame){
        if(currentFrameIndex < getLength())
            currentFrameIndex = frame;
        else
            currentFrameIndex = getLength() - 1;
    }

    void setLeft(const int &newValue){
        if (currentFrameIndex < frames.size())
            frames[currentFrameIndex].setLeft(newValue);
    }

    void setTop(const int &newValue){
        if (currentFrameIndex < frames.size())
            frames[currentFrameIndex].setTop(newValue);
    }

    void setWidth(const int &newValue){
        if (currentFrameIndex < frames.size())
            frames[currentFrameIndex].setWidth(newValue);
    }

    void setHeight(const int &newValue){
        if (currentFrameIndex < frames.size())
            frames[currentFrameIndex].setHeight(newValue);
    }

    void setDuration(const int &newValue){
        if (currentFrameIndex < frames.size()){
            frames[currentFrameIndex].setDuration(newValue);
        }
    }

    void setFrameOffsetX(const int &newValue){
        if (currentFrameIndex < frames.size())
            frames[currentFrameIndex].setOffsetX(newValue);
    }

    void setFrameOffsetY(const int &newValue){
        if (currentFrameIndex < frames.size())
            frames[currentFrameIndex].setOffsetY(newValue);
    }

    void setOffsetX(const int &newValue){
        offset.setX(newValue);
    }

    void setOffsetY(const int &newValue){
        offset.setY(newValue);
    }

    QPoint getFrameOffset(size_t frameIndex){
        if (frameIndex < frames.size())
            return frames[frameIndex].getOffset();
        else
            return QPoint(0, 0);
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

    Frame getFrame(size_t frameIndex){
        if (frameIndex < frames.size())
            return frames[frameIndex];
        else
            return Frame();
    }

    void removeFrame(unsigned int index){
        frames.erase(frames.begin() + index);
    }

    unsigned int getLength()    {
        return frames.size();
    }

    /*QString emitJson(){
        if (0 < frames.size()){
            frames[0].emitJson();
        }
        return "";
    }*/

    QJsonObject emitJson(){
        QJsonObject jsonSeq;

        QJsonArray jsonFrames;
        for (size_t i = 0; i < frames.size(); i++){
            jsonFrames.append(frames[i].emitJson());
        }
        jsonSeq["frames"]      = jsonFrames;
        jsonSeq["offsetX"]     = offset.x();
        jsonSeq["offsetY"]     = offset.y();
        jsonSeq["loopStart"]   = int(loopStart);
        jsonSeq["loopEnd"]     = int(loopEnd);
        jsonSeq["loopEnabled"] = loopEnabled;
        jsonSeq["name"]        = name;

        return jsonSeq;
    }


};

#endif // SEQUENCE_H
