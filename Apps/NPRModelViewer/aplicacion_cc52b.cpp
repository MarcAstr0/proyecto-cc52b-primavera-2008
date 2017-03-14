#include "aplicacion_cc52b.h"

AplicacionCC52B::AplicacionCC52B(QWidget *parent) : QMainWindow(parent) 
{
    ui.setupUi(this);

    visor_principal = new CC52BWidget();
    this->setCentralWidget(visor_principal);
    
    action_abrir = new QAction(tr("&Open..."), this);
    connect(action_abrir, SIGNAL(triggered()), this, SLOT(openModel()));
    connect(this, SIGNAL(sendModelToWidget(QString)), this->visor_principal, SLOT(loadModel(QString)));
    
    action_guardar_imagen = new QAction(tr("&Guardar Imagen"), this);
    connect(action_guardar_imagen, SIGNAL(triggered()), this, SLOT(guardarImagen()));
    
    ui.menuArchivo->addAction(action_abrir);
    ui.menuArchivo->addAction(action_guardar_imagen);
    connect(this, SIGNAL(enviarArchivoDeImagenAVisor()), this->visor_principal, SLOT(guardarImagen()));
}

AplicacionCC52B::~AplicacionCC52B() 
{
    delete visor_principal;
    delete action_abrir;
    delete action_guardar_imagen;
}

void AplicacionCC52B::guardarImagen() {
    emit enviarArchivoDeImagenAVisor();
}

void AplicacionCC52B::openModel()
{
    QString s = QFileDialog::getOpenFileName(
                    this,
                    "Select a 3D Model",
                    "",
                    "Wavefront OBJ (*.obj)");
    if (s != NULL)
        emit sendModelToWidget(s);
}