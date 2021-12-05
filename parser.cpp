#include "parser.h"
#include <QDebug>

parser::parser()
{

}

void parser::Data(){
        headfile HeadFile; // заголовок файла
        //namespar* NamesPar; // имена переменных
        nbytespar* NBytesPar; // размер и положение переменных в файле(относительно начала данных)
        FILE* stream; // поток из файла
        int i;
        int  NumParKadr; // количество блоков данных в файле
        long long CalcNumParKadr; // количество блоков данных в файле вычисленое из размера файла
        int numread; // служебная переменная
        fpos_t DimFile; // размер файла
        int NumByteBlock = 0; // размер блока данных в байтах
        char** ByteArr; // массив данных из файла
        int row, column; // строка, столбец интересующего нас параметра
        if ((fopen_s(&stream, source_file_name.toLocal8Bit().data(), "rb")) != NULL) // открываем бинарный файл на чтение
        {

        }
        else
        {
            fseek(stream, 0, SEEK_END);
            fgetpos(stream, &DimFile);
            fseek(stream, 0, 0);

            numread = fread(&HeadFile, sizeof(char), 80, stream);
            subheaders_size = HeadFile.NumPar;
            NamesPar = new namespar[HeadFile.NumPar];
            NBytesPar = new nbytespar[HeadFile.NumPar];
            for (i = 0; i < HeadFile.NumPar; i++)
            {
                numread = fread(&NamesPar[i], sizeof(char), 21, stream);
                switch (NamesPar[i].DimPar)
                {
                case 'd':
                    NBytesPar[i].nBytePar = 8;
                    break;
                case 'e':
                    NBytesPar[i].nBytePar = 4;
                    break;
                case 'm':
                    NBytesPar[i].nBytePar = 2;
                    break;
                case 'p':
                    NBytesPar[i].nBytePar = 4;

                    break;
                case 'i':
                    NBytesPar[i].nBytePar = 2;
                    break;
                default:
                    ;
                }
                NumByteBlock += NBytesPar[i].nBytePar;
                NBytesPar[i].NumBytePar = NumByteBlock - NBytesPar[0].nBytePar;
            }
            CalcNumParKadr = (DimFile - (80 + HeadFile.NumPar * 21 + 4)) / NumByteBlock;
            numread = fread(&NumParKadr, sizeof(char), 4, stream);
            // сравнивать CalcNumParKadr и NumParKadr, поэтому
            // нужно проверить, есть ли К
            // сравнивая строки, чтобы их длина и содержимое было одинаковыми

            MassData.resize(HeadFile.NumPar);
            ByteArr = new char* [CalcNumParKadr];
            for (i = 0; i < CalcNumParKadr; i++)
            {
                ByteArr[i] = new char[NumByteBlock];
                numread = fread(ByteArr[i], sizeof(char), NumByteBlock, stream);
                for (auto j = 0; j < HeadFile.NumPar; ++j)
                {
                    switch (NamesPar[j].DimPar)
                    {
                        case 'd':
                            //double 8 Byte
                            double dData;
                            memcpy(&dData, ByteArr[i] + NBytesPar[j].NumBytePar, 8);
                            MassData[j].push_back(dData);
                            break;
                        case 'e':
                            //float 4 Byte
                            float eData;
                            memcpy(&eData, ByteArr[i] + NBytesPar[j].NumBytePar, 4);
                            MassData[j].push_back(eData);
                            break;
                        case 'm':
                            //short 2 Byte
                            short mData;
                            memcpy(&mData, ByteArr[i] + NBytesPar[j].NumBytePar, 2);
                            MassData[j].push_back(mData);
                            break;
                        case 'p':
                            //int 4 Byte
                            int pData;
                            memcpy(&pData, ByteArr[i] + NBytesPar[j].NumBytePar, 4);
                            MassData[j].push_back(pData);
                            break;
                        case 'i':
                            //short 2 Byte
                            short iData;
                            memcpy(&iData, ByteArr[i] + NBytesPar[j].NumBytePar, 2);
                            MassData[j].push_back(iData);
                            break;
                        default:
                                ;
                        }
                    }
               }
        }
}

std::vector<std::any>* parser::GetSubheaderByName(QString name)
{
    std::vector<std::any>* data;
    qDebug() << name;
    for (auto i = 0; i < subheaders_size; ++i)
    {
        char fur[7];
        strncpy_s(fur, NamesPar[i].NamePar, 6);
        if ((strcmp(fur, name.toStdString().c_str()) == 0))
        {
            qDebug() << fur;
            qDebug() << name.toStdString().c_str();
            data = &MassData[i];
            break;
        }
    }
    return data;
}
