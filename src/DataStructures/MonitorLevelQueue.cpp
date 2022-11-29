#include "MonitorLevelQueue.hpp"

MonitorLevelQueue::MonitorLevelQueue(int maxSize)
{
    this->maxSize = maxSize;
};

float MonitorLevelQueue::average()
{
    int total = 0;
    int size = queueArray.size();
    for (int i = 0; i < size; i++)
        total += queueArray[i];
    return (float)total / (float)size;
};

void MonitorLevelQueue::queue(int item)
{
    if (queueArray.size() == maxSize)
    {
        dequeue();
        queueArray.push_back(item);
        return;
    }
    queueArray.push_back(item);
};

int MonitorLevelQueue::dequeue()
{
    if (queueArray.size() > 0)
    {
        int value = queueArray[0];
        queueArray.erase(queueArray.begin());
        return value;
    }
    std::__throw_runtime_error("fila vazia.");
};

std::string MonitorLevelQueue::toString()
{
    std::string text = "[";
    int size = queueArray.size();
    for (int i = 0; i < size; i++)
    {
        char *itemString;
        if (i < size - 1)
            sprintf(itemString, "%d, ", queueArray[i]);
        else
            sprintf(itemString, "%d", queueArray[i]);
        text += itemString;
    }
    return text;
};
