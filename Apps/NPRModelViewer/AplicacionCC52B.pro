CONFIG += release
TEMPLATE = app
TARGET = AplicacionCC52B
QT += core \
    gui \
    opengl

QMAKE_CXXFLAGS_RELEASE += -O9 -funroll-loops -msse2

DEFINES += GL_GLEXT_PROTOTYPES
INCLUDEPATH += 	../../Zahir/External \
				../../
QMAKE_LIBDIR += 	../../Zahir/External/OpenMesh/Core/lib
LIBS += -lOpenMesh_Core -lCg -lCgGL -lGLEW 
HEADERS +=  	*.h \
		../../Zahir/External/*.h \
		../../Zahir/External/fboClass/framebufferObject.h \
		../../Zahir/External/fboClass/renderbuffer.h \
		../../Zahir/Generics/Tree/*.hh \
		../../Zahir/Generics/Matrix/*.h \
		../../Zahir/Generics/Interpolators/*.h \
		../../Zahir/Geometry/*.h \
		../../Zahir/Models/*.h \
		../../Zahir/Models/Operations/*.h \
		../../Zahir/Models/Parameterization/LappedTextures/*.h \
		../../Zahir/Models/Cargadores/*.h \
		../../Zahir/Models/ModelFactory/*.h \
		../../Zahir/Visualization/*.h \
		../../Zahir/Visualization/GPU/*.h \
		../../Zahir/Visualization/Renderers/*.h \
		../../Zahir/Visualization/Renderers/Surface/*.h \
		../../Zahir/Visualization/Renderers/Surface/Gooch/*.h \
		../../Zahir/Visualization/Renderers/Surface/Phong/*.h \
		../../Zahir/Visualization/Renderers/Surface/Toon/*.h \
        	../../Zahir/Visualization/Renderers/Surface/DepthMap/*.h \
                ../../Zahir/Visualization/Renderers/Surface/NormalMap/*.h \
		../../Zahir/Visualization/Renderers/Image/*.h \
		../../Zahir/Visualization/Matrices/*.h \
		../../Zahir/Visualization/Texturas/*.h 
SOURCES +=  	*.cpp \
		../../Zahir/External/fboClass/framebufferObject.cpp \
		../../Zahir/External/fboClass/renderbuffer.cpp \
		../../Zahir/Models/*.cpp \
		../../Zahir/Models/Operations/*.cpp \
		../../Zahir/Models/Parameterization/LappedTextures/*.cpp \
		../../Zahir/Models/Cargadores/*.cpp \
		../../Zahir/Visualization/*.cpp \
		../../Zahir/Visualization/Renderers/Surface/*.cpp \
		../../Zahir/Visualization/Renderers/Surface/Gooch/*.cpp \
		../../Zahir/Visualization/Renderers/Surface/Toon/*.cpp \
		../../Zahir/Visualization/Renderers/Surface/DepthMap/*.cpp \
		../../Zahir/Visualization/Matrices/*.cpp \
		../../Zahir/Visualization/Texturas/*.cpp 
FORMS += aplicacion_cc52b.ui
RESOURCES += 
