#ifndef QINT_H
#define QINT_H

#include <QObject>

class QInt : public QObject
{
   Q_OBJECT
public:
   explicit QInt(QObject *parent = nullptr);
   ~QInt() { m_value = 0; }
   int value() const { return m_value; }
public slots:
   void setValue(int value);
signals:
   void valueChanged(int newValue);

private:
   int m_value;
};


#endif // QINT_H
