/****************************************************************************
** Meta object code from reading C++ file 'MapView.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../include/MapView.h"
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MapView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN7MapViewE_t {};
} // unnamed namespace

template <> constexpr inline auto MapView::qt_create_metaobjectdata<qt_meta_tag_ZN7MapViewE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "MapView",
        "isReadyToShow",
        "",
        "getChosenProvinceInfo",
        "provinceID",
        "highlightProvinces",
        "highlight",
        "addProvinceToHighlight",
        "QList<QPoint>",
        "province_pixels",
        "removeProvinceFromHighlight",
        "progressMade",
        "progress",
        "prepare",
        "GetCountryColor",
        "QRgb",
        "tag",
        "getProvincesPixels",
        "QList<QList<QPoint>>",
        "QList<int>",
        "provinces",
        "GetBordersViewMap",
        "GetCountriesViewMap",
        "setProvinceChoosingMode",
        "choose",
        "showHighlighting",
        "show",
        "clearChosenProvinces"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'isReadyToShow'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'getChosenProvinceInfo'
        QtMocHelpers::SignalData<void(int)>(3, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 4 },
        }}),
        // Signal 'highlightProvinces'
        QtMocHelpers::SignalData<void(bool)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 6 },
        }}),
        // Signal 'addProvinceToHighlight'
        QtMocHelpers::SignalData<void(const QList<QPoint> &, int)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 8, 9 }, { QMetaType::Int, 4 },
        }}),
        // Signal 'removeProvinceFromHighlight'
        QtMocHelpers::SignalData<void(const QList<QPoint> &, int)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 8, 9 }, { QMetaType::Int, 4 },
        }}),
        // Signal 'progressMade'
        QtMocHelpers::SignalData<void(int)>(11, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 12 },
        }}),
        // Slot 'prepare'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'GetCountryColor'
        QtMocHelpers::SlotData<QRgb(QString)>(14, 2, QMC::AccessPublic, 0x80000000 | 15, {{
            { QMetaType::QString, 16 },
        }}),
        // Slot 'getProvincesPixels'
        QtMocHelpers::SlotData<QList<QList<QPoint>>(QList<int>)>(17, 2, QMC::AccessPublic, 0x80000000 | 18, {{
            { 0x80000000 | 19, 20 },
        }}),
        // Slot 'GetBordersViewMap'
        QtMocHelpers::SlotData<QImage()>(21, 2, QMC::AccessPublic, QMetaType::QImage),
        // Slot 'GetCountriesViewMap'
        QtMocHelpers::SlotData<QImage()>(22, 2, QMC::AccessPublic, QMetaType::QImage),
        // Slot 'setProvinceChoosingMode'
        QtMocHelpers::SlotData<void(bool)>(23, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 24 },
        }}),
        // Slot 'showHighlighting'
        QtMocHelpers::SlotData<void(bool)>(25, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 26 },
        }}),
        // Slot 'clearChosenProvinces'
        QtMocHelpers::SlotData<void()>(27, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<MapView, qt_meta_tag_ZN7MapViewE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject MapView::staticMetaObject = { {
    QMetaObject::SuperData::link<QGraphicsView::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7MapViewE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7MapViewE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN7MapViewE_t>.metaTypes,
    nullptr
} };

void MapView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<MapView *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->isReadyToShow(); break;
        case 1: _t->getChosenProvinceInfo((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->highlightProvinces((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 3: _t->addProvinceToHighlight((*reinterpret_cast< std::add_pointer_t<QList<QPoint>>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 4: _t->removeProvinceFromHighlight((*reinterpret_cast< std::add_pointer_t<QList<QPoint>>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 5: _t->progressMade((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 6: _t->prepare(); break;
        case 7: { QRgb _r = _t->GetCountryColor((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QRgb*>(_a[0]) = std::move(_r); }  break;
        case 8: { QList<QList<QPoint>> _r = _t->getProvincesPixels((*reinterpret_cast< std::add_pointer_t<QList<int>>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QList<QList<QPoint>>*>(_a[0]) = std::move(_r); }  break;
        case 9: { QImage _r = _t->GetBordersViewMap();
            if (_a[0]) *reinterpret_cast< QImage*>(_a[0]) = std::move(_r); }  break;
        case 10: { QImage _r = _t->GetCountriesViewMap();
            if (_a[0]) *reinterpret_cast< QImage*>(_a[0]) = std::move(_r); }  break;
        case 11: _t->setProvinceChoosingMode((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 12: _t->showHighlighting((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 13: _t->clearChosenProvinces(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<QPoint> >(); break;
            }
            break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<QPoint> >(); break;
            }
            break;
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<int> >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (MapView::*)()>(_a, &MapView::isReadyToShow, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (MapView::*)(int )>(_a, &MapView::getChosenProvinceInfo, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (MapView::*)(bool )>(_a, &MapView::highlightProvinces, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (MapView::*)(const QList<QPoint> & , int )>(_a, &MapView::addProvinceToHighlight, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (MapView::*)(const QList<QPoint> & , int )>(_a, &MapView::removeProvinceFromHighlight, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (MapView::*)(int )>(_a, &MapView::progressMade, 5))
            return;
    }
}

const QMetaObject *MapView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MapView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7MapViewE_t>.strings))
        return static_cast<void*>(this);
    return QGraphicsView::qt_metacast(_clname);
}

int MapView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void MapView::isReadyToShow()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void MapView::getChosenProvinceInfo(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void MapView::highlightProvinces(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void MapView::addProvinceToHighlight(const QList<QPoint> & _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1, _t2);
}

// SIGNAL 4
void MapView::removeProvinceFromHighlight(const QList<QPoint> & _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1, _t2);
}

// SIGNAL 5
void MapView::progressMade(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1);
}
QT_WARNING_POP
