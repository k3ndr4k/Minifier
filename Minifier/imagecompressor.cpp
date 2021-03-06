/*******************************************************************
Copyright (C) 2018 Oscar MARIE--TAILLEFER   <o5mariet@enib.fr>
Copyright (C) 2018 Nils JEGOU-GERGAUD       <n5jegoug@enib.fr>
Copyright (C) 2018 Nicols FRANCIS           <n4franci@enib.fr>
Copyright (C) 2018 Corentin LAMBERT         <c4lamber@enib.fr>
This file is part of Minifier.
Minifier is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
Minifier is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with Minifier.  If not, see <http://www.gnu.org/licenses/>.
*******************************************************************/

#include "imagecompressor.h"

image_compressor::ImageCompressor::ImageCompressor()
{
    // List of extension available
    this->_pngList << "*.png";
    this->_tifList << "*.tif" << "*.tiff";
    this->_psdList << "*.psd";
    this->_bmpList << "*.bmp" << "*.rle" << "*.dib";
    this->_gifList << "*.gif";
    this->_icoList << "*.ico";

    this->_extList = this->_pngList + this->_tifList + this->_psdList + this->_bmpList + this->_gifList + this->_icoList;

    this->count = 0;
}

image_compressor::ImageCompressor::~ImageCompressor()
{

}

/**
 * @brief ImageCompressor::canConvert
 * @param extension
 * @return True|False if extension is available to convert
 */
bool image_compressor::ImageCompressor::canConvert(const QString &extension)
{
    return !(this->_extList.filter(extension, Qt::CaseInsensitive).isEmpty());
}

/**
 * @brief inList test if a file get his extension in a list
 * @param filePath filepath
 * @param list list of extension to test
 * @return true|false if file's extension is in list
 */
bool static inList(const QString &filePath, const QStringList &list)
{
    QString ext = filePath.split('.').last();
    return !(list.filter(ext, Qt::CaseInsensitive).isEmpty());
}

/**
 * @brief makeCmd Build a cmd
 * @param exe extension for launch .exe ( for exemple jpg => jpg2png.exe)
 * @param filePath filepath
 * @param fileName output fileName
 * @param quality quality level
 * @return cmd built
 */
inline QString image_compressor::ImageCompressor::makeCmd(const QString &exe, const QString &filePath, const QString &fileName, const int &quality)
{
    QString cmd = ExePath() + "convert/" + exe +"2jpg.exe";
    QStringList cmdArg;

    QStringList cmd_to_test;
    cmd_to_test << cmd << filePath << fileName;
    cmdCheck(&cmd_to_test);

    cmdArg << "-i " + cmd_to_test.at(1) << "-q" << "--filename-mask=\"" + cmd_to_test.at(2) + "\"" << "--jpg-quality=" + QString::number(quality);
    return cmd_to_test.at(0) + " " + cmdArg.join(' ');
}

/**
 * @brief makeCmd Build a cmd
 * @param exe extension for launch .exe ( for exemple jpg => jpg2png.exe)
 * @param filePath filepath
 * @param quality quality level
 * @return cmd built
 */
inline QString image_compressor::ImageCompressor::makeCmd(const QString &exe, const QString &filePath, const int &quality)
{
    QString cmd = ExePath() + "convert/" + exe +"2jpg.exe";
    QStringList cmdArg;

    QStringList cmd_to_test;
    cmd_to_test << cmd << filePath;
    cmdCheck(&cmd_to_test);

    cmdArg << "-i " + cmd_to_test.at(1) << "-q" << "--jpg-quality=" + QString::number(quality);
    return cmd_to_test.at(0) + " " + cmdArg.join(' ');
}

/** 
 * @brief  return filePath without fileName
 * @param  &filePath: filePath with fileName
 * @retval filePath without fileName
 */
static QString getFilePath(const QString &filePath)
{
    QStringList path = filePath.split('/');
    QStringList fp;
    for(int i = 0; i < path.size()-1; i++)
    {
        fp << path.at(i);
    }
    return fp.join('/');
}

/**
 * @brief convert algo to create and launch convertissor and compressor
 * @param filePath input filePath
 * @param fileName output File's Name
 * @param quality [0-100] % quality for compression (default 70)
 */
