#include "nf_reader.h"
#include <QDebug>

NF_Reader::NF_Reader(FILE *fp) :
    line(0),
    version("")
{
    this->fp = fp;
}

NF_Reader::~NF_Reader()
{

}

bool NF_Reader::reader()
{
    while(getStrippedLine(lineStr, NF_TNF_MAXLINE, fp)){
#ifdef DEBUGNFREADER
        qDebug() << QString::fromStdString(lineStr);
#endif
        bool ret = true;
        std::string errorMsg;
        line += 1;
        if(lineStr == (std::string)TAB + (std::string)VERSION){
            ret = readVersion(fp);
            if(!ret){
                errorMsg = "read version error";
            }
#ifdef DEBUGNFREADER
            qDebug() << "version: " << QString::fromStdString(version);
#endif
        }
        else if(lineStr == (std::string)TAB + (std::string)SHEET_COUNT){
            ret = readSheetCount(fp);
            if(!ret){
                errorMsg = "read sheet count error";
            }
#ifdef DEBUGNFREADER
            qDebug() << "sheet count: " << sheetCount;
#endif
        }
        else if(lineStr == (std::string)TAB + (std::string)SHEET_INFO){
            ret = readSheetList(fp);
            if(!ret){
                errorMsg = "read sheet info error";
            }
#ifdef DEBUGNFREADER
            qDebug() << "sheet info" ;
            for(int i=0; i<sheetList.length(); i++){
                qDebug() << "width: " << sheetList[i]->width;
                qDebug() << "height: " << sheetList[i]->height;
                qDebug() << "componentGap: " << sheetList[i]->componentGap;
                qDebug() << "leftMargin: " << sheetList[i]->leftMargin;
                qDebug() << "topMargin: " << sheetList[i]->topMargin;
                qDebug() << "rightMargin: " << sheetList[i]->rightMargin;
                qDebug() << "bottomMargin: " << sheetList[i]->bottomMargin;
                qDebug() << "type: " << sheetList[i]->type;
                if(sheetList[i]->type == Sheet::Whole){
                    qDebug() << "layers: " << sheetList[i]->layers;
                } else {
                    qDebug() << "margin: " << sheetList[i]->margin;
                    qDebug() << "strip length: " << sheetList[i]->stripPW.length();
                    for(int j=0; j<sheetList[i]->stripPW.length(); j++){
                        qDebug() << "position: " << sheetList[i]->stripPW[j].position;
                        qDebug() << "width: " << sheetList[i]->stripPW[j].width;
                    }
                }
            }
#endif
        }
        else if(lineStr == (std::string)TAB + (std::string)PIECE_CENTER_COUNT){
            ret = readPieceCenterCount(fp);
            if(!ret){
                errorMsg = "read piece center count error";
            }
#ifdef DEBUGNFREADER
            qDebug() << "piece center count: " << pieceCenterCount;
#endif
        }
        else if(lineStr == (std::string)TAB + (std::string)PIECE_CENTER_INFO){
            ret = readPieceCenterList(fp);
            if(!ret){
                errorMsg = "read piece center info error";
            }
#ifdef DEBUGNFREADER
            qDebug() << "piece center info: ";
            for(int i=0; i<pieceCenterList.length();i++){
                qDebug() << "id: " << pieceCenterList[i].id;
                qDebug() << "sheetId: " << pieceCenterList[i].sheetId;
                qDebug() << "type: " << pieceCenterList[i].type;
                qDebug() << "angle: " << pieceCenterList[i].angle;
                qDebug() << "reserve2: " << pieceCenterList[i].reserve2;
                qDebug() << "center point: " << pieceCenterList[i].getCenter();
            }
            qDebug() << "---------" << endl;
#endif
        }
        else if(lineStr == (std::string)TAB + (std::string)PIECE_OFFSET_COUNT){
            ret = readPieceOffsetCount(fp);
            if(!ret){
                errorMsg = "read piece offset count error";
            }
#ifdef DEBUGNFREADER
            qDebug() << "piece offset count: " << pieceOffsetCount;
#endif
        }
        else if(lineStr == (std::string)TAB + (std::string)PIECE_OFFSET_INFO){
            ret = readPieceOffsetList(fp);
            if(!ret){
                errorMsg = "read piece offset info error";
            }
#ifdef DEBUGNFREADER
            qDebug() << "piece offset info";
            for(int i=0; i<pieceOffsetList.length();i++){
                qDebug() << "typeId: " << pieceOffsetList[i].typeId;
                qDebug() << "count: " << pieceOffsetList[i].count;
                for(int j=0;j<pieceOffsetList[i].count;j++){
                    qDebug() << pieceOffsetList[i].pointsList[j].x << "  "
                             << pieceOffsetList[i].pointsList[j].y << "  "
                             << pieceOffsetList[i].pointsList[j].z;
                }
            }
#endif
        }

        if(!ret){
            throw(errorMsg);
            return false;
        }
        FILE *tmp = fp;
        getStrippedLine(lineStr, NF_TNF_MAXLINE, tmp, false);
        if(lineStr == "\n"){
            fp = tmp;
            line += 1;
        }
    }

    return true;
}

