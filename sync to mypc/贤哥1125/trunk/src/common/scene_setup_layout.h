#ifndef SCENE_SETUP_LAYOUT_H
#define SCENE_SETUP_LAYOUT_H

#include "widget_common.h"

/* Style: label. lineedit. label(unit). */
#define SCENE_SETUP_COL_LABEL                   90
#define SCENE_SETUP_COL_LINEEDIT                395
#define SCENE_SETUP_COL_UNIT                    510

#define SCENE_SETUP_COL_WIDTH_LABEL             300
#define SCENE_SETUP_COL_WIDTH_EDIT              110
#define SCENE_SETUP_COL_WIDTH_COMBOBOX          110
#define SCENE_SETUP_COL_WIDTH_UNIT              100

#define SCENE_SETUP_WIDGET_HIGH                 38


#define SCENE_SETUP_COL_WIDTH_PARAMS_LABEL      110
#define SCENE_SETUP_COL_WIDTH_PARAMS_EDIT       95

#define SCENE_SETUP_PARAMS_TITLE_COL_1          150
#define SCENE_SETUP_PARAMS_TITLE_COL_2          355
#define SCENE_SETUP_PARAMS_TITLE_COL_3          555
#define SCENE_SETUP_PARAMS_TITLE_COL_4          755

#define SCENE_SETUP_PARAMS_EDIT_COL_1           260
#define SCENE_SETUP_PARAMS_EDIT_COL_2           465
#define SCENE_SETUP_PARAMS_EDIT_COL_3           665
#define SCENE_SETUP_PARAMS_EDIT_COL_4           865

#define SCENE_SETUP_COL_UNIT_PARAMS_1           360
#define SCENE_SETUP_COL_UNIT_PARAMS_2           565
#define SCENE_SETUP_COL_UNIT_PARAMS_3           765
#define SCENE_SETUP_COL_UNIT_PARAMS_4           965

#define SCENE_SETUP_ROW_1                       76
#define SCENE_SETUP_ROW_2                       (SCENE_SETUP_WIDGET_HIGH+SCENE_SETUP_ROW_1+7)
#define SCENE_SETUP_ROW_3                       (SCENE_SETUP_WIDGET_HIGH+SCENE_SETUP_ROW_2+7)
#define SCENE_SETUP_ROW_4                       (SCENE_SETUP_WIDGET_HIGH+SCENE_SETUP_ROW_3+7)
#define SCENE_SETUP_ROW_5                       (SCENE_SETUP_WIDGET_HIGH+SCENE_SETUP_ROW_4+7)
#define SCENE_SETUP_ROW_6                       (SCENE_SETUP_WIDGET_HIGH+SCENE_SETUP_ROW_5+7)
#define SCENE_SETUP_ROW_7                       (SCENE_SETUP_WIDGET_HIGH+SCENE_SETUP_ROW_6+7)
#define SCENE_SETUP_ROW_8                       (SCENE_SETUP_WIDGET_HIGH+SCENE_SETUP_ROW_7+7)
#define SCENE_SETUP_ROW_9                       (SCENE_SETUP_WIDGET_HIGH+SCENE_SETUP_ROW_8+7)
#define SCENE_SETUP_ROW_10                      (SCENE_SETUP_WIDGET_HIGH+SCENE_SETUP_ROW_9+7)
#define SCENE_SETUP_ROW_11                      (SCENE_SETUP_WIDGET_HIGH+SCENE_SETUP_ROW_10+7)
#define SCENE_SETUP_ROW_12                      (SCENE_SETUP_WIDGET_HIGH+SCENE_SETUP_ROW_11+7)
#define SCENE_SETUP_ROW_13                      (SCENE_SETUP_WIDGET_HIGH+SCENE_SETUP_ROW_12+7)
#define SCENE_SETUP_ROW_14                      (SCENE_SETUP_WIDGET_HIGH+SCENE_SETUP_ROW_13+7)


#define SCENE_SETUP_CREATE_PARAMS_TABLE(x,y,z)           \
    do {                                        \
        (x).setGeometry(QRect(SCENE_SETUP_PARAMS_TITLE_COL_##z,          \
                    SCENE_SETUP_ROW_##y,                        \
                    SCENE_SETUP_COL_WIDTH_PARAMS_LABEL,         \
                    SCENE_SETUP_WIDGET_HIGH));;                 \
        (x).setAlignment( Qt::AlignVCenter | Qt::AlignRight); \
        (x).setFont(QFont(QString(CONFIG_FONT_STYLE),         \
                    CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL)); \
        (x).setStyleSheet(CONFIG_COLOR_LABEL);                \
    } while(0)

