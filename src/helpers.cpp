#include "helpers.h"


//_____________________________________________________________________________
void getActualDataNumber(int* actualDataArr, int16** pData, int bufferSize, int nBlocks) {
    for (int k = 0; k < nBlocks ; k++) {
        for (int i = 0; i < bufferSize - 10; i++) {
            bool actualDataEnd = true;
            for (int j = i + 1; j < i + 10; j++) {
                if (pData[k][j] != 0) {
                    actualDataEnd = false;
                }
            }
            if (actualDataEnd) {
                actualDataArr[k] = i + 1;
            }
        }
    }
}

//_____________________________________________________________________________
int getNumberOfLines(std::string filename) {
    std::string line;
    std::ifstream myfile(filename);
    int numberOfLines = 0;

    if(myfile.is_open()){
        while(!myfile.eof()){
            getline(myfile,line);
            numberOfLines++;
        }
        myfile.close();
    }
    return numberOfLines;
}

//_____________________________________________________________________________
void readMockUpAlignedData(int16** dataArr, std::string filename)
{
    std::string line;
    std::ifstream file (filename);

    if (file.is_open())
    {
        int lineNumber = 0;
        while (getline(file,line))
        {
            std::string tmp;
            std::stringstream ss(line);
            int k = 0;
            int j = 0;
            while(getline(ss, tmp, ',')) {
                if (k != 0 && !ss.eof()) {
                    int entry = stoi(tmp);
                    dataArr[lineNumber][j] = entry;
                    j++;
                }
                k++;
            }
            lineNumber++;
        }
        file.close();
    }
}