bool NF_Reader::readVersion(FILE *fp)
{
    if(getStrippedLine(lineStr, NF_TNF_MAXLINE, fp, false)){
        if(lineStr.size() != 0){
            version = lineStr;
        } else{
            version = "";
        }
        line += 1;
        return true;
    }
    return false;
}

bool NF_Reader::readSheetCount(FILE *fp)
{
    if(getStrippedLine(lineStr, NF_TNF_MAXLINE, fp, false)){
        if(lineStr.size() != 0){
            str2int(sheetCount, lineStr);
        } else{
            return false;
        }
        line += 1;
        return true;
    }
    return false;
}

bool NF_Reader::readSheetList(FILE *fp)
{
    int count = 0;
    while(count < sheetCount){
        Sheet* sheet = new Sheet;
        int i = 0;
        while(i < LINE_SHEET_INFO){
            getStrippedLine(lineStr, NF_TNF_MAXLINE, fp, false);
            switch (i) {
            case 1:
                str2double(sheet->width, lineStr);
                break;
            case 2:
                str2double(sheet->height, lineStr);
                break;
            case 3:
                str2double(sheet->componentGap, lineStr);
                break;
            case 4:
                str2double(sheet->leftMargin, lineStr);
                break;
            case 5:
                str2double(sheet->topMargin, lineStr);
                break;
            case 6:
                str2double(sheet->rightMargin, lineStr);
                break;
            case 7:
                str2double(sheet->bottomMargin, lineStr);
                break;
            case 8:
                int type_int;
                str2int(type_int, lineStr);
                sheet->type = (Sheet::SheetType)type_int;
                break;
            case 9:
                if(sheet->type == Sheet::Whole){
                    str2int(sheet->layers, lineStr);
                }
                if(sheet->type == Sheet::Strip){
                    str2double(sheet->margin, lineStr);
                }
                break;
            default:
                break;
            }
            i++;
        }

        line += i;
        if(sheet->type == Sheet::Whole){
            sheetList.append(sheet);
            return true;
        }

        // 条板数量
        getStrippedLine(lineStr, NF_TNF_MAXLINE, fp, false);
        int stripNum = 0;
        str2int(stripNum, lineStr);
        int stripIndex = 0;
        while(stripIndex <= stripNum){
            getStrippedLine(lineStr, NF_TNF_MAXLINE, fp, false);
            if(lineStr == ""){
                continue;
            }
            const char *sep = ",";
            char *tokenPtr = strtok((char*)lineStr.data(), sep);
            int temp = 0;
            double position, width;
            while(tokenPtr!=NULL) {
                if(temp == 0){
                    str2double(position, tokenPtr);
                }
                else if(temp == 1){
                    str2double(width, tokenPtr);
                }
                tokenPtr=strtok(NULL,",");
                temp++;
            }
            StripPW stripPWTmp(position, width);
            sheet->stripPW.append(stripPWTmp);
            stripIndex++;
        }

        sheetList.append(sheet);
        line += stripIndex;
        count++;
    }
    return true;
}

bool NF_Reader::readPieceCenterCount(FILE *fp)
{
    if(getStrippedLine(lineStr, NF_TNF_MAXLINE, fp, false)){
        if(lineStr.size() != 0){
            str2int(pieceCenterCount, lineStr);
        } else{
            return false;
        }
        line += 1;
        return true;
    }
    return false;
}

bool NF_Reader::readPieceCenterList(FILE *fp)
{
    int count = 0;
    while(count < pieceCenterCount){
        PieceCenter pieceCenter;
        int lineTmp = 0;
        while(lineTmp <= LINE_PIECE_CENTER_INFO){
            getStrippedLine(lineStr, NF_TNF_MAXLINE, fp, false);
            // qDebug() << lineTmp << " " << QString::fromStdString(lineStr);
            switch (lineTmp) {
            case 0:
                lineStr.replace(lineStr.begin(), lineStr.begin()+3, "");
                str2int(pieceCenter.id, lineStr);
                break;
            case 1:
                str2int(pieceCenter.sheetId, lineStr);
                break;
            case 2:
                str2int(pieceCenter.type, lineStr);
                break;
            case 3:
                str2double(pieceCenter.angle, lineStr);
                break;
            case 4:
                str2int(pieceCenter.reserve2, lineStr);
                break;
            case 5:
                str2double(pieceCenter.x, lineStr);
                break;
            case 6:
                str2double(pieceCenter.y, lineStr);
                break;
            case 7:
                break;
            default:
                break;
            }
            lineTmp++;
        }
        pieceCenterList.append(pieceCenter);
        line += lineTmp;
        count++;
    }
    return true;
}

