#ifndef CRC32_H
#define CRC32_H
#include <QString>
#include <vector>
class crc32
{
public:
    crc32();
    unsigned int CRC32_function(unsigned char *buf, unsigned long len);
    unsigned int CRC32_count_qstring(QString str);
    unsigned int CRC32_count_int(std::vector<int>* ar);
    unsigned int CRC32_count_file(QString file_name);
};
#endif // CRC32_H
