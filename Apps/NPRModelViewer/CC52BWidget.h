#ifndef CC52BWIDGET_H_
#define CC52BWIDGET_H_

#include <QtGui>
#include <QtOpenGL>
#include <QBasicTimer>
#include <QTime>
#include <list>
#include <map>

#include <Zahir/Generics/Tree/tree.hh>
#include <Zahir/Models/Modelo3D.h>
#include <Zahir/Visualization/Renderers/SurfaceRenderers.h>
#include <Zahir/Visualization/Camara.h>
#include <Zahir/Visualization/FuenteDeLuz.h>
#include <Zahir/Visualization/GPU/CgWrapper.h>
#include "../../Zahir/Visualization/Renderers/Image/ImageRenderer.h"

class CC52BWidget: public QGLWidget 
{
    Q_OBJECT

    public:
        CC52BWidget(QWidget *parent = 0);
        virtual ~CC52BWidget();
    
        QSize minimumSizeHint() const;
        QSize sizeHint() const;
        void timerEvent(QTimerEvent* event);

    public slots:
        /// abre un diálogo para guardar una imagen con el contenido de la aplicación. útil para los informes o incluso memorias :p
        void guardarImagen();
        void loadModel(QString filename);

    protected:
        /// este método inicializa las variables internas de OpenGL, y otras variables que dependan de OpenGL a través del curso. por ahora no hace mucho.
        void initializeGL();
        /// este método es el que dibuja en pantalla.
        void paintGL();
        /// este método configura el sistema de coordenadas del viewport. se llama resizeGL porque se ejecuta cuando se cambia el tamaño de la ventana (que es cuando interesa actualizar esa información). también se ejecuta en initializeGL.
        void resizeGL(int width, int height);
    
        void mousePressEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void wheelEvent(QWheelEvent *event);
        void keyPressEvent(QKeyEvent *event);
            
    //     aquí van las variables que ustedes consideren prudentes.
    
        QBasicTimer timer;
        QTime tiempo;
    
        bool model_loaded;
    
        Modelo3D* model;
        SurfaceRenderer* fbo_renderer1;
        SurfaceRenderer* fbo_renderer2;
        std::vector<SurfaceRenderer*> renderers;
        int id_graficador;
            
    //     ImageRenderer* image_renderer;
    //     Textura background_image;
    
        OpenMesh::Vec3f center_of_the_universe;
        
        int widget_width;
        int widget_height;
            
        /* visualization */
            
        Camara camara;
        
        float fov;
        OpenMesh::Vec4f object_pos;
                
        int mouse_pos_x;
        int mouse_pos_y;
        
        bool rotating;
        bool translating;
        
        float rotation_x;
        float rotation_y;
        float rotation_z;
        
        float translation_x;
        float translation_y;
        float translation_z;
        
        /* light */
        
        FuenteDeLuz* luz;
        CgContext& cg_context;
};

#endif /*CC52BWIDGET_H_*/
