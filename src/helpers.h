#include <sstream>
#include <fstream>
#include <vector>
#include <EthernetStandAlone.h>

#ifndef HELPERS_H
#define HELPERS_H

void getActualDataNumber(int* actualDataArr, int16** pData, int bufferSize, int nBlocks);

int getNumberOfLines(std::string filename);

void readMockUpAlignedData(int16** dataArr, std::string filename);

#endif //HELPERS_H
