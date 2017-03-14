#include "../../Zahir/Visualization/Renderers/Surface/FBORenderer.h"
#include "CC52BWidget.h"
#include <Zahir/Models/ModelFactory/ModelFactory.h>
#include <Zahir/Visualization/Matrices/MatrizDeTransformacion.h>
#include <iostream>
#include <Zahir/Visualization/Renderers/Surface/CgSurfaceRenderer.h>
#include <Zahir/Visualization/Renderers/Surface/Gooch/Gooch.h>
#include <Zahir/Visualization/Renderers/Surface/Phong/Phong.h>
#include <Zahir/Visualization/Renderers/Surface/Toon/Toon.h>
#include <Zahir/Visualization/Renderers/Surface/DepthMap/DepthMap.h>
#include <Zahir/Visualization/Renderers/Surface/NormalMap/NormalMap.h>

CC52BWidget::CC52BWidget(QWidget *parent): QGLWidget(parent), cg_context(CgSingleContext::Instance()) 
{
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);

    model_loaded = false;
    model = NULL;

    OpenMesh::Vec4f posicion_luz;
    posicion_luz[0] = 100.0f;
    posicion_luz[1] = 100.0f;
    posicion_luz[2] = 100.0f;
    posicion_luz[3] = 1.0f;
    luz = new FuenteDeLuz(GL_LIGHT0);
    luz->definirPosicion(posicion_luz);
    luz->definirIntensidadDifusa(1.0f, 1.0f, 1.0f);
    luz->definirIntensidadEspecular(1.0f, 1.0f, 1.0f);
    luz->definirIntensidadAmbiental(0.1f, 0.1f, 0.1f);
    
    id_graficador = 0;
    
    rotating = false;
    translating = false;
    
    fov = 45.0f;
    
    widget_width = 640;
    widget_height = 480;
}

void CC52BWidget::guardarImagen() 
{
    makeCurrent();
    QImage imagen = grabFrameBuffer(true);

    QString filename = QFileDialog::getSaveFileName(
        this,
        "Elije un archivo previo o ingresa uno nuevo",
        "/",
        "Imagen PNG (*.png)");
    if (filename != NULL)
        imagen.save(filename, "PNG");
}

CC52BWidget::~CC52BWidget() 
{ 
    delete fbo_renderer1;
    delete fbo_renderer2;
    for (unsigned int i = 0; i < renderers.size(); i++)
        delete renderers.at(i);
//     background_image.eliminarTextura();
    if (model != NULL)
        delete model;
    cg_context.destroyContext();
}

QSize CC52BWidget::minimumSizeHint() const {
    return QSize(640, 480);
}

QSize CC52BWidget::sizeHint() const {
    QWidget* parent = parentWidget();
    return QSize(parent->size());
}

