/*------------------------------------------------------------------------------
--  This file is a part of the SocExplorer Software
--  Copyright (C) 2014, Plasma Physics Laboratory - CNRS
--
--  This program is free software; you can redistribute it and/or modify
--  it under the terms of the GNU General Public License as published by
--  the Free Software Foundation; either version 2 of the License, or
--  (at your option) any later version.
--
--  This program is distributed in the hope that it will be useful,
--  but WITHOUT ANY WARRANTY; without even the implied warranty of
--  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--  GNU General Public License for more details.
--
--  You should have received a copy of the GNU General Public License
--  along with this program; if not, write to the Free Software
--  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
-------------------------------------------------------------------------------*/
/*--                  Author : Alexis Jeandet
--                     Mail : alexis.jeandet@member.fsf.org
----------------------------------------------------------------------------*/
#ifndef ABSTRACTBINFILE_H
#define ABSTRACTBINFILE_H

#include <QtCore/QObject>

class codeFragment
{
public:
    codeFragment();
    codeFragment(char* data, quint64 size, quint64 address):data(data),size(size),address(address){}
    QString header;
    char* data;
    quint64 size;
    quint64 address;
};

class abstractBinFile : public QObject
{
    Q_OBJECT
public:
//    virtual abstractExecFile()=0;
    virtual bool openFile(const QString& File)=0;
    virtual bool isopened()=0;
    virtual int closeFile()=0;
    virtual QList<codeFragment*> getFragments()=0;

protected:
     QString p_fileName;
};

#endif // ABSTRACTBINFILE_H