#ifndef UTIL_H
#define UTIL_H

#include <QString>

class Util
{
public:
    Util();
    static void QStringToUnsignedChar(const QString& str, unsigned char* &buf, unsigned long &len);
};

#endif // UTIL_H
