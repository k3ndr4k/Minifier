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

#ifndef IMAGECOMPRESS_IMAGECOMPRESSOR_HPP
#define IMAGECOMPRESS_IMAGECOMPRESSOR_HPP 1

#include <QString>
#include <QStringList>
#include <QProcess>

#include "utils.h"

namespace image_compressor{

class ImageCompressor{

public :

    ImageCompressor();

    ImageCompressor(ImageCompressor &&) = delete;
    ImageCompressor(const ImageCompressor &) = default;
    ImageCompressor &operator=(ImageCompressor &&) = delete;
    ImageCompressor &operator=(const ImageCompressor &) = default;
    ~ImageCompressor();

    bool canConvert(const QString &extension);
    void convert(const QString &filePath , QString &fileName, int quality);

private:
    QStringList _extList, _rawList, _pngList, _tifList, _psdList, _bmpList, _gifList, _icoList;
    QString _cmd;

};

} // End imageCompress

#endif
