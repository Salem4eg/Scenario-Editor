/****************************************************************************
** Meta object code from reading C++ file 'ProvinceInfoToolTip.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../include/ProvinceInfoToolTip.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ProvinceInfoToolTip.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN19ProvinceInfoToolTipE_t {};
} // unnamed namespace

template <> constexpr inline auto ProvinceInfoToolTip::qt_create_metaobjectdata<qt_meta_tag_ZN19ProvinceInfoToolTipE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ProvinceInfoToolTip",
        "hideToolTip",
        "",
        "showProvinceDetails",
        "setInfo",
        "ProvinceInfo",
        "info"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'hideToolTip'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'showProvinceDetails'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'setInfo'
        QtMocHelpers::SlotData<void(const ProvinceInfo &)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 5, 6 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ProvinceInfoToolTip, qt_meta_tag_ZN19ProvinceInfoToolTipE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ProvinceInfoToolTip::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN19ProvinceInfoToolTipE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN19ProvinceInfoToolTipE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN19ProvinceInfoToolTipE_t>.metaTypes,
    nullptr
} };

void ProvinceInfoToolTip::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ProvinceInfoToolTip *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->hideToolTip(); break;
        case 1: _t->showProvinceDetails(); break;
        case 2: _t->setInfo((*reinterpret_cast< std::add_pointer_t<ProvinceInfo>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ProvinceInfoToolTip::*)()>(_a, &ProvinceInfoToolTip::hideToolTip, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (ProvinceInfoToolTip::*)()>(_a, &ProvinceInfoToolTip::showProvinceDetails, 1))
            return;
    }
}

const QMetaObject *ProvinceInfoToolTip::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ProvinceInfoToolTip::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN19ProvinceInfoToolTipE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ProvinceInfoToolTip::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void ProvinceInfoToolTip::hideToolTip()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ProvinceInfoToolTip::showProvinceDetails()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
