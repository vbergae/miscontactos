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
//  main.cpp
//  Estrada a MisContactos
//


#include <QtGui/QApplication>

#include "dbConnection.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    if (!createConnection())
    {
        return 1;
    }

    QCoreApplication::setOrganizationName("vbergae");
    QCoreApplication::setOrganizationDomain("victorberga.com");
    QCoreApplication::setApplicationName("Mis Contactos");
    QCoreApplication::setApplicationVersion("0.0.2");

    MainWindow w;
    w.setUnifiedTitleAndToolBarOnMac(true);
    w.show();
    return a.exec();
}