bool NF_Reader::readPieceOffsetHeaderList(FILE *fp)
{
    Q_UNUSED(fp);
    return true;
}

bool NF_Reader::readPieceOffsetCount(FILE *fp)
{
    if(getStrippedLine(lineStr, NF_TNF_MAXLINE, fp, false)){
        if(lineStr.size() != 0){
            str2int(pieceOffsetCount, lineStr);
        } else{
            return false;
        }
        line += 1;
        return true;
    }
    return false;
}

bool NF_Reader::readPieceOffsetList(FILE *fp)
{
    int count = 0;
    while(count < pieceOffsetCount){
        PieceOffset pieceOffset;
        int lineTmp = 0;
        while(lineTmp < LINE_PIECE_OFFSET){
            getStrippedLine(lineStr, NF_TNF_MAXLINE, fp, false);
            switch (lineTmp) {
            case 0:
                str2int(pieceOffset.typeId, lineStr);
                break;
            case 1:
                str2double(pieceOffset.reserve1, lineStr);
                break;
            case 2:
                str2double(pieceOffset.reserve2, lineStr);
                break;
            case 3:
                str2double(pieceOffset.reserve3, lineStr);
                break;
            case 4:
                pieceOffset.reserve4 = lineStr;
                break;
            case 5:
                str2double(pieceOffset.reserve5, lineStr);
                break;
            case 6:
                str2int(pieceOffset.reserve6, lineStr);
                break;
            case 7:
                str2int(pieceOffset.count, lineStr);
               break;
            default:
                break;
            }
            lineTmp++;
        }

        line += lineTmp;

        // 获取点位
        lineTmp = 0;
        while(lineTmp <= pieceOffset.count){
            getStrippedLine(lineStr, NF_TNF_MAXLINE, fp, false);
            // 代表该字段结束
            if(lineStr == "\n"){
                continue;
            }
            char *tokenPtr = strtok((char*)lineStr.c_str(),",");
            int temp = 0;
            double px, py, pz;
            while(tokenPtr!=NULL) {
                if(temp == 0){
                    str2double(px, tokenPtr);
                }
                else if(temp == 1){
                    str2double(py, tokenPtr);
                }
                else if(temp == 2){
                    str2double(pz, tokenPtr);
                }
                tokenPtr=strtok(NULL,",");
                temp++;
            }
            PiecePointsList points(px, py, pz);
            pieceOffset.pointsList.append(points);
            lineTmp++;
        }
        pieceOffsetList.append(pieceOffset);
        line += lineTmp;
        count++;
    }
    return true;
}

bool NF_Reader::getStrippedLine(std::string &s, unsigned int size, FILE *fp, bool stripSpace)
{
    if (!feof(fp)) {
        // The whole line in the file.  Includes space for NULL.
        char* wholeLine = new char[size];
        // Only the useful part of the line
        char* line;

        line = fgets(wholeLine, size, fp);

        if (line != NULL && line[0] != '\0') { // Evaluates to fgets() retval
            // line == wholeLine at this point.
            // Both guaranteed to be NULL terminated.

            // Strip leading whitespace and trailing CR/LF.
            stripWhiteSpace(&line, stripSpace);

            s = line;
          //  assert(size > s.length());
        }

        delete[] wholeLine; // Done with wholeLine

        return true;
    } else {
        s = "";
        return false;
    }
}

/**
 * @brief Strips leading whitespace and trailing Carriage Return (CR)
 * and Line Feed (LF) from NULL terminated string.
 *
 * @param s Input and output.
 *      NULL terminates string.
 *
 * @retval true if \p s is non-NULL
 * @retval false if \p s is NULL
 */
bool NF_Reader::stripWhiteSpace(char **s, bool stripSpace)
{
    // last non-NULL char:
    int lastChar = strlen(*s) - 1;

    // Is last character CR or LF?
    while ( (lastChar >= 0) &&
            (((*s)[lastChar] == 10) || ((*s)[lastChar] == 13) ||
             (stripSpace && ((*s)[lastChar] == ' ' || ((*s)[lastChar] == '\t')))) ) {
        (*s)[lastChar] = '\0';
        lastChar--;
    }

    // Skip whitespace, excluding \t, at beginning of line
    if (stripSpace) {
        while ((*s)[0]==' ' || (*s)[0]=='\t') {
            ++(*s);
        }
    }

    return ((*s) ? true : false);
}

std::string NF_Reader::getVersion()
{
    return this->version;
}

QList<Sheet *> NF_Reader::getSheetList()
{
    return this->sheetList;
}

QList<PieceCenter> NF_Reader::getPieceCenterList()
{
    return this->pieceCenterList;
}

QList<PieceOffsetHeader> NF_Reader::getPieceOffsetHeaderList()
{
    return this->pieceOffsetHeaderList;
}

QList<PieceOffset> NF_Reader::getPieceOffsetList()
{
    return this->pieceOffsetList;
}
