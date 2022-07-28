#ifndef ANIMATION_H
#define ANIMATION_H

#include <sequence.h>

class Animation
{

private:

    vector<Sequence> sequences;
    QString name;
    QString currentSequence;
    size_t currentFrame;
    size_t currentSequenceIndex;

public:

    void setLoopStart(size_t newValue){
        if (currentSequenceIndex < sequences.size()){
            sequences[currentSequenceIndex].setLoopStart(newValue);
        }
    }

    void setLoopEnd(size_t newValue){
        if (currentSequenceIndex < sequences.size()){
            sequences[currentSequenceIndex].setLoopEnd(newValue);
        }
    }

    void setLoopEnabled(bool newValue){
        if (currentSequenceIndex < sequences.size()){
            sequences[currentSequenceIndex].setLoopEnabled(newValue);
        }
    }

    size_t getLoopStart(){
        if (currentSequenceIndex < sequences.size()){
            return sequences[currentSequenceIndex].getLoopStart();
        }
        return 0;
    }

    size_t getLoopEnd(){
        if (currentSequenceIndex < sequences.size()){
            return sequences[currentSequenceIndex].getLoopEnd();
        }
        return 0;
    }

    bool getLoopEnabled(){
        if (currentSequenceIndex < sequences.size()){
            return sequences[currentSequenceIndex].getLoopEnabled();
        }
        return 0;
    }


    void setCurrentSequence(size_t index){
        currentSequenceIndex = index;
    }

    void setCurrentFrame(size_t &newIndex){
        currentFrame = newIndex;
        sequences[currentSequenceIndex].setCurrentFrame(currentFrame);
    }

    int addFrame(Frame &frame){
        if (currentSequenceIndex < sequences.size()){
            sequences[currentSequenceIndex].addFrame(frame);
            // return the index of added frame:
            return sequences[currentSequenceIndex].getLength() - 1;
        }
        else
            return -1;
    }

    size_t addSequence(QString name = ""){
        Sequence newSequence(name);
        sequences.push_back(newSequence);
        return sequences.size() - 1;
    }

    void setLeft(const int &newValue){
        if (currentSequenceIndex < sequences.size())
            sequences[currentSequenceIndex].setLeft(newValue);
    }

    void setTop(const int &newValue){
        if (currentSequenceIndex < sequences.size())
            sequences[currentSequenceIndex].setTop(newValue);
    }

    void setWidth(const int &newValue){
        if (currentSequenceIndex < sequences.size())
            sequences[currentSequenceIndex].setWidth(newValue);
    }

    void setHeight(const int &newValue){
        if (currentSequenceIndex < sequences.size())
            sequences[currentSequenceIndex].setHeight(newValue);
    }

    void setDuration(const int &newValue){
        if (currentSequenceIndex < sequences.size())
            sequences[currentSequenceIndex].setDuration(newValue);
    }

    int getDuration(){
        if (currentSequenceIndex < sequences.size())
            return sequences[currentSequenceIndex].getFrame(currentFrame).getDuration();
        else
            return 0;
    }

    void setFrameOffsetX(const int &newValue){
        if (currentSequenceIndex < sequences.size())
            sequences[currentSequenceIndex].setFrameOffsetX(newValue);
    }

    void setFrameOffsetY(const int &newValue){
        if (currentSequenceIndex < sequences.size())
            sequences[currentSequenceIndex].setFrameOffsetY(newValue);
    }

    QPoint getOffset(){
            return sequences[currentSequenceIndex].getOffset();
    }

    int getOffsetX(){
            return sequences[currentSequenceIndex].getOffsetX();
    }

    int getOffsetY(){
            return sequences[currentSequenceIndex].getOffsetY();
    }

    QPoint getFrameOffset(){
        if(currentSequenceIndex < sequences.size())
            return sequences[currentSequenceIndex].getFrameOffset(currentFrame);
        else
            return QPoint(0, 0);
    }

    void setSequenceOffsetX(const int &newValue){
        if (currentSequenceIndex < sequences.size())
            sequences[currentSequenceIndex].setOffsetX(newValue);
    }

    void setSequenceOffsetY(const int &newValue){
        if (currentSequenceIndex < sequences.size())
            sequences[currentSequenceIndex].setOffsetY(newValue);
    }

    void setSequenceName(QString name){
        if (currentSequenceIndex < sequences.size())
            sequences[currentSequenceIndex].setName(name);
    }

    QString getSequenceName(){
        return sequences[currentSequenceIndex].getName();
    }

    QString getSequenceName(size_t index){
        return sequences[index].getName();
    }

    size_t getSequenceIndexByName(QString name){
        for (size_t i = 0; i < sequences.size(); i++)
            if (sequences[i].getName() == name)
                return i;
        return -1;
    }

    size_t getSequenceLength(){
        return sequences[currentSequenceIndex].getLength();
    }

    void setName(QString newName){
        name = newName;
    }

    QString getName(){
        return name;
    }

    Frame getFrame(size_t sequenceIndex, size_t frameIndex){
        return sequences[sequenceIndex].getFrame(frameIndex);
    }

    Frame getFrame(size_t frameIndex){
        if(currentSequenceIndex < sequences.size() && frameIndex < sequences[currentSequenceIndex].getLength())
            return sequences[currentSequenceIndex].getFrame(frameIndex);
        else
            return Frame();
    }

    size_t getNumSequences(){
        return sequences.size();
    }

    // TODO makeFromJsonObject for all classes

    QString saveToJson(QString fileName);

    void loadFromJson(QString fileName);

    Animation();
    ~Animation();

};

#endif // ANIMATION_H
