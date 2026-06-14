/****************************************************************************
** Meta object code from reading C++ file 'MapImageProcessor.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../include/MapImageProcessor.h"
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MapImageProcessor.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN17MapImageProcessorE_t {};
} // unnamed namespace

template <> constexpr inline auto MapImageProcessor::qt_create_metaobjectdata<qt_meta_tag_ZN17MapImageProcessorE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "MapImageProcessor",
        "mapPixelsToProvinces",
        "",
        "GetBordersViewMap",
        "GetCountriesViewMap",
        "colorOfCountry",
        "QRgb",
        "countryTag",
        "pixelsOfProvince",
        "QList<QPoint>",
        "province",
        "setCountriesProvinces",
        "QHash<QString,QList<int>>",
        "countries_provinces",
        "setCountriesColor",
        "QHash<QString,QRgb>",
        "countries_color",
        "setColorToProvince",
        "QHash<QRgb,int>",
        "color_to_province",
        "getMapWidth",
        "getMapHeight",
        "provinceAt",
        "x",
        "y",
        "mapPartOfPixelsToProvinces",
        "start_index",
        "indexes_to_process",
        "paintProvince",
        "QImage&",
        "countries_view_map",
        "color",
        "makeOutlineForPartOfMap",
        "outline_map",
        "hasAnotherProvinceNear",
        "colorAtPixel"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'mapPixelsToProvinces'
        QtMocHelpers::SlotData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'GetBordersViewMap'
        QtMocHelpers::SlotData<QImage()>(3, 2, QMC::AccessPublic, QMetaType::QImage),
        // Slot 'GetCountriesViewMap'
        QtMocHelpers::SlotData<QImage()>(4, 2, QMC::AccessPublic, QMetaType::QImage),
        // Slot 'colorOfCountry'
        QtMocHelpers::SlotData<QRgb(QString)>(5, 2, QMC::AccessPublic, 0x80000000 | 6, {{
            { QMetaType::QString, 7 },
        }}),
        // Slot 'pixelsOfProvince'
        QtMocHelpers::SlotData<QList<QPoint>(int)>(8, 2, QMC::AccessPublic, 0x80000000 | 9, {{
            { QMetaType::Int, 10 },
        }}),
        // Slot 'setCountriesProvinces'
        QtMocHelpers::SlotData<void(const QHash<QString,QList<int>> &)>(11, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 12, 13 },
        }}),
        // Slot 'setCountriesColor'
        QtMocHelpers::SlotData<void(const QHash<QString,QRgb> &)>(14, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 15, 16 },
        }}),
        // Slot 'setColorToProvince'
        QtMocHelpers::SlotData<void(const QHash<QRgb,int> &)>(17, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 18, 19 },
        }}),
        // Slot 'getMapWidth'
        QtMocHelpers::SlotData<int()>(20, 2, QMC::AccessPublic, QMetaType::Int),
        // Slot 'getMapHeight'
        QtMocHelpers::SlotData<int()>(21, 2, QMC::AccessPublic, QMetaType::Int),
        // Slot 'provinceAt'
        QtMocHelpers::SlotData<int(int, int)>(22, 2, QMC::AccessPublic, QMetaType::Int, {{
            { QMetaType::Int, 23 }, { QMetaType::Int, 24 },
        }}),
        // Slot 'mapPartOfPixelsToProvinces'
        QtMocHelpers::SlotData<void(int, int)>(25, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 26 }, { QMetaType::Int, 27 },
        }}),
        // Slot 'paintProvince'
        QtMocHelpers::SlotData<void(QImage &, int, QRgb)>(28, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 29, 30 }, { QMetaType::Int, 10 }, { 0x80000000 | 6, 31 },
        }}),
        // Slot 'makeOutlineForPartOfMap'
        QtMocHelpers::SlotData<void(QImage &, int, int)>(32, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 29, 33 }, { QMetaType::Int, 26 }, { QMetaType::Int, 27 },
        }}),
        // Slot 'hasAnotherProvinceNear'
        QtMocHelpers::SlotData<bool(int, int)>(34, 2, QMC::AccessPrivate, QMetaType::Bool, {{
            { QMetaType::Int, 23 }, { QMetaType::Int, 24 },
        }}),
        // Slot 'colorAtPixel'
        QtMocHelpers::SlotData<QColor(int, int)>(35, 2, QMC::AccessPrivate, QMetaType::QColor, {{
            { QMetaType::Int, 23 }, { QMetaType::Int, 24 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<MapImageProcessor, qt_meta_tag_ZN17MapImageProcessorE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject MapImageProcessor::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17MapImageProcessorE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17MapImageProcessorE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN17MapImageProcessorE_t>.metaTypes,
    nullptr
} };

void MapImageProcessor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<MapImageProcessor *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->mapPixelsToProvinces(); break;
        case 1: { QImage _r = _t->GetBordersViewMap();
            if (_a[0]) *reinterpret_cast< QImage*>(_a[0]) = std::move(_r); }  break;
        case 2: { QImage _r = _t->GetCountriesViewMap();
            if (_a[0]) *reinterpret_cast< QImage*>(_a[0]) = std::move(_r); }  break;
        case 3: { QRgb _r = _t->colorOfCountry((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QRgb*>(_a[0]) = std::move(_r); }  break;
        case 4: { QList<QPoint> _r = _t->pixelsOfProvince((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QList<QPoint>*>(_a[0]) = std::move(_r); }  break;
        case 5: _t->setCountriesProvinces((*reinterpret_cast< std::add_pointer_t<QHash<QString,QList<int>>>>(_a[1]))); break;
        case 6: _t->setCountriesColor((*reinterpret_cast< std::add_pointer_t<QHash<QString,QRgb>>>(_a[1]))); break;
        case 7: _t->setColorToProvince((*reinterpret_cast< std::add_pointer_t<QHash<QRgb,int>>>(_a[1]))); break;
        case 8: { int _r = _t->getMapWidth();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 9: { int _r = _t->getMapHeight();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 10: { int _r = _t->provinceAt((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 11: _t->mapPartOfPixelsToProvinces((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 12: _t->paintProvince((*reinterpret_cast< std::add_pointer_t<QImage&>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QRgb>>(_a[3]))); break;
        case 13: _t->makeOutlineForPartOfMap((*reinterpret_cast< std::add_pointer_t<QImage&>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3]))); break;
        case 14: { bool _r = _t->hasAnotherProvinceNear((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 15: { QColor _r = _t->colorAtPixel((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast< QColor*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
}

const QMetaObject *MapImageProcessor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MapImageProcessor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17MapImageProcessorE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int MapImageProcessor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 16;
    }
    return _id;
}
QT_WARNING_POP
