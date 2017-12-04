#include "page_info.h"
#include "window_status_map.h"
#include "Alarm_module.h"

inline void WindowStatusMap::setUi(QWidget* parent)
{
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/resource/images/background_pure.bmp")));
    parent->setAutoFillBackground(true);
    parent->setPalette(palette);

    buttonGraph.setStyleNormal(":/resource/images/status/status_button.bmp",
            QRect(0, 698, 255, 68));
    buttonGraph.setTextFontPos(20, CONFIG_FONT_NORMAL, 0xFF031e37,
            QRect(0, 0, 255, 68));

    buttonTable.setStyleNormal(":/resource/images/status/status_button.bmp",
            QRect(255, 698, 255, 68));
    buttonTable.setTextFontPos(20, CONFIG_FONT_NORMAL, 0xFF031e37,
            QRect(0, 0, 255, 68));

    buttonUser.setStyleNormal(":/resource/images/status/status_button.bmp",
            QRect(510, 698, 255, 68));
    buttonUser.setTextFontPos(20, CONFIG_FONT_NORMAL, 0xFF031e37,
            QRect(0, 0, 255, 68));

    buttonReturn.setStyleNormal(":/resource/images/status/status_button.bmp",
            QRect(765, 698, 255, 68));
    buttonReturn.setTextFontPos(20, CONFIG_FONT_NORMAL, 0xFF031e37,
            QRect(0, 0, 255, 68));
}

WindowStatusMap::WindowStatusMap(QWidget* parent):
    QObject(parent),
    sceneStatusGraph(parent),
    sceneStatusTable(parent),
    sceneStatusUser(parent),
    buttonGraph(parent),
    buttonTable(parent),
    buttonUser(parent),
    buttonReturn(parent)
{
    setUi(parent);

    connect(&buttonGraph, SIGNAL(clicked()), this, SLOT(slotButtonGraph()));
    connect(&buttonTable, SIGNAL(clicked()), this, SLOT(slotButtonTable()));
    connect(&buttonUser, SIGNAL(clicked()), this, SLOT(slotButtonUser()));
    connect(&buttonReturn, SIGNAL(clicked()), this, SLOT(slotButtonReturn()));

    PageAppendPiece("Status Graph", this);
    PageAppendPiece("Status Table", this);
    PageAppendPiece("Status User", this);
}

void WindowStatusMap::CustomSetText()
{
    buttonGraph.setText(tr("Graph"));
    buttonTable.setText(tr("Table"));
    buttonUser.setText(tr("User"));
    buttonReturn.setText(tr("Return"));
}

void WindowStatusMap::setCusVisible(bool vis, char* para)
{
    para = para;    
    buttonGraph.setVisible(vis);
    buttonTable.setVisible(vis);
    buttonUser.setVisible(vis);
    buttonReturn.setVisible(vis);

 /*
    if(vis==true)
    {
        if(alarmmodule!=NULL)
            alarmmodule->Set_WorkState(DE_HIDDED_FLAG);
    }
    else
    {
        if(alarmmodule!=NULL)
            alarmmodule->Set_WorkState(DE_NORMAL_FLAG);
    }
    */
}

void WindowStatusMap::slotButtonGraph()
{
    PageShow("Status Graph");
}

void WindowStatusMap::slotButtonTable()
{
    PageShow("Status Table");
}

void WindowStatusMap::slotButtonUser()
{
    PageShow("Status User");
}

void WindowStatusMap::slotButtonReturn()
{
    if(alarmmodule!=NULL)
        alarmmodule->Set_WorkState(DE_NORMAL_FLAG);
    PageShow("Main");
}
