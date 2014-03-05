/*------------------------------------------------------------------------------
--  This file is a part of the SocExplorer Software
--  Copyright (C) 2013, Plasma Physics Laboratory - CNRS
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
--                     Mail : alexis.jeandet@lpp.polytechnique.fr
----------------------------------------------------------------------------*/
#ifndef XMLDATA_H
#define XMLDATA_H
#include <QString>
#include <QList>
#include <QObject>

class xmlDataBitfield: public QObject
{
    Q_OBJECT
public:
    explicit xmlDataBitfield(QObject* parent =0);
    explicit xmlDataBitfield(const QString& name,int offset,const QString& description,QObject* parent =0);
    QString name();
    int offset();
    QString description();
public slots:
    void setName(const QString& name);
    void setOffset(int offset);
    void setDescription(const QString& description);

signals:
    void nameChanged(const QString& name);
    void offsetChanged(int offset);
    void descriptionChanged(const QString& description);
private:

    QString p_name;
    int p_offset;
    QString p_description;
};



class xmlDataRegister : public QObject, public QList<xmlDataBitfield*>
{
    Q_OBJECT
public:
    xmlDataRegister(QObject* parent=0);
    quint32 value();
public slots:
    void setName(const QString& name);
    void setValue(quint32 value);
    void setOffset(qint64 offset);
signals:
    void nameChanged(const QString& name);
    void offsetChanged(qint64 offset);
    void valueChanged(quint32 value);
private:
    QString p_name;
    qint64  p_addressOffset;
    quint32 p_value;
};

class xmlDataPeripheral:public QObject, public QList<xmlDataRegister*>
{
    Q_OBJECT
public:
    xmlDataPeripheral(QObject* parent=0);
private:
    QString p_name;
    qint64  p_baseAddress;
};



class xmlDataSystemOnChip : public QObject, public QList<xmlDataRegister*>
{
    Q_OBJECT
public:
    xmlDataSystemOnChip(QObject* parent=0);
public slots:
    void setName(const QString& name);
signals:
    void nameChanged(const QString& name);
private:
    QString p_name;
};


#endif // XMLDATA_H







