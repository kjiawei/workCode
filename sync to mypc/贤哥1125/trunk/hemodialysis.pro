Platform = $$system(qmake -v | grep arm)

TEMPLATE = app
DESTDIR +=
#CONFIG += qt warn_on debug -lpthread -Wno-psabi
CONFIG += qt warn_on

CONFIG(debug){
    TARGET = hemodialysis
    OBJECTS_DIR += build/debug_obj
}else{
    TARGET = hemodialysis
    OBJECTS_DIR += build/release_obj
    DEFINES += RELEASE
}

RCC_DIR += build/tmp
MOC_DIR += build/tmp

#RCC_DIR += $$PWD/tmp
#MOC_DIR += $$PWD/tmp

DEFINES += MULIT_LINE

INCLUDEPATH += . \
	       include \
               depend/CanFestival-3-ab83333fee78/include \
               depend/CanFestival-3-ab83333fee78/include/can_virtual \
               depend/CanFestival-3-ab83333fee78/include/timers_unix \
               src/windows \
               src/window_status_map \
               src/packet \
               src/common \
               src/common_widget \
               src/scenes/scene_main \
               src/scenes/scene_disinfect \
               src/scenes/scene_ext \
               src/scenes/scene_warn \
               src/scenes/scene_summary \
               src/scenes/scene_ims \
               src/setup/setup_cure \
               src/setup/setup_workmode \
               src/setup/setup_advanced \
               src/setup/setup_other \
               src/maintain \
               src/canopen \
               src/data_manage \
               src/virtual_keyboard \
               src/audio \
               src/lamp \
               src/buttons \
               src/xml_parse \
               src/wlan \
               src/RFCard \
               src/business \
               src/engineering_mode \
               src/scenes/scene_note
