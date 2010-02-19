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
//  mainwindow.h
//  Ventana Principal de Mis Contactos
//
//  Muestra la lista de contactos y permite realizar las operaciones
//  para ver, crear, editar y eliminarlos.
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QDataWidgetMapper>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;
    QSqlTableModel *model;
    QDataWidgetMapper *mapper;

    void activarCampos();
    void desactivarCampos();

    void createValidators();
    void createConnections();

    bool validateFields();

    bool nuevo;

private slots:
    void activarForm();
    void guardarForm();
    void nuevoContacto();
    void cancelarEdicion();
    void eliminarContacto();
    void buscarContacto(QString busqueda);
    void irUrl();
    void irEmail();
};

#endif // MAINWINDOW_H
