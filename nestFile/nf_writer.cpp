#include "nf_writer.h"

NF_Writer::NF_Writer(const char *fname, std::string ver) :
    version(ver),
    m_ofile(fname)
{

}

bool NF_Writer::openFailed() const
{
    return m_ofile.fail();
}

void NF_Writer::close() const
{
    m_ofile.close();
}

void NF_Writer::writeHeader(std::string header, std::string version)
{
    m_ofile << TAB << VERSION << ENDL;
    m_ofile << header << " ";
    m_ofile << version << ENDL;
    m_ofile << ENDL;
}

void NF_Writer::writeSheet(const QList<Sheet*> &sheetList)
{
    int len = sheetList.length();
    m_ofile << TAB << SHEET_COUNT << ENDL;
    m_ofile << len << ENDL;
    m_ofile << ENDL;

    m_ofile << TAB << SHEET_INFO << ENDL;
    for(int i=0; i<len; i++){
        Sheet sheet = *sheetList[i];
        m_ofile << i << ENDL;
        m_ofile << sheet.width << ENDL;
        m_ofile << sheet.height << ENDL;
        m_ofile << sheet.componentGap << ENDL;
        m_ofile << sheet.leftMargin << ENDL;
        m_ofile << sheet.topMargin << ENDL;
        m_ofile << sheet.rightMargin << ENDL;
        m_ofile << sheet.bottomMargin << ENDL;
        switch (sheet.type) {
        case Sheet::Whole:
            m_ofile << 0 << ENDL;
            m_ofile << sheet.layers << ENDL;
            break;
        case Sheet::Strip:
            m_ofile << 1 << ENDL;
            m_ofile << sheet.margin << ENDL;
            m_ofile << sheet.stripPW.length() << ENDL;
            for(int i=0; i<sheet.stripPW.length(); i++){
                m_ofile << sheet.stripPW[i].position
                        << ","
                        << sheet.stripPW[i].width
                        << ENDL;
            }
            break;
        default:
            break;
        }

        m_ofile << ENDL;
    }
}

void NF_Writer::writePieceCenterHeader(const int count)
{
    m_ofile << TAB << PIECE_CENTER_COUNT << ENDL;
    m_ofile << count << ENDL;
    m_ofile << ENDL;
}

void NF_Writer::writePieceCenter(const QList<PieceCenter> &pieceCenterList)
{
    m_ofile << TAB << PIECE_CENTER_INFO << ENDL;
    for(int i=0; i<pieceCenterList.length(); i++){
        PieceCenter pieceCenter = pieceCenterList[i];
        m_ofile << TAB << pieceCenter.id << ENDL;
        m_ofile << pieceCenter.sheetId << ENDL;
        m_ofile << pieceCenter.type << ENDL;
        m_ofile << pieceCenter.angle << ENDL;
        m_ofile << pieceCenter.reserve2 << ENDL;
        m_ofile << pieceCenter.x << ENDL;
        m_ofile << pieceCenter.y << ENDL;
        m_ofile << ENDL;
    }
    m_ofile << ENDL;
}

void NF_Writer::writePieceOffsetHeader(const int count)
{
    m_ofile << TAB << PIECE_OFFSET_COUNT << ENDL;
//    m_ofile << pieceOffsetHeader.reserve1 << ENDL;
//    m_ofile << pieceOffsetHeader.layer << ENDL;
//    m_ofile << pieceOffsetHeader.type << ENDL;
//    m_ofile << pieceOffsetHeader.sheet << ENDL;
//    m_ofile << pieceOffsetHeader.typeNum << ENDL;
//    m_ofile << pieceOffsetHeader.reserve2 << ENDL;
    m_ofile << count << ENDL;
    m_ofile << ENDL;
}

void NF_Writer::writePieceOffset(const QList<PieceOffset> &pieceOffsetList)
{
    m_ofile << TAB << PIECE_OFFSET_INFO << ENDL;
    for(int i=0; i<pieceOffsetList.length(); i++){
        PieceOffset pieceOffset = pieceOffsetList[i];
        m_ofile << pieceOffset.typeId << ENDL;
        m_ofile << pieceOffset.reserve1 << ENDL;
        m_ofile << pieceOffset.reserve2 << ENDL;
        m_ofile << pieceOffset.reserve3 << ENDL;
        m_ofile << pieceOffset.reserve4 << ENDL;
        m_ofile << pieceOffset.reserve5 << ENDL;
        m_ofile << pieceOffset.reserve6 << ENDL;
        m_ofile << pieceOffset.count << ENDL;
        for(int i=0; i<pieceOffset.pointsList.length(); i++){
            m_ofile << pieceOffset.pointsList[i].x << ","
                    << pieceOffset.pointsList[i].y << ","
                    << pieceOffset.pointsList[i].z << ENDL ;

        }
        m_ofile << ENDL;
    }
}
