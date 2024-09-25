#include <iostream>
#include <QFile>
#include <QString>
#include "crc32.h"
#include "util.h"

using namespace std;

crc32::crc32() {}

unsigned int crc32::CRC32_function(unsigned char *buf, unsigned
                                   long len)
{
    unsigned long crc_table[256];
    unsigned long crc;
    for (int i = 0; i < 256; i++)
    {
        crc = i;
        for (int j = 0; j < 8; j++)
            crc = crc & 1 ? (crc >> 1) ^ 0xEDB88320UL : crc >> 1;
        crc_table[i] = crc;
    };
    crc = 0xFFFFFFFFUL;
    while (len--)
        crc = crc_table[(crc ^ *buf++) & 0xFF] ^ (crc >> 8);
    return crc ^ 0xFFFFFFFFUL;
}

unsigned int crc32::CRC32_count_qstring(QString str)
{
    unsigned char* buffer = nullptr;
    unsigned long length = 0;

    Util::QStringToUnsignedChar(str, buffer, length);

    crc32 crc32;
    unsigned int sum = crc32.CRC32_function(buffer, length);

    delete[] buffer;
    return sum;
}

// возвращание конечного CRC32. Достаточно вызвать эту функцию и
// указать имя файла, для которого будет произведён расчёт
unsigned int crc32::CRC32_count_file(QString file_name)
{
    QFile f(file_name);
    f.open(QIODevice::ReadOnly);
    if (!f.isOpen()) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }
    unsigned long file_size = f.size();
    char* buf = new char[file_size]; //сколько символов в файле
    f.read(buf, file_size);
    unsigned int res = CRC32_function((unsigned char*)buf,
                                      file_size);
    f.close();
    delete[] buf;
    return res;
}

unsigned int crc32::CRC32_count_int(std::vector<int>* ar)
{
    string bufstr;
    for(size_t i = 0; i < ar->size(); i++){
        bufstr += to_string((*ar)[i]);
    }
    const char* buf = bufstr.c_str();
    return CRC32_function((unsigned char*)buf, bufstr.length());
}
