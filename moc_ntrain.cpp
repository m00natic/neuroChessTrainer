/****************************************************************************
** Meta object code from reading C++ file 'ntrain.h'
**
** Created: Sun Nov 28 19:56:09 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ntrain.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ntrain.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_nTrain[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       8,    7,    7,    7, 0x08,
      28,    7,    7,    7, 0x08,
      48,    7,    7,    7, 0x08,
      71,    7,    7,    7, 0x08,
      91,    7,    7,    7, 0x08,
     117,    7,    7,    7, 0x08,
     142,    7,    7,    7, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_nTrain[] = {
    "nTrain\0\0on_pbTest_clicked()\0"
    "on_pbStop_clicked()\0on_pbConvert_clicked()\0"
    "on_pbSave_clicked()\0on_pbBrowseFile_clicked()\0"
    "on_pbBrowseDir_clicked()\0on_pbTrain_clicked()\0"
};

const QMetaObject nTrain::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_nTrain,
      qt_meta_data_nTrain, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &nTrain::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *nTrain::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *nTrain::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_nTrain))
        return static_cast<void*>(const_cast< nTrain*>(this));
    return QDialog::qt_metacast(_clname);
}

int nTrain::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_pbTest_clicked(); break;
        case 1: on_pbStop_clicked(); break;
        case 2: on_pbConvert_clicked(); break;
        case 3: on_pbSave_clicked(); break;
        case 4: on_pbBrowseFile_clicked(); break;
        case 5: on_pbBrowseDir_clicked(); break;
        case 6: on_pbTrain_clicked(); break;
        default: ;
        }
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