#define SCENE_SETUP_CREATE_PARAMS_LINEEDIT(x,y,z)        \
    do {                                        \
        (x).setGeometry(QRect(SCENE_SETUP_PARAMS_EDIT_COL_##z, SCENE_SETUP_ROW_##y, SCENE_SETUP_COL_WIDTH_PARAMS_EDIT,  \
                SCENE_SETUP_WIDGET_HIGH));          \
        LINEEDIT_STYLE(x);                      \
    } while(0)

#define SCENE_SETUP_CREATE_PARAMS_UNIT(x,y,z)            \
        do {                                    \
            (x).setGeometry(QRect(SCENE_SETUP_COL_UNIT_PARAMS_##z,       \
                        SCENE_SETUP_ROW_##y,                \
                        SCENE_SETUP_COL_WIDTH_UNIT,         \
                        SCENE_SETUP_WIDGET_HIGH));          \
            (x).setAlignment( Qt::AlignVCenter | Qt::AlignLeft); \
            (x).setFont(QFont(QString(CONFIG_FONT_STYLE),     \
                        CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL)); \
            (x).setStyleSheet(CONFIG_COLOR_LABEL);    \
        } while(0)


/* Create a table, x:QLabel, y:1~7 */
#define SCENE_SETUP_CREATE_TABLE(x,y)           \
    do {                                        \
        (x).setGeometry(QRect(SCENE_SETUP_COL_LABEL,          \
                    SCENE_SETUP_ROW_##y,                        \
                    SCENE_SETUP_COL_WIDTH_LABEL,                \
                    SCENE_SETUP_WIDGET_HIGH));;                 \
        (x).setAlignment( Qt::AlignVCenter | Qt::AlignRight); \
    (x).setFont(QFont(QString(CONFIG_FONT_STYLE),         \
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL)); \
        (x).setStyleSheet(CONFIG_COLOR_LABEL);                \
    } while(0)

/* Create a widget */
#define SCENE_SETUP_WIDGET_XYW(p, x, y, w)      \
    do {                                        \
        (p).setGeometry(QRect(x, SCENE_SETUP_ROW_##y, w,  \
                    SCENE_SETUP_WIDGET_HIGH));          \
    } while(0)

#define SCENE_SETUP_WIDGET(x,y,w)            \
    SCENE_SETUP_WIDGET_XYW((x), SCENE_SETUP_COL_LINEEDIT, y, w)

/* Create a LineEdit, x:QLineEdit, y:1~7 */
#define SCENE_SETUP_CREATE_EDIT(x,y)        \
    SCENE_SETUP_WIDGET((x),y,SCENE_SETUP_COL_WIDTH_EDIT)

#define SCENE_SETUP_CREATE_LINEEDIT(x,y)        \
    do {                                        \
        SCENE_SETUP_CREATE_EDIT((x),y);           \
        LINEEDIT_STYLE(x);                      \
    } while(0)

#define SCENE_SETUP_CREATE_SHOWSTYLE(x,y)        \
    do {                                        \
        SCENE_SETUP_CREATE_EDIT((x),y);           \
        LINEEDIT_SHOWSTYLE(x);                      \
    } while(0)

/* Create a value table, x:QLabel, y:1~7 */
#define SCENE_SETUP_CREATE_VALUE(x,y)           \
    do {                                        \
        SCENE_SETUP_CREATE_EDIT(x,y);           \
        x.setFont(QFont(QString(CONFIG_FONT_STYLE),         \
            CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));      \
        x.setAlignment(Qt::AlignCenter);        \
        x.setStyleSheet(CONFIG_COLOR_LABEL);    \
    } while(0)

/* Create a value QComboBox, x:QComboBox, y:1~7 */
#define SCENE_SETUP_CREATE_COMBOBOX(x,y)        \
    do {                                        \
        (x).setGeometry(QRect(SCENE_SETUP_COL_LINEEDIT,   \
                    SCENE_SETUP_ROW_##y+4,              \
                    SCENE_SETUP_COL_WIDTH_COMBOBOX,     \
                    SCENE_SETUP_WIDGET_HIGH-4));        \
        (x).setFont(QFont(QString(CONFIG_FONT_STYLE),     \
                    CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL)); \
        COMBOBOX_STYLE(x);                      \
    } while(0)

/* Create a unit table, x:QLabel, y:1~7 */
#define SCENE_SETUP_CREATE_UNIT(x,y)            \
        do {                                    \
            (x).setGeometry(QRect(SCENE_SETUP_COL_UNIT,       \
                        SCENE_SETUP_ROW_##y,                \
                        SCENE_SETUP_COL_WIDTH_UNIT,         \
                        SCENE_SETUP_WIDGET_HIGH));          \
            (x).setAlignment( Qt::AlignVCenter | Qt::AlignLeft); \
            (x).setFont(QFont(QString(CONFIG_FONT_STYLE),     \
                        CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL)); \
            (x).setStyleSheet(CONFIG_COLOR_LABEL);    \
        } while(0)

#endif