# Input
HEADERS += include/*.h \
           src/windows/*.h \
           src/window_status_map/*.h \
           src/data_manage/*.h \
           src/packet/*.h \
           src/canopen/*.h \
           src/common/*.h \
           src/common_widget/*.h \
           src/setup/setup_workmode/*.h \
           src/setup/setup_cure/*.h \
           src/setup/setup_advanced/*.h \
           src/setup/setup_other/*.h \
           src/scenes/scene_main/*.h \
           src/scenes/scene_disinfect/*.h \
           src/scenes/scene_ext/*.h \
           src/scenes/scene_warn/*.h \
           src/scenes/scene_summary/*.h \
           src/scenes/scene_ims/*.h \
           src/virtual_keyboard/*.h \
           src/wlan/widget_connect_wlan_icon.h \
           src/wlan/widget_wlan.h \
           src/xml_parse/*.h \
           src/audio/*.h \
           src/maintain/*.h \
           src/RFCard/*.h \
           src/business/state_test.h \
           src/business/state_prime.h \
           src/business/state_disinfect.h \
           src/business/state_dialysis.h \
           src/business/state.h \
           src/engineering_mode/engineering_mode.h \
           src/lamp/lamp_control.h \
           src/lamp/lamp_task.h \
           src/scenes/scene_note/scene_note.h \
           src/buttons/buttons.h \
           src/buttons/buttosirq.h \
    src/debug_mode/widget_debug_mode.h \
    src/debug_mode/scene_blood_debug/scene_blood_debug.h \
    src/debug_mode/scene_drain_option/scene_drain_option.h \
    src/debug_mode/scene_project_param/scene_project_param.h \
    src/debug_mode/scene_setup_param/scene_setup_param.h \
    src/debug_mode/scene_ts_debug/scene_ts_debug.h \
    src/debug_mode/scene_user_param/scene_user_param.h \
    src/debug_mode/scene_use_time/scene_use_time.h \
    src/debug_mode/scene_water_debug/scene_water_debug.h \
    src/debug_mode/scene_water_test/scene_water_test.h \
    src/debug_mode/scene_blood_debug/scene2_bloodpump.h \
    src/debug_mode/scene_blood_debug/scene2_heparinpump.h \
    src/debug_mode/scene_blood_debug/scene2_pressuredebug.h \
    src/debug_mode/scene_blood_debug/scene2_bubble.h \
    src/debug_mode/scene_blood_debug/scene2_liquidlevel.h \
    src/debug_mode/scene_blood_debug/scene2_arteriovenous.h \
    src/debug_mode/scene_blood_debug/scene2_chokeclip.h \
    src/debug_mode/scene_blood_debug/scene2_soundlight.h \
    src/debug_mode/scene_blood_debug/scene2_batterysupply.h \
    src/debug_mode/scene_controlwater_graph/scene_controlwater_graph.h \
    src/debug_mode/scene_water_debug/scene2_BloodLeakCheck.h \
    src/debug_mode/scene_blood_debug/scene2_subpump.h \
    src/debug_mode/scene_water_debug/scene2_SetFormula.h \
    src/debug_mode/scene_water_debug/scene2_BeforeAfterPump.h \
    src/debug_mode/scene_water_debug/scene2_DegassingPump.h \
    src/debug_mode/scene_water_debug/scene2_DialystateFlow.h \
    src/debug_mode/scene_water_debug/scene2_ABPump.h \
    src/debug_mode/scene_water_debug/scene2_TempAdjust.h \
    src/debug_mode/scene_water_debug/scene2_CondAdjust.h \
    src/debug_mode/scene_water_debug/scene2_UFPump.h \
    src/httpserver/http_parser.h \
    src/httpserver/httpconnection.h \
    src/httpserver/httprequest.h \
    src/httpserver/httpresponse.h \
    src/httpserver/httpserver.h \
    src/wlan/os.h

           
SOURCES += main.cpp \
           src/data_manage/*.cpp \
           src/canopen/deal.c \
           src/canopen/middle.cpp \
           src/canopen/canopen_shell.c \
           src/common/*.cpp \
           src/windows/*.cpp \
           src/window_status_map/*.cpp \
           src/scenes/scene_main/*.cpp \
           src/common_widget/*.cpp \
           src/setup/setup_workmode/*.cpp \
           src/setup/setup_cure/*.cpp \
           src/setup/setup_advanced/*.cpp \
           src/setup/setup_other/*.cpp \
           src/scenes/scene_disinfect/*.cpp \
           src/scenes/scene_ext/*.cpp \
           src/scenes/scene_warn/*.cpp \
           src/scenes/scene_summary/*.cpp \
           src/scenes/scene_ims/*.cpp \
           src/virtual_keyboard/*.cpp \
           src/xml_parse/xml_parse.cpp \
           src/wlan/widget_connect_wlan_icon.cpp \
           src/wlan/widget_wlan.cpp \
           src/audio/*.cpp \
           src/audio/*.c \
           src/maintain/*.cpp \
           src/business/state.cpp \
           src/business/state_test.cpp \
           src/business/state_prime.cpp \
           src/business/state_disinfect.cpp \
           src/business/state_dialysis.cpp \
           src/engineering_mode/*.cpp \
           src/RFCard/*.cpp \
           src/RFCard/*.c \
           src/lamp/lamp_control.c \
           src/lamp/lamp_task.cpp \
           src/scenes/scene_note/scene_note.cpp \
    src/buttons/buttons.c \
    src/buttons/buttosirq.cpp \
    src/debug_mode/widget_debug_mode.cpp \
    src/debug_mode/scene_blood_debug/scene_blood_debug.cpp \
    src/debug_mode/scene_water_debug/scene_water_debug.cpp \
    src/debug_mode/scene_drain_option/scene_drain_option.cpp \
    src/debug_mode/scene_project_param/scene_project_param.cpp \
    src/debug_mode/scene_setup_param/scene_setup_param.cpp \
    src/debug_mode/scene_ts_debug/scene_ts_debug.cpp \
    src/debug_mode/scene_user_param/scene_user_param.cpp \
    src/debug_mode/scene_use_time/scene_use_time.cpp \
    src/debug_mode/scene_water_test/scene_water_test.cpp \
    src/debug_mode/scene_blood_debug/scene2_bloodpump.cpp \
    src/debug_mode/scene_blood_debug/scene2_heparinpump.cpp \
    src/debug_mode/scene_blood_debug/scene2_pressuredebug.cpp \
    src/debug_mode/scene_blood_debug/scene2_bubble.cpp \
    src/debug_mode/scene_blood_debug/scene2_liquidlevel.cpp \
    src/debug_mode/scene_blood_debug/scene2_arteriovenous.cpp \
    src/debug_mode/scene_blood_debug/scene2_chokeclip.cpp \
    src/debug_mode/scene_blood_debug/scene2_soundlight.cpp \
    src/debug_mode/scene_blood_debug/scene2_batterysupply.cpp \
    src/debug_mode/scene_controlwater_graph/scene_controlwater_graph.cpp \
    src/debug_mode/scene_water_debug/scene2_BloodLeakCheck.cpp \
    src/debug_mode/scene_blood_debug/scene2_subpump.cpp \
    src/debug_mode/scene_water_debug/scene2_SetFormula.cpp \
    src/debug_mode/scene_water_debug/scene2_BeforeAfterPump.cpp \
    src/debug_mode/scene_water_debug/scene2_DegassingPump.cpp \
    src/debug_mode/scene_water_debug/scene2_DialystateFlow.cpp \
    src/debug_mode/scene_water_debug/scene2_ABPump.cpp \
    src/debug_mode/scene_water_debug/scene2_TempAdjust.cpp \
    src/debug_mode/scene_water_debug/scene2_CondAdjust.cpp \
    src/debug_mode/scene_water_debug/scene2_UFPump.cpp \
    src/httpserver/http_parser.cpp \
    src/httpserver/httpconnection.cpp \
    src/httpserver/httprequest.cpp \
    src/httpserver/httpresponse.cpp \
    src/httpserver/httpserver.cpp



QT += network
QT += sql xml

TRANSLATIONS += resource/languages/chinese.ts \
                resource/languages/english.ts


RESOURCES += hemodialysisviewer.qrc

isEmpty( Platform ) {
    Platform = HW_X86
    DEFINES += $$Platform
} else {
    Platform = HW_ARM
    DEFINES += $$Platform _MODULE_CAN
}

win32 {
    DEFINES += OS_WIN32
    INCLUDEPATH += depend/CanFestival-3-ab83333fee78/include/win32
}

unix {
    DEFINES += OS_LINUX
    DEFINES += CONFIG_CTRL_IFACE

    DEFINES += CONFIG_CTRL_IFACE_UNIX

    HEADERS += src/canopen/ObjDict.h \
               src/wlan/build_config.h \
               src/wlan/common.h \
               src/wlan/includes.h \
               src/wlan/userdatarequest.h \
               src/wlan/wlan.h \
               src/wlan/wpabuf.h \
               src/wlan/wpa_ctrl.h \
               src/wlan/wpa_debug.h \
               src/wlan/password.h

    SOURCES += src/canopen/ObjDict.c \
               src/wlan/os_unix.c \
               src/wlan/userdatarequest.cpp \
               src/wlan/wlan.cpp \
               src/wlan/common.c \
               src/wlan/wpabuf.c \
               src/wlan/wpa_ctrl.c \
               src/wlan/password.cpp \
               src/wlan/wpa_debug.c

    INCLUDEPATH += depend/CanFestival-3-ab83333fee78/include/unix
    LIBS +=    -L./depend/CanFestival-3-ab83333fee78/src -lcanfestival \
               -L./depend/CanFestival-3-ab83333fee78/drivers/unix -lcanfestival_unix \
               -lts -lz -ldl -lrt

    system(cd depend/CanFestival-3-ab83333fee78;./build_CanFestival.sh $$Platform;)
    system(make -C depend/CanFestival-3-ab83333fee78/objdictgen gnosis)
    system(python depend/CanFestival-3-ab83333fee78/objdictgen/objdictgen.py src/canopen/ObjDict.od src/canopen/ObjDict.c)

}

system(./version.sh)

FORMS += \
    src/engineering_mode/engineering_mode.ui

