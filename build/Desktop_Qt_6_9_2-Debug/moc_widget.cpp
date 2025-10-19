/****************************************************************************
** Meta object code from reading C++ file 'widget.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../widget.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'widget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.2. It"
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
struct qt_meta_tag_ZN6WidgetE_t {};
} // unnamed namespace

template <> constexpr inline auto Widget::qt_create_metaobjectdata<qt_meta_tag_ZN6WidgetE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Widget",
        "on_openButton_clicked",
        "",
        "on_closeButton_clicked",
        "updateCOM",
        "serialTransmit",
        "serialReceive",
        "tcpReceive",
        "tcpConnect",
        "tcpStart",
        "golveReceive",
        "golveOrderReceive",
        "UR5Connect",
        "ipAddr",
        "armMove",
        "mx",
        "my",
        "mz",
        "mrx",
        "mry",
        "mrz",
        "on_action1_clicked",
        "on_action2_clicked",
        "on_action3_clicked",
        "on_action4_clicked",
        "on_modeWire_clicked",
        "on_modeWireless_clicked",
        "on_modeCamera_clicked",
        "on_action5_clicked",
        "on_action6_clicked",
        "on_modeGolve_clicked",
        "on_resume_clicked",
        "on_actionSlider1_valueChanged",
        "value",
        "on_actionSlider2_valueChanged",
        "on_actionSlider3_valueChanged",
        "on_actionSlider4_valueChanged",
        "on_actionSlider5_valueChanged",
        "on_actionSlider6_valueChanged",
        "on_actionSlider7_valueChanged",
        "on_actionSlider8_valueChanged",
        "on_actionSlider9_valueChanged",
        "on_actionSlider10_valueChanged",
        "on_actionSlider11_valueChanged",
        "on_actionSlider12_valueChanged",
        "on_actionSlider13_valueChanged",
        "on_actionSlider14_valueChanged",
        "on_actionSlider15_valueChanged",
        "on_actionSlider16_valueChanged",
        "on_actionSlider17_valueChanged",
        "on_actionSlider18_valueChanged",
        "on_modeEveryAction_clicked",
        "on_addActionButton_clicked",
        "on_deleteActionButton_clicked",
        "on_actButton_clicked",
        "on_golveAdjustButton_clicked",
        "on_action7_clicked",
        "Action7",
        "on_action8_clicked",
        "on_modeRock_clicked"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'on_openButton_clicked'
        QtMocHelpers::SlotData<void()>(1, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_closeButton_clicked'
        QtMocHelpers::SlotData<void()>(3, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'updateCOM'
        QtMocHelpers::SlotData<void()>(4, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'serialTransmit'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'serialReceive'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'tcpReceive'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'tcpConnect'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'tcpStart'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'golveReceive'
        QtMocHelpers::SlotData<void()>(10, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'golveOrderReceive'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'UR5Connect'
        QtMocHelpers::SlotData<void(QString)>(12, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 13 },
        }}),
        // Slot 'armMove'
        QtMocHelpers::SlotData<void(double, double, double, double, double, double)>(14, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Double, 15 }, { QMetaType::Double, 16 }, { QMetaType::Double, 17 }, { QMetaType::Double, 18 },
            { QMetaType::Double, 19 }, { QMetaType::Double, 20 },
        }}),
        // Slot 'on_action1_clicked'
        QtMocHelpers::SlotData<void()>(21, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_action2_clicked'
        QtMocHelpers::SlotData<void()>(22, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_action3_clicked'
        QtMocHelpers::SlotData<void()>(23, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_action4_clicked'
        QtMocHelpers::SlotData<void()>(24, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_modeWire_clicked'
        QtMocHelpers::SlotData<void()>(25, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_modeWireless_clicked'
        QtMocHelpers::SlotData<void()>(26, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_modeCamera_clicked'
        QtMocHelpers::SlotData<void()>(27, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_action5_clicked'
        QtMocHelpers::SlotData<void()>(28, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_action6_clicked'
        QtMocHelpers::SlotData<void()>(29, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_modeGolve_clicked'
        QtMocHelpers::SlotData<void()>(30, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_resume_clicked'
        QtMocHelpers::SlotData<void()>(31, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_actionSlider1_valueChanged'
        QtMocHelpers::SlotData<void(int)>(32, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 33 },
        }}),
        // Slot 'on_actionSlider2_valueChanged'
        QtMocHelpers::SlotData<void(int)>(34, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 33 },
        }}),
        // Slot 'on_actionSlider3_valueChanged'
        QtMocHelpers::SlotData<void(int)>(35, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 33 },
        }}),
        // Slot 'on_actionSlider4_valueChanged'
        QtMocHelpers::SlotData<void(int)>(36, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 33 },
        }}),
        // Slot 'on_actionSlider5_valueChanged'
        QtMocHelpers::SlotData<void(int)>(37, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 33 },
        }}),
        // Slot 'on_actionSlider6_valueChanged'
        QtMocHelpers::SlotData<void(int)>(38, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 33 },
        }}),
        // Slot 'on_actionSlider7_valueChanged'
        QtMocHelpers::SlotData<void(int)>(39, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 33 },
        }}),
        // Slot 'on_actionSlider8_valueChanged'
        QtMocHelpers::SlotData<void(int)>(40, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 33 },
        }}),
        // Slot 'on_actionSlider9_valueChanged'
        QtMocHelpers::SlotData<void(int)>(41, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 33 },
        }}),
        // Slot 'on_actionSlider10_valueChanged'
        QtMocHelpers::SlotData<void(int)>(42, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 33 },
        }}),
        // Slot 'on_actionSlider11_valueChanged'
        QtMocHelpers::SlotData<void(int)>(43, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 33 },
        }}),
        // Slot 'on_actionSlider12_valueChanged'
        QtMocHelpers::SlotData<void(int)>(44, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 33 },
        }}),
        // Slot 'on_actionSlider13_valueChanged'
        QtMocHelpers::SlotData<void(int)>(45, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 33 },
        }}),
        // Slot 'on_actionSlider14_valueChanged'
        QtMocHelpers::SlotData<void(int)>(46, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 33 },
        }}),
        // Slot 'on_actionSlider15_valueChanged'
        QtMocHelpers::SlotData<void(int)>(47, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 33 },
        }}),
        // Slot 'on_actionSlider16_valueChanged'
        QtMocHelpers::SlotData<void(int)>(48, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 33 },
        }}),
        // Slot 'on_actionSlider17_valueChanged'
        QtMocHelpers::SlotData<void(int)>(49, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 33 },
        }}),
        // Slot 'on_actionSlider18_valueChanged'
        QtMocHelpers::SlotData<void(int)>(50, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 33 },
        }}),
        // Slot 'on_modeEveryAction_clicked'
        QtMocHelpers::SlotData<void()>(51, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_addActionButton_clicked'
        QtMocHelpers::SlotData<void()>(52, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_deleteActionButton_clicked'
        QtMocHelpers::SlotData<void()>(53, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_actButton_clicked'
        QtMocHelpers::SlotData<void()>(54, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_golveAdjustButton_clicked'
        QtMocHelpers::SlotData<void()>(55, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_action7_clicked'
        QtMocHelpers::SlotData<void()>(56, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'Action7'
        QtMocHelpers::SlotData<void()>(57, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_action8_clicked'
        QtMocHelpers::SlotData<void()>(58, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_modeRock_clicked'
        QtMocHelpers::SlotData<void()>(59, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Widget, qt_meta_tag_ZN6WidgetE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Widget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6WidgetE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6WidgetE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN6WidgetE_t>.metaTypes,
    nullptr
} };

void Widget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Widget *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->on_openButton_clicked(); break;
        case 1: _t->on_closeButton_clicked(); break;
        case 2: _t->updateCOM(); break;
        case 3: _t->serialTransmit(); break;
        case 4: _t->serialReceive(); break;
        case 5: _t->tcpReceive(); break;
        case 6: _t->tcpConnect(); break;
        case 7: _t->tcpStart(); break;
        case 8: _t->golveReceive(); break;
        case 9: _t->golveOrderReceive(); break;
        case 10: _t->UR5Connect((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 11: _t->armMove((*reinterpret_cast< std::add_pointer_t<double>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[5])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[6]))); break;
        case 12: _t->on_action1_clicked(); break;
        case 13: _t->on_action2_clicked(); break;
        case 14: _t->on_action3_clicked(); break;
        case 15: _t->on_action4_clicked(); break;
        case 16: _t->on_modeWire_clicked(); break;
        case 17: _t->on_modeWireless_clicked(); break;
        case 18: _t->on_modeCamera_clicked(); break;
        case 19: _t->on_action5_clicked(); break;
        case 20: _t->on_action6_clicked(); break;
        case 21: _t->on_modeGolve_clicked(); break;
        case 22: _t->on_resume_clicked(); break;
        case 23: _t->on_actionSlider1_valueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 24: _t->on_actionSlider2_valueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 25: _t->on_actionSlider3_valueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 26: _t->on_actionSlider4_valueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 27: _t->on_actionSlider5_valueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 28: _t->on_actionSlider6_valueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 29: _t->on_actionSlider7_valueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 30: _t->on_actionSlider8_valueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 31: _t->on_actionSlider9_valueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 32: _t->on_actionSlider10_valueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 33: _t->on_actionSlider11_valueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 34: _t->on_actionSlider12_valueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 35: _t->on_actionSlider13_valueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 36: _t->on_actionSlider14_valueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 37: _t->on_actionSlider15_valueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 38: _t->on_actionSlider16_valueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 39: _t->on_actionSlider17_valueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 40: _t->on_actionSlider18_valueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 41: _t->on_modeEveryAction_clicked(); break;
        case 42: _t->on_addActionButton_clicked(); break;
        case 43: _t->on_deleteActionButton_clicked(); break;
        case 44: _t->on_actButton_clicked(); break;
        case 45: _t->on_golveAdjustButton_clicked(); break;
        case 46: _t->on_action7_clicked(); break;
        case 47: _t->Action7(); break;
        case 48: _t->on_action8_clicked(); break;
        case 49: _t->on_modeRock_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject *Widget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Widget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6WidgetE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Widget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 50)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 50;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 50)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 50;
    }
    return _id;
}
QT_WARNING_POP
