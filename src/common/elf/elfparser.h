/*------------------------------------------------------------------------------
--  This file is a part of the SocExplorer Software
--  Copyright (C) 2013, Laboratory of Plasmas Physic - CNRS
--
--  This program is free software; you can redistribute it and/or modify
--  it under the terms of the GNU General Public License as published by
--  the Free Software Foundation; either version 3 of the License, or
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
--                     Mail : alexis.jeandet@lpp.polytechnique.fr
----------------------------------------------------------------------------*/
#ifndef ELFPARSER_H
#define ELFPARSER_H
#include <libelf.h>
#include <gelf.h>
#include <QString>
#include <stdio.h>
#include <QList>
#include <stdint.h>

class Elf_Section
{
public:
    Elf_Section(){}
    Elf_Section(Elf_Data*  data,GElf_Shdr* section_header)
    {
        this->data = data;
        this->section_header = section_header;
    }
    Elf_Data*  data;
    GElf_Shdr* section_header;
};

class elfparser
{
public:
    elfparser();
    bool iself();
    bool isopened();
    int setFilename(const QString& name);
    int closeFile();
    QString getArchitecture();
    QString getType();
    int32_t getVersion();
    int getSectioncount();
    int getSegmentcount();
    QString getSegmentType(int index);
    int64_t getSegmentOffset(int index);
    int64_t getSegmentVaddr(int index);
    int64_t getSegmentPaddr(int index);
    int64_t getSectionPaddr(int index);
    int64_t getSegmentFilesz(int index);
    int64_t getSectionDatasz(int index);
    bool getSectionData(int index, char **buffer);
    int64_t getSegmentMemsz(int index);
    int64_t getSectionMemsz(int index);
    QString getSegmentFlags(int index);
    QString getSectionName(int index);
    QString getSectionType(int index);
    static bool isElf(const QString& File);

private:
    void updateSections();
    void updateSegments();
    int elfFile;
    bool opened;
    bool type_elf;
    Elf* e;
    Elf_Kind ek;
    GElf_Ehdr ehdr;
    Elf_Scn * scn;
    Elf_Data * data;
    size_t SectionCount,SegmentCount, shstrndx;
    QList<GElf_Phdr*> Segments;
    //QList<GElf_Shdr*> Sections;
    QList<Elf_Section*> sections;

};

#endif // ELFPARSER_H







