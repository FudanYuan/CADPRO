#include "nf_tnf.h"

NF_Tnf::NF_Tnf()
{

}

NF_Reader *NF_Tnf::in(const std::string &file)
{
    FILE *fp;
    fp = fopen(file.c_str(), "rt");
    NF_Reader *nr;
    if (fp) {
        nr = new NF_Reader(fp);
        if(!nr->reader()){
            throw("read version error");
        };
        return nr;
    }
    throw("read version error");
    return NULL;
}

NF_Writer *NF_Tnf::out(const char *file, std::string version)
{
    char* f = new char[strlen(file)+1];
    strcpy(f, file);
    this->version = version;

    NF_Writer* nw = new NF_Writer(f, version);
    if (nw->openFailed()) {
        delete nw;
        delete[] f;
        return NULL;
    } else {
        delete[] f;
        return nw;
    }
}
