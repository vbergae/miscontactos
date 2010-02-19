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
//  mainwindow.cpp
//  Ventana Principal de Mis Contactos
//
//  Muestra la lista de contactos y permite realizar las operaciones
//  para ver, crear, editar y eliminarlos.
//

#include <QtGui>

#include "mainwindow.h"
#include "ui_mainwindow.h"

//  CONSTRUCTOR
//  Instala la ventana principal
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //Crea la interfaz
    ui->setupUi(this);
    setWindowTitle(QCoreApplication::applicationName());


    //Establece los parametros y botones mostrados por defecto al iniciar
    nuevo = false;

    ui->guardarButton->setVisible(false);
    ui->cancelarButton->setVisible(false);
    ui->eliminarButton->setVisible(false);

    //Estable conexion con la tabla contactos
    model = new QSqlTableModel;
    model->setTable("contactos");
    model->setSort(1,Qt::AscendingOrder);
    model->select();

    if (model->lastError().type() != QSqlError::NoError)
        QMessageBox::warning(this,tr("Error"),model->lastError().text());

    //Visualiza los contactos en listaContactos
    ui->listaContactos->setModel(model);
    ui->listaContactos->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->listaContactos->setModelColumn(1);
    ui->listaContactos->setCursor(Qt::PointingHandCursor);

    //Configura el mapeador para mostrar los datos de la tabla en los Widgets
    mapper = new QDataWidgetMapper;
    mapper->setModel(model);
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper->addMapping(ui->nombreLine,1);
    mapper->addMapping(ui->aliasLine,2);
    mapper->addMapping(ui->fijoLine,3);
    mapper->addMapping(ui->movilLine,4);
    mapper->addMapping(ui->fijo2Line,5);
    mapper->addMapping(ui->movil2Line,6);
    mapper->addMapping(ui->correoLine,7);
    //mapper->addMapping(); MENSAJERIA
    mapper->addMapping(ui->urlLine,9);
    mapper->addMapping(ui->direccionLine,10);
    mapper->setCurrentModelIndex(model->index(0,1));

    //Crea las conexiones entre widgets y establece las reglas de validacion para los widgets
    createConnections();
    createValidators();


}

//  Activa los campos del formulario
void MainWindow::activarCampos()
{
    ui->nombreLine->setReadOnly(false);
    ui->nombreLine->setFocus();
    ui->nombreLine->setFrame(true);

    ui->aliasLine->setReadOnly(false);
    ui->aliasLine->setFrame(true);

    ui->fijoLine->setReadOnly(false);
    ui->fijoLine->setFrame(true);

    ui->movilLine->setReadOnly(false);
    ui->movilLine->setFrame(true);

    ui->fijo2Line->setReadOnly(false);
    ui->fijo2Line->setFrame(true);

    ui->movil2Line->setReadOnly(false);
    ui->movil2Line->setFrame(true);

    ui->correoLine->setReadOnly(false);
    ui->correoLine->setFrame(true);

    ui->urlLine->setReadOnly(false);
    ui->urlLine->setFrame(true);

    ui->direccionLine->setReadOnly(false);
    ui->direccionLine->setFrame(true);

}

//  Desactiva los campos del formulario
void MainWindow::desactivarCampos()
{
    ui->guardarButton->setVisible(false);
    ui->editarButton->setVisible(true);

    ui->nombreLine->setReadOnly(true);
    ui->nombreLine->setFrame(false);

    ui->aliasLine->setReadOnly(true);
    ui->aliasLine->setFrame(false);

    ui->fijoLine->setReadOnly(true);
    ui->fijoLine->setFrame(false);

    ui->movilLine->setReadOnly(true);
    ui->movilLine->setFrame(false);

    ui->fijo2Line->setReadOnly(true);
    ui->fijo2Line->setFrame(false);

    ui->movil2Line->setReadOnly(true);
    ui->movil2Line->setFrame(false);

    ui->correoLine->setReadOnly(true);
    ui->correoLine->setFrame(false);

    ui->urlLine->setReadOnly(true);
    ui->urlLine->setFrame(false);

    ui->direccionLine->setReadOnly(true);
    ui->direccionLine->setFrame(false);
}

