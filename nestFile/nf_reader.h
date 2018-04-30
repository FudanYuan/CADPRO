#ifndef NF_READER_H
#define NF_READER_H

#include "debug.h"
#include "nf_global.h"
#include "nf_struct.h"
#include "common.h"
#include <string>

class NF_Reader
{
public:
    NF_Reader(FILE *fp);
    ~NF_Reader();

    bool reader();
    bool readVersion(FILE *fp);
    bool readSheetCount(FILE *fp);
    bool readSheetList(FILE *fp);
    bool readPieceCenterCount(FILE *fp);
    bool readPieceCenterList(FILE *fp);
    bool readPieceOffsetHeaderList(FILE *fp);
    bool readPieceOffsetCount(FILE *fp);
    bool readPieceOffsetList(FILE *fp);

    bool getStrippedLine(std::string& s, unsigned int size,
                               FILE* fp, bool stripSpace = true);

    bool stripWhiteSpace(char** s, bool stripSpace);

    std::string getVersion();
    QList<Sheet*> getSheetList();
    QList<PieceCenter> getPieceCenterList();
    QList<PieceOffsetHeader> getPieceOffsetHeaderList();
    QList<PieceOffset> getPieceOffsetList();

private:
    FILE *fp;
    int line;
    std::string lineStr;
    std::string version;
    int sheetCount;
    QList<Sheet*> sheetList;
    int pieceCenterCount;
    QList<PieceCenter> pieceCenterList;
    QList<PieceOffsetHeader> pieceOffsetHeaderList;
    int pieceOffsetCount;
    QList<PieceOffset> pieceOffsetList;
};

#endif // NF_READER_H