void image_compressor::ImageCompressor::convert(const QString &filePath , QString &fileName, int quality)
{
    if(quality == 0)
    {
        quality = 100;
    }

    QRegExp re_jpg("*.jpg", Qt::CaseInsensitive);
    re_jpg.setPatternSyntax(QRegExp::Wildcard);
    if(re_jpg.exactMatch(filePath) == true)
    {
        this->_cmd = ExePath() + "convert.exe";

        QStringList cmd_to_test ;
        cmd_to_test.clear();
        cmd_to_test << this->_cmd << filePath << fileName;

        cmdCheck(&cmd_to_test);

        QStringList cmdArg;
        // Argument use by convert.exe
        cmdArg << "-strip" << "-interlace Plane" << "-quality " + QString::number(quality) + "%%" << cmd_to_test.at(1) << cmd_to_test.at(2);

        // Build cmd / Sturct : %exe% + %cmd argument%
        this->_cmd = cmd_to_test.at(0) + " " + cmdArg.join(' ');
    } else if( inList(filePath , this->_pngList )) {
        this->_cmd = makeCmd("png", filePath, fileName, quality);
    } else if( inList(filePath , this->_tifList )) {
        this->_cmd = makeCmd("tiff", filePath, fileName, quality);
    } else if( inList(filePath , this->_psdList )) {
        this->_cmd = makeCmd("psd", filePath, fileName, quality);
    } else if( inList(filePath , this->_bmpList )) {
        this->_cmd = makeCmd("bmp", filePath, fileName, quality);
    } else if( inList(filePath , this->_gifList )) {
        this->_cmd = makeCmd("gif", filePath, fileName, quality);
    } else if( inList(filePath , this->_icoList )) {
        this->_cmd = makeCmd("ico", filePath, fileName, quality);
    }

    this->bat_name = ExePath() + "tmp/" + QString::number(this->count) + ".bat";
    this->count += 1;

    // Set bat's name
    this->bat.setFileName(this->bat_name);

    // Write cmd to compress png in bat file
    if(!this->bat.open(QIODevice::WriteOnly) )
       return;

    this->bat.write(this->_cmd.toUtf8().constData());
    this->bat.close();

    // Launch cmd
    this->launch();

    // Clear cmd to avoid error
    this->_cmd = "";
}

/**
 * @brief convert algo to create and launch convertissor and compressor
 * @param filePath input filePath
 * @param quality [0-100] % quality for compression (default 70)
 */
void image_compressor::ImageCompressor::convert(const QString &filePath, int quality)
{
    if(quality < 30)
    {
        quality = 30;
    }

    QRegExp re_jpg("*.jpg", Qt::CaseInsensitive);
    re_jpg.setPatternSyntax(QRegExp::Wildcard);
    if(re_jpg.exactMatch(filePath) == true)
    {
        this->_cmd = ExePath() + "convert.exe";

        QStringList cmd_to_test ;
        cmd_to_test.clear();
        cmd_to_test << this->_cmd << filePath;
        
        cmdCheck(&cmd_to_test);
        
        QStringList cmdArg;
        // Argument use by convert.exe 
        cmdArg << "-strip" << "-interlace Plane" << "-quality " + QString::number(quality) + "%%" << cmd_to_test.at(1) << cmd_to_test.at(1);

        // Build cmd / Sturct : %exe% + %cmd argument%
        this->_cmd = cmd_to_test.at(0) + " " + cmdArg.join(' ');
    } else if( inList(filePath , this->_pngList )) {
        this->_cmd = makeCmd("png", filePath, quality);
    } else if( inList(filePath , this->_tifList )) {
        this->_cmd = makeCmd("tiff", filePath, quality);
    } else if( inList(filePath , this->_psdList )) {
        this->_cmd = makeCmd("psd", filePath, quality);
    } else if( inList(filePath , this->_bmpList )) {
        this->_cmd = makeCmd("bmp", filePath, quality);
    } else if( inList(filePath , this->_gifList )) {
        this->_cmd = makeCmd("gif", filePath, quality);
    } else if( inList(filePath , this->_icoList )) {
        this->_cmd = makeCmd("ico", filePath, quality);
    }

    this->bat_name = ExePath() + "tmp/" + QString::number(this->count) + ".bat";
    this->count += 1;

    // Set bat's name
    this->bat.setFileName(this->bat_name);

    // Write cmd to compress png in bat file
    if(!this->bat.open(QIODevice::WriteOnly) )
       return;

    this->bat.write(this->_cmd.toUtf8().constData());
    this->bat.close();

    // Launch cmd
    this->launch();

    // Clear cmd to avoid error
    this->_cmd = "";
}

void image_compressor::ImageCompressor::launch()
{
    QProcess p;
    p.setWorkingDirectory(ExePath() + "tmp/");
    p.start(bat_name);

    p.waitForFinished();
}
