#include "SDManager.hpp"

SDManager::SDManager(int csPin)
{
    SD.begin(csPin);
};

bool SDManager::writeContentToFile(std::string fileName, std::string content)
{
    String fileNameString = String(fileName.c_str());
    File file;
    if (SD.exists(fileNameString))
    {
        file = SD.open(fileNameString, FILE_APPEND);
        if (file.availableForWrite())
        {
            file.println(content.c_str());
        }
        file.close();
        return true;
    }
    else
    {
        file = SD.open(fileNameString, FILE_WRITE);
        if (file.availableForWrite())
        {
            file.println(content.c_str());
        }
        file.close();
        return true;
    }
    return false;
};

std::string SDManager::getContentFromFile(std::string fileName)
{
    String fileNameString = String(fileName.c_str());
    File file;
    if (SD.exists(fileNameString))
    {
        file = SD.open(fileNameString, FILE_READ);
        std::string content = "";
        while (file.available())
            content += file.readString().c_str();
        file.close();
        return content;
    }
    std::__throw_runtime_error("file doesn't exist.");
}

void SDManager::copyFile(std::string originFileName, std::string destinationFileName)
{
    String originFileNameString = String(originFileName.c_str());
    String destinationFileNameString = String(destinationFileName.c_str());
    File originFile;
    File destinationFile;

    if (SD.exists(originFileNameString))
    {
        originFile = SD.open(originFileNameString, FILE_READ);
        destinationFile = SD.open(destinationFileNameString, FILE_WRITE);
        // std::string content = "";
        while (originFile.available())
            destinationFile.print(originFile.readString());
        originFile.close();
    }
    std::__throw_runtime_error("file doesn't exist.");
};
