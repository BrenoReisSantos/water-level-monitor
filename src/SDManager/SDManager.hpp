#ifndef _SD_MANAGER_H_
#define _SD_MANAGER_H_

#include <SD.h>
#include <string>

class SDManager
{
public:
    SDManager(int csPin);
    bool writeContentToFile(std::string fileName, std::string content);
    std::string getContentFromFile(std::string fileName);
    void copyFile(std::string originFileName, std::string destinationFileName);
};

#endif
