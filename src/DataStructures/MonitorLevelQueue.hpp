#ifndef MONITOR_LEVEL_QUEUE_H
#define MONITOR_LEVEL_QUEUE_H

#include <string>
#include <vector>
#include <stdio.h>

class MonitorLevelQueue
{
    int maxSize;
    std::vector<int> queueArray;

public:
    MonitorLevelQueue(int maxSize);
    float average();
    void queue(int item);
    int dequeue();
    std::string toString();
};

#endif
