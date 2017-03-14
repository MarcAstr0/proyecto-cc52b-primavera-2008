/****************************************************************************
** Meta object code from reading C++ file 'QGLViewerWidget.hh'
**
** Created: Sun Sep 7 23:53:30 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../QGLViewerWidget.hh"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QGLViewerWidget.hh' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QGLViewerWidget[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      23,   17,   16,   16, 0x08,
      46,   16,   16,   16, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QGLViewerWidget[] = {
    "QGLViewerWidget\0\0_mode\0slotDrawMode(QAction*)\0"
    "slotSnapshot()\0"
};

const QMetaObject QGLViewerWidget::staticMetaObject = {
    { &QGLWidget::staticMetaObject, qt_meta_stringdata_QGLViewerWidget,
      qt_meta_data_QGLViewerWidget, 0 }
};

const QMetaObject *QGLViewerWidget::metaObject() const
{
    return &staticMetaObject;
}

void *QGLViewerWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QGLViewerWidget))
	return static_cast<void*>(const_cast< QGLViewerWidget*>(this));
    return QGLWidget::qt_metacast(_clname);
}

int QGLViewerWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: slotDrawMode((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 1: slotSnapshot(); break;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
