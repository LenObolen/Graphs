#ifndef PARSER_H
#define PARSER_H

#include <QString>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <memory.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <any>

#pragma pack ( push , 1 )
struct headfile {
    char Regim[6];
    char AircraftName[8];
    char TypeWork;
    short int  NumWork;
    char DataWork[6];
    char PilotName[20];
    char Podves[35];
    short int  NumPar;
};
struct namespar {
    char  NamePar[6];
    char  NotaPar[6];
    char  DimPar;
    float Xmin;
    float Xmax;
};
struct nbytespar {
    short nBytePar;
    int   NumBytePar;
};
#pragma pack ( pop )

class parser
{
public:
    parser();
    void Data();
    void SetSourceFileName(QString name){
        source_file_name = name;
    }
    auto GetMassData(){
        return MassData;
    }
    auto GetNamesPar(){
        return NamesPar;
    }
    auto GetSubheadersSize(){
        return subheaders_size;
    }
    std::vector<std::any>* GetSubheaderByName(QString name);

private:
    QString source_file_name;
    std::vector<std::vector<std::any>> MassData;
    namespar* NamesPar;
    size_t subheaders_size;
};

#endif // PARSER_H
