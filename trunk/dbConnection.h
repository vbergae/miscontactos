//  Copyright 2010 Victor Berga <vbergae@gmail.com>
//
//  This file is part of MisContactos.
//
//  MisContactos is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  MyContacts is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with MisContactos.  If not, see <http://www.gnu.org/licenses/>.
//
//  dbConnection.h
//  Conecta a la base de datos
//

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QtGui/QApplication>
#include <QMessageBox>
#include <QDesktopServices>
#include <QDir>

#ifndef BASEDATOS_H
#define BASEDATOS_H

//  Crea la conexion a la BD y crea la tabla contactos si no existe
static bool createConnection()
{

    QString directorio = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
    QDir dir(directorio);
    QString dbName = dir.filePath("MisContactosDb.sqlite");

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);

    //qDebug() << dbName;
    if (!db.open()) {
        QMessageBox::critical(0, qApp->tr("Error al abrir la Agenda"),
            qApp->tr("Ha sido imposible abrir los contactos de la Agenda"),
                     QMessageBox::Ok);

        return false;
    }

    QSqlQuery query;

    query.exec("create table if not exists contactos (id integer primary key autoincrement, "
           "nombreCompleto varchar(50), "
           "alias varchar(50), "
           "telefonoFijo varchar(9), "
           "telefonoMovil varchar(9), "
           "telefonoFijo2 varchar(9), "
           "telefonoMovil2 varchar(9), "
           "correo varchar(100), "
           "mensajeria varchar(100), "
           "url varhcar(255), "
           "direccion varchar(150))");

//    if(query.lastError().type() != QSqlError::NoError)
//    {
//        qDebug() << "Contacos: " << query.lastError().text();
//    }

    return true;

}


#endif // BASEDATOS_H
