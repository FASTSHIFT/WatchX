#include "EEPROM_File.h"

bool EEPROM_File::begin(const char* dir, const char* fileName, uint32_t size)
{
//    if(!SD.exists(dir))
//    {
//        SD.mkdir(dir);
//    }
//    String path = String(dir) + "/" + String(fileName);
//    bool retval = file.open(path.c_str(), O_RDWR | O_CREAT);
//    if(retval)
//    {
//        while(file.fileSize() < size)
//        {
//            file.write((uint8_t)0);
//        }
//    }
//    
//    return retval;
    return false;
}

void EEPROM_File::end()
{
//    file.close();
}
    
uint8_t EEPROM_File::read(uint32_t idx)
{
//    uint8_t retval = 0;

//    if(file.isOpen() && idx < file.fileSize())
//    {
//        file.seekSet(idx);
//        retval = file.read();
//    }
        
    return 0;
}

void EEPROM_File::write(uint32_t idx, uint8_t val)
{
//    if(file.isOpen() && idx < file.fileSize())
//    {
//        file.seekSet(idx);
//        file.write(val);
//    }
}

void EEPROM_File::update(uint32_t idx, uint8_t val)
{
//    if(read(idx) != val)
//    {
//        write(idx, val);
//    }
}
