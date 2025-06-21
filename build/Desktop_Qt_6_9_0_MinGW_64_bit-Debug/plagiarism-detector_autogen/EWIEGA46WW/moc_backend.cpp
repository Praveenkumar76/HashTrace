/****************************************************************************
** Meta object code from reading C++ file 'backend.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../backend.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'backend.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN7BackendE_t {};
} // unnamed namespace

template <> constexpr inline auto Backend::qt_create_metaobjectdata<qt_meta_tag_ZN7BackendE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Backend",
        "comparisonFinished",
        "",
        "similarityScore",
        "QVariantList",
        "matches",
        "processingChanged",
        "processing",
        "errorOccurred",
        "message",
        "comparisonDataReady",
        "matchedSegments",
        "processFiles",
        "filePaths",
        "cancelProcessing",
        "getProcessedContent",
        "filePath",
        "isProcessing"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'comparisonFinished'
        QtMocHelpers::SignalData<void(double, const QVariantList &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 3 }, { 0x80000000 | 4, 5 },
        }}),
        // Signal 'processingChanged'
        QtMocHelpers::SignalData<void(bool)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 7 },
        }}),
        // Signal 'errorOccurred'
        QtMocHelpers::SignalData<void(const QString &)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 9 },
        }}),
        // Signal 'comparisonDataReady'
        QtMocHelpers::SignalData<void(const QVariantList &)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 4, 11 },
        }}),
        // Slot 'processFiles'
        QtMocHelpers::SlotData<void(const QStringList &)>(12, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QStringList, 13 },
        }}),
        // Slot 'cancelProcessing'
        QtMocHelpers::SlotData<void()>(14, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'getProcessedContent'
        QtMocHelpers::MethodData<QString(const QString &)>(15, 2, QMC::AccessPublic, QMetaType::QString, {{
            { QMetaType::QString, 16 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'isProcessing'
        QtMocHelpers::PropertyData<bool>(17, QMetaType::Bool, QMC::DefaultPropertyFlags, 1),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Backend, qt_meta_tag_ZN7BackendE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Backend::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7BackendE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7BackendE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN7BackendE_t>.metaTypes,
    nullptr
} };

void Backend::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Backend *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->comparisonFinished((*reinterpret_cast< std::add_pointer_t<double>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QVariantList>>(_a[2]))); break;
        case 1: _t->processingChanged((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 2: _t->errorOccurred((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->comparisonDataReady((*reinterpret_cast< std::add_pointer_t<QVariantList>>(_a[1]))); break;
        case 4: _t->processFiles((*reinterpret_cast< std::add_pointer_t<QStringList>>(_a[1]))); break;
        case 5: _t->cancelProcessing(); break;
        case 6: { QString _r = _t->getProcessedContent((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (Backend::*)(double , const QVariantList & )>(_a, &Backend::comparisonFinished, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (Backend::*)(bool )>(_a, &Backend::processingChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (Backend::*)(const QString & )>(_a, &Backend::errorOccurred, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (Backend::*)(const QVariantList & )>(_a, &Backend::comparisonDataReady, 3))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<bool*>(_v) = _t->isProcessing(); break;
        default: break;
        }
    }
}

const QMetaObject *Backend::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Backend::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7BackendE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Backend::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 7;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void Backend::comparisonFinished(double _t1, const QVariantList & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2);
}

// SIGNAL 1
void Backend::processingChanged(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void Backend::errorOccurred(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void Backend::comparisonDataReady(const QVariantList & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}
QT_WARNING_POP
