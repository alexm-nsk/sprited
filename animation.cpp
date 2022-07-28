#include "animation.h"

Animation::Animation(){

}

Animation::~Animation(){
    sequences.clear();
}

QString Animation::saveToJson(QString fileName){
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return "";
    QTextStream out(&file);
    QString result = "";
    QJsonObject animationJson;
    QJsonArray jsonSeqs;
    for (size_t i = 0; i < sequences.size(); i++){
        jsonSeqs.append(sequences[i].emitJson());
    }
    animationJson["sequences"] = jsonSeqs;
    QJsonDocument doc(animationJson);
    //QJsonDocument::Compact
    //QJsonDocument::Indented
    result = doc.toJson(QJsonDocument::Indented);
    out << result << "\n";
    file.close();
    return result;
}

void Animation::loadFromJson(QString fileName){
    sequences.clear();

    QFile file(fileName);

    file.open(QFile::ReadOnly);
    QJsonDocument doc =QJsonDocument::fromJson(file.readAll());

    QJsonObject seq = doc.object();
    bool val = seq.value("sequences").isArray();
    QJsonArray arr = seq.value("sequences").toArray();

    for (int i = 0 ; i < arr.size(); i++){
        Sequence newSequence(arr[i].toObject());
        sequences.push_back(newSequence);
    }

    file.close();
}
