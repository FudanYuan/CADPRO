#ifndef NF_WRITER_H
#define NF_WRITER_H

#include "nf_global.h"
#include "nf_struct.h"

class NF_Writer
{
public:

    NF_Writer(const char* fname,
              std::string ver = NF_VERSION_1);

    bool openFailed() const;
    void close() const;

    void writeHeader(std::string header, std::string version = NF_VERSION_1);
    void writeSheet(const QList<Sheet*> &sheetList);
    void writePieceCenterHeader(const int count);
    void writePieceCenter(const QList<PieceCenter> &pieceCenterList);
    void writePieceOffsetHeader(const int count);
    void writePieceOffset(const QList<PieceOffset> &pieceOffsetList);

protected:
    std::string version;

private:
    mutable std::ofstream m_ofile;
};

#endif // NF_WRITER_H
