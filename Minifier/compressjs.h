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

#ifndef compress_js_hpp
#define compress_js_hpp 1

#include <Qfilesystemwatcher>
#include <QProcess>

#include "utils.h"
#include "config.h"
#include "Compressfile.h"

namespace code_compressor {

    class CompressJS : public code_compressor::CompressFile
    {
        Q_OBJECT
    public:
        CompressJS(const QString &filePath);
        CompressJS(CompressJS &&) = delete;
        CompressJS(const CompressJS &) = default;
        CompressJS &operator=(CompressJS &&) = delete;
        CompressJS &operator=(const CompressJS &) = default;
        ~CompressJS() override;

        void compress() override;
        void stop() override;

        QFileSystemWatcher* fileWatch;

    public slots:
        UINT CompressCallback();
    };

} //end code_compressor

#endif
