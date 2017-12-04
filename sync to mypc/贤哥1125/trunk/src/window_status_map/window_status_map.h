#ifndef WINDOW_STATUS_MAP_H
#define WINDOW_STATUS_MAP_H

#include <QVector>
#include <QWidget>
#include <QTabWidget>
#include "lang_switch.h"
#include "widget_common.h"
#include "widget_button.h"
#include "scene_status_user.h"
#include "scene_status_table.h"
#include "scene_status_graph.h"

/*!
 * \brief The WindowStatusMap class
 */
class WindowStatusMap : public QObject, public LangSetText, public PieceVisible
{
    Q_OBJECT

public:
    WindowStatusMap(QWidget *parent=0);
    void CustomSetText();
    void setCusVisible(bool vis=false, char* para=NULL);

private:
    inline void setUi(QWidget* parent=0);

private slots:
    void slotButtonGraph();
    void slotButtonTable();
    void slotButtonUser();
    void slotButtonReturn();

private:
    SceneStatusGraph sceneStatusGraph;  //水路图
    SceneStatusTable sceneStatusTable;  //表格
    SceneStatusUser sceneStatusUser;  //用户

    WidgetButton buttonGraph;
    WidgetButton buttonTable;
    WidgetButton buttonUser;
    WidgetButton buttonReturn;
};

#endif
