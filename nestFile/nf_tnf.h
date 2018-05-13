#ifndef NF_TNF_H
#define NF_TNF_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cstdio>
#include <cassert>
#include <cmath>
#include <sstream>
#include <map>
#include "iostream"

#include "nf_global.h"
#include "nf_writer.h"
#include "nf_reader.h"

class NF_Tnf
{
public:
    NF_Tnf();

    NF_Reader *in(const std::string& file);

    NF_Writer *out(const char* file, std::string version=NF_VERSION_1);

private:
    std::string version;
};

#endif // NF_TNF_H
