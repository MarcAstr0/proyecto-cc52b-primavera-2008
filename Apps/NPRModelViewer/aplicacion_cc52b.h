#ifndef APPCC52B_H
#define APPCC52B_H

#include <QtGui/QMainWindow>
#include "ui_aplicacion_cc52b.h"
#include "CC52BWidget.h"

class AplicacionCC52B : public QMainWindow
{
    Q_OBJECT
    
    public:
        AplicacionCC52B(QWidget *parent = 0);
        ~AplicacionCC52B();
    
    signals:
        void sendModelToWidget(QString filename);
        void enviarArchivoDeImagenAVisor();
    
    private slots:
        void openModel();
        void guardarImagen();
    
    private:
        Ui::InterfazAplicacionCC52B ui;
        CC52BWidget *visor_principal;
    
        QMenu* menu_archivo;
        QAction* action_abrir;
        QAction* action_guardar_imagen;
};

#endif // APPCC52B_H