void CC52BWidget::initializeGL() 
{
    glewInit(); //TODO: al usar glew hay que llamar siempre esta función. bueno saberlo. 
    //TODO: un método global "zahir init".
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glPointSize(10.0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glDepthFunc(GL_LEQUAL);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
    luz->prender();
    luz->entregarIntensidades();
    renderers.resize(5);
	
    /* my variables */
    
    cg_context.initContext();
    std::cerr << "CG Profiles: " << cg_context.vertexProfileName() << ", " << cg_context.fragmentProfileName() << std::endl;
    cg_context.checkForErrors("CC52BWidget", "initializeGL (iniciar contexto)");
    
    renderers[0] = new Phong();
    renderers[1] = new Gooch();
    renderers[2] = new Toon();
    renderers[3] = new DepthMap();
    renderers[4] = new NormalMap();

    cg_context.checkForErrors("CC52BWidget", "initializeGL (crear renderer)");
    
    fbo_renderer1 = new FBORenderer(renderers[3], widget_width, widget_height);
    fbo_renderer2 = new FBORenderer(renderers[4], widget_width, widget_height);
    
    cg_context.checkForErrors("CC52BWidget", "initializeGL (crear FBO renderer)");
  
    timer.start(16, this);
    tiempo.start();
}

void CC52BWidget::paintGL() 
{
    if (model == NULL)
        return;
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
	
//     image_renderer->graficar(background_image);
	
    camara.posicionarEnEscena();
    luz->entregarValores();
    
    MatrizDeTransformacion m =  
        MatrizDeTransformacion::MatrizDeTraslacion(translation_x, translation_y, translation_z)
        * MatrizDeTransformacion::MatrizDeTraslacion(object_pos[0], object_pos[1], object_pos[2])
        * MatrizDeTransformacion::MatrizDeRotacion(0.0f, rotation_y, rotation_z) 
        * MatrizDeTransformacion::MatrizDeTraslacion(-object_pos[0], -object_pos[1], -object_pos[2]);
    
    glMultMatrixf(&(m[0]));
    
    /* necesito la inversa de la matriz para algunos efectos gráficos */
    
    MatrizDeTransformacion inversa = m.obtenerInversa();
    
    OpenMesh::Vec4f posicion_camara_transformada = inversa * camara.obtenerPosicion();
    OpenMesh::Vec4f posicion_luz_transformada = inversa * luz->obtenerPosicion();
    
    luz->definirPosicionTransformada(posicion_luz_transformada);
    camara.definirPosicionTransformada(posicion_camara_transformada);
    
    switch (id_graficador)
    {
        case 0:
            (dynamic_cast<CgSurfaceRenderer*> (renderers[id_graficador]))->asignarCamara(camara);
            (dynamic_cast<CgSurfaceRenderer*> (renderers[id_graficador]))->asignarFuenteDeLuz(*luz);
            renderers[id_graficador]->graficar(model);
            break;
        case 1:
            (dynamic_cast<CgSurfaceRenderer*> (renderers[id_graficador]))->asignarCamara(camara);
            (dynamic_cast<CgSurfaceRenderer*> (renderers[id_graficador]))->asignarFuenteDeLuz(*luz);
            renderers[id_graficador]->graficar(model);
            break;
        case 2:
            (dynamic_cast<CgSurfaceRenderer*> (renderers[id_graficador]))->asignarCamara(camara);
            (dynamic_cast<CgSurfaceRenderer*> (renderers[id_graficador]))->asignarFuenteDeLuz(*luz);
            renderers[id_graficador]->graficar(model);
            break;
        case 3:
            (dynamic_cast<CgSurfaceRenderer*> (renderers[id_graficador]))->asignarCamara(camara);
            (dynamic_cast<CgSurfaceRenderer*> (renderers[id_graficador]))->asignarFuenteDeLuz(*luz);
            renderers[id_graficador]->graficar(model);
            break;
        case 4:
            (dynamic_cast<CgSurfaceRenderer*> (renderers[id_graficador]))->asignarCamara(camara);
            (dynamic_cast<CgSurfaceRenderer*> (renderers[id_graficador]))->asignarFuenteDeLuz(*luz);
            renderers[id_graficador]->graficar(model);
            break;
        case 5:
            (dynamic_cast<CgSurfaceRenderer*> (renderers[3]))->asignarCamara(camara);
            (dynamic_cast<CgSurfaceRenderer*> (renderers[3]))->asignarFuenteDeLuz(*luz);
            fbo_renderer1->graficar(model);
            break;
        case 6:
            (dynamic_cast<CgSurfaceRenderer*> (renderers[4]))->asignarCamara(camara);
            (dynamic_cast<CgSurfaceRenderer*> (renderers[4]))->asignarFuenteDeLuz(*luz);
            fbo_renderer2->graficar(model);
            break;
        default: // no debería suceder
            assert(false); 
    }
}

void CC52BWidget::resizeGL(int width, int height) 
{
    float fov = 45.0f;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov, float(width)/float(height), 0.1f, 2000.0f);
    glMatrixMode(GL_MODELVIEW);

    QWidget::resize(width, height);
    widget_width = width;
    widget_height = height;

}