//  Crea las reglas de validacion para los widgets
void MainWindow::createValidators()
{

    QRegExp nombreRx(".{2,50}");
    ui->nombreLine->setValidator(new QRegExpValidator(nombreRx, this));
    ui->aliasLine->setValidator(new QRegExpValidator(nombreRx, this));

    QRegExp movilRx("6\\d{8}");
    ui->movilLine->setValidator(new QRegExpValidator(movilRx,this));
    ui->movil2Line->setValidator(new QRegExpValidator(movilRx, this));

    QRegExp fijoRx("9\\d{8}");
    ui->fijoLine->setValidator(new QRegExpValidator(fijoRx,this));
    ui->fijo2Line->setValidator(new QRegExpValidator(fijoRx, this));


    QRegExp emailRx("[a-z0-9!#$%&'*+/=?^_`{|}~-]+(?:\\.[a-z0-9!#$%&'*+/=?^_`{|}~-]+)*@(?:[a-z0-9](?:[a-z0-9-]*[a-z0-9])?\\.)+[a-z0-9](?:[a-z0-9-]*[a-z0-9])?");
    ui->correoLine->setValidator(new QRegExpValidator(emailRx, this));

    QRegExp urlRx("(ftp|http|https):\\/\\/(\\w+:{0,1}\\w*@)?(\\S+)(:[0-9]+)?(\\/|\\/([\\w#!:.?+=&%@!\\-\\/]))?");
    ui->urlLine->setValidator(new QRegExpValidator(urlRx, this));
}

//  Valida los widgets
bool MainWindow::validateFields()
{
    if (!ui->nombreLine->hasAcceptableInput())
    {
        QMessageBox::warning(this,tr("Nombre de contacto no valido"), tr("El nombre del contacto debe tener entre 2 y 50 caracteres"));
        ui->nombreLine->setFocus();
        return false;
    }

    if (ui->aliasLine->text().length() > 0)
    {
        if (!ui->aliasLine->hasAcceptableInput())
        {
            QMessageBox::warning(this,tr("El alias no es valido"), tr("El alias debe tener entre 2 y 50 caracteres"));
            ui->aliasLine->setFocus();
            return false;
        }
    }

    if (ui->fijoLine->text().length() > 0)
    {
        if (!ui->fijoLine->hasAcceptableInput())
        {
            QMessageBox::warning(this,tr("Telefono no valido"), "El telefono fijo 1 no es valido");
            ui->fijoLine->setFocus();
            return false;
        }
    }

    if (ui->fijo2Line->text().length() > 0)
    {
        if (!ui->fijo2Line->hasAcceptableInput())
        {
            QMessageBox::warning(this,tr("Telefono2 no valido"), "El telefono fijo 2 no es valido");
            ui->fijo2Line->setFocus();
            return false;
        }
    }

    if (ui->movilLine->text().length() > 0)
    {
        if (!ui->movilLine->hasAcceptableInput())
        {
            QMessageBox::warning(this,tr("Telefono no valido"), "El telefono movil 1 no es valido");
            ui->movilLine->setFocus();
            return false;
        }
    }

    if (ui->movil2Line->text().length() > 0)
    {
        if (!ui->movil2Line->hasAcceptableInput())
        {
            QMessageBox::warning(this,tr("Telefono no valido"), "El telefono movil 2 no es valido");
            ui->movil2Line->setFocus();
            return false;
        }
    }

    if (ui->correoLine->text().length() > 0)
    {
        if (!ui->correoLine->hasAcceptableInput())
        {
            QMessageBox::warning(this,tr("Email no valido"), "El formato correcto para el Email es usuario@dominio.com");
            ui->correoLine->setFocus();
            return false;
        }
    }

    if (ui->urlLine->text().length() > 0)
    {
        if (!ui->urlLine->hasAcceptableInput())
        {
            QMessageBox::warning(this,tr("URL no valida"), "El formato correcto para la URL es http://www.paginadelcontacto.com");
            ui->correoLine->setFocus();
            return false;
        }
    }


    return true;
}

//  Configura las conexiones entre widgets
void MainWindow::createConnections()
{
    connect(ui->listaContactos,SIGNAL(clicked(QModelIndex)),mapper,SLOT(setCurrentModelIndex(QModelIndex)));
    connect(ui->editarButton, SIGNAL(clicked()),this,SLOT(activarForm()));
    connect(ui->guardarButton, SIGNAL(clicked()),this,SLOT(guardarForm()));
    connect(ui->nuevoButton, SIGNAL(clicked()),this,SLOT(nuevoContacto()));
    connect(ui->cancelarButton, SIGNAL(clicked()),this,SLOT(cancelarEdicion()));
    connect(ui->eliminarButton, SIGNAL(clicked()),this,SLOT(eliminarContacto()));
    connect(ui->buscarLine, SIGNAL(textChanged(QString)),this,SLOT(buscarContacto(QString)));
    connect(ui->irButton, SIGNAL(clicked()), this, SLOT(irUrl()));
    connect(ui->emailButton, SIGNAL(clicked()), this, SLOT(irEmail()));
}

//  Activa el formulario para editar o crear
void MainWindow::activarForm()
{
    ui->editarButton->setVisible(false);
    ui->guardarButton->setVisible(true);
    ui->nuevoButton->setVisible(false);
    ui->cancelarButton->setVisible(true);
    ui->eliminarButton->setVisible(true);

    activarCampos();
}

