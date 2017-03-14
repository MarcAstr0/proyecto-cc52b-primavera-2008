/****************************************************************************
** Meta object code from reading C++ file 'MeshViewerWidget.hh'
**
** Created: Sun Sep 7 23:53:30 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../MeshViewerWidget.hh"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MeshViewerWidget.hh' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MeshViewerWidget[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x0a,
      41,   17,   17,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MeshViewerWidget[] = {
    "MeshViewerWidget\0\0query_open_mesh_file()\0"
    "query_open_texture_file()\0"
};

const QMetaObject MeshViewerWidget::staticMetaObject = {
    { &MeshViewerWidgetT<MyMesh>::staticMetaObject, qt_meta_stringdata_MeshViewerWidget,
      qt_meta_data_MeshViewerWidget, 0 }
};

const QMetaObject *MeshViewerWidget::metaObject() const
{
    return &staticMetaObject;
}

void *MeshViewerWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MeshViewerWidget))
	return static_cast<void*>(const_cast< MeshViewerWidget*>(this));
    return MeshViewerWidgetT<MyMesh>::qt_metacast(_clname);
}

int MeshViewerWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = MeshViewerWidgetT<MyMesh>::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: query_open_mesh_file(); break;
        case 1: query_open_texture_file(); break;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