void CC52BWidget::timerEvent(QTimerEvent* event) 
{
    if (event->timerId() == timer.timerId()) 
    {
//         updateGL();
//         float delta_t = tiempo.elapsed();
//         for(tree<Planeta>::iterator current_object = sistema.begin(); current_object != sistema.end(); current_object++) 
//         {
//             (*current_object).angulo += delta_t * angle_speed;
//             if ((*current_object).angulo >= 360.0f)
//                 (*current_object).angulo = 0.0f;
//         }
//         tiempo.restart();
//         updateGL();
    }
    else
        QWidget::timerEvent(event);
}

void CC52BWidget::loadModel(QString filename) 
{
    if (model != NULL) 
        delete model;
        
    char nombre_temporal[filename.length() + 1];
    
    for (int i = 0; i < filename.length(); i++)
        nombre_temporal[i] = filename.at(i).toAscii();
    
    nombre_temporal[filename.length()] = '\0';
    
    std::string nombre_de_archivo(nombre_temporal);
    
    model = ModelFactory::loadFromFile(nombre_de_archivo);
    
    object_pos[0] = model->centroid()[0];
    object_pos[1] = model->centroid()[1];
    object_pos[2] = model->centroid()[2];
    object_pos[3] = 1.0f;
    camara.asignarModelo3D(model);
    
    OpenMesh::Vec4f posicion_luz;
    posicion_luz[0] = model->boundingSphereRatio()*3.0f + model->centroid()[0];
    posicion_luz[1] = model->boundingSphereRatio()*3.0f + model->centroid()[1];
    posicion_luz[2] = model->boundingSphereRatio()*3.0f + model->centroid()[2];
    posicion_luz[3] = 1.0f;
    luz->definirPosicion(posicion_luz);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov, float(widget_width)/float(widget_height), 0.1f, model->boundingSphereRatio() * 20.0f);
    glMatrixMode(GL_MODELVIEW);
    
    translation_x = 0;
    translation_y = 0;
    translation_z = 0;
    
    mouse_pos_x = 0;
    mouse_pos_y = 0;
	   
    updateGL();
}


void CC52BWidget::mousePressEvent(QMouseEvent *event) 
{
    if (model == NULL) 
        return;
    Qt::MouseButtons state = event->buttons();
    
    rotating = state & Qt::LeftButton; // left button pressed
    translating = state & Qt::RightButton; // right button pressed

    updateGL();
}

void CC52BWidget::mouseReleaseEvent(QMouseEvent *event) 
{
    if (model == NULL) 
        return;
    Qt::MouseButtons state = event->buttons();
    if (state | Qt::LeftButton)
        rotating = false;
    if (state | Qt::RightButton)
        translating = false;
    updateGL();
}

void CC52BWidget::mouseMoveEvent(QMouseEvent *event) 
{
    if (model == NULL) 
        return;
    if (mouse_pos_x == 0 && mouse_pos_y == 0) 
    {
        mouse_pos_x = event->x();
        mouse_pos_y = event->y();
    } else {
        float delta_x = event->x() - mouse_pos_x;
        float delta_y = event->y() - mouse_pos_y;
        // TODO: normalizar los valores de acuerdo a algún criterio
        if (rotating) 
        {
            rotation_y += delta_y;
            rotation_z -= delta_x;
        } else if (translating) {
            translation_x += delta_x / widget_width * model->boundingSphereRatio();
            translation_y -= delta_y / widget_height * model->boundingSphereRatio();
        }
        mouse_pos_x = event->x();
        mouse_pos_y = event->y();
    }
    updateGL();
}

void CC52BWidget::wheelEvent(QWheelEvent *event) 
{
    if (model == NULL)
        return;
    int numDegrees = event->delta() / 8;
    int numSteps = numDegrees / 15;

    translation_z += float(numSteps) / 3.0f;

    event->accept();
    updateGL();
 }

void CC52BWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key_1:
            id_graficador = 0;
            break;
        case Qt::Key_2:
            id_graficador = 1;
            break;
        case Qt::Key_3:
            id_graficador = 2;
            break;
        case Qt::Key_4:
            id_graficador = 3;
            break;
        case Qt::Key_5:
            id_graficador = 4;
            break;
        case Qt::Key_6:
            id_graficador = 5;
            break;
        case Qt::Key_7:
            id_graficador = 6;
            break;
        default:
            keyPressEvent(event);
    }
    updateGL();
}