//  Guarda los datos de los widgets en la BD
void MainWindow::guardarForm()
{

    if (validateFields())
    {
        if (nuevo == false)
        {

            mapper->submit();

            if(model->lastError().type() != QSqlError::NoError)
                QMessageBox::warning(this,tr("Error"),model->lastError().text());

            desactivarCampos();
            ui->cancelarButton->setVisible(false);
            ui->eliminarButton->setVisible(false);
            ui->nuevoButton->setVisible(true);
        }
        else
        {
            int row = model->rowCount();
            model->insertRows(row, 1);
            model->setData(model->index(row, 1), ui->nombreLine->text());
            model->setData(model->index(row, 2), ui->aliasLine->text());
            model->setData(model->index(row, 3), ui->fijoLine->text());
            model->setData(model->index(row, 4), ui->movilLine->text());
            model->setData(model->index(row, 5), ui->fijo2Line->text());
            model->setData(model->index(row, 6), ui->movil2Line->text());
            model->setData(model->index(row, 7), ui->correoLine->text());
            model->setData(model->index(row, 9), ui->urlLine->text());
            model->setData(model->index(row, 10), ui->direccionLine->text());
            model->submitAll();

            if(model->lastError().type() != QSqlError::NoError)
                QMessageBox::warning(this,tr("Error"),model->lastError().text());

            desactivarCampos();
            ui->cancelarButton->setVisible(false);
            ui->eliminarButton->setVisible(false);
            ui->nuevoButton->setVisible(true);

            mapper->setCurrentModelIndex(model->index(row,1));
            nuevo = false;

        } //End isNew

    } //End validateFields
}

//  Prepara el formulario para crear un nuevo contacto
void MainWindow::nuevoContacto()
{

    nuevo = true;
    ui->editarButton->setVisible(false);
    ui->nuevoButton->setVisible(false);
    ui->guardarButton->setVisible(true);
    ui->cancelarButton->setVisible(true);

    activarCampos();

    ui->nombreLine->setText("Nuevo Contacto");
    ui->nombreLine->selectAll();
    ui->aliasLine->setText("");
    ui->fijoLine->setText("");
    ui->movilLine->setText("");
    ui->fijo2Line->setText("");
    ui->movil2Line->setText("");
    ui->correoLine->setText("");
    ui->urlLine->setText("");
    ui->direccionLine->setText("");
}

//  Cancela la edicion de un contacto y desactiva el formulario
void MainWindow::cancelarEdicion()
{
    nuevo = false;
    ui->editarButton->setVisible(true);
    ui->nuevoButton->setVisible(true);
    ui->guardarButton->setVisible(false);
    ui->cancelarButton->setVisible(false);
    ui->eliminarButton->setVisible(false);

    desactivarCampos();

    //mapper->setCurrentModelIndex(model->index(0,1));
}

//  Elimina un contacto de la BD
void MainWindow::eliminarContacto()
{
    QMessageBox msgBox;
    msgBox.setText(tr("¿Seguro que deseas eliminar el contacto?"));
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    msgBox.setIcon(QMessageBox::Warning);
    int execMsgBox = msgBox.exec();

    switch(execMsgBox)
    {
        case QMessageBox::Ok:
            model->removeRow(mapper->currentIndex());

            if (model->lastError().type() != QSqlError::NoError)
            {
                QMessageBox::critical(this,tr("Error"),model->lastError().text());
            }

            ui->editarButton->setVisible(true);
            ui->nuevoButton->setVisible(true);
            ui->guardarButton->setVisible(false);
            ui->cancelarButton->setVisible(false);
            ui->eliminarButton->setVisible(false);

            desactivarCampos();


        default:
            break;
    }
}

//  Realiza las busqueda de un contacto en listaContactos
void MainWindow::buscarContacto(QString busqueda)
{

    if (busqueda != "")
    {
        model->setFilter(QString("nombreCompleto LIKE '%" + busqueda + "%'"));
    }
    else
    {
        model->setFilter(QString(""));
    }

    qDebug() << "Filter: " << model->filter();
}

//  Abre el navegador por defecto para ver la URL  del contacto
void MainWindow::irUrl()
{
    QDesktopServices::openUrl(QUrl(ui->urlLine->text()));
}

//  Abre el cliente de correo por defecto para escribir un email al contacto
void MainWindow::irEmail()
{
    QDesktopServices::openUrl("mailto:" + ui->correoLine->text());
}

//  DESTRUCTOR
MainWindow::~MainWindow()
{
    delete ui;
}

//  Maneja los eventos Qt
void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
