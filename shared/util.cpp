#include "util.h"

Util::Util() {}

void Util::QStringToUnsignedChar(const QString& str, unsigned char* &buf, unsigned long &len) {
    // Convert QString to QByteArray (UTF-8 encoded by default)
    QByteArray byteArray = str.toUtf8();

    // Get the length of the QByteArray
    len = static_cast<unsigned long>(byteArray.size());

    // Allocate memory for the buffer
    buf = new unsigned char[len];

    // Copy data from QByteArray to unsigned char buffer
    std::memcpy(buf, byteArray.constData(), len);
}
