#include "qint.h"

QInt::QInt(QObject *parent) : QObject(parent)
{
   m_value = 0;
}

void QInt::setValue(int value)
{
   if (m_value != value) {
     m_value = value;
     emit valueChanged(value);
   }
}
