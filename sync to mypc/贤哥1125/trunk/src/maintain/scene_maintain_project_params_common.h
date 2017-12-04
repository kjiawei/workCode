#ifndef SCENE_PROJECT_MODE_PARAMS_COMMON_H
#define SCENE_PROJECT_MODE_PARAMS_COMMON_H

#include <QtGui>
#include <QLabel>
#include <QLineEdit>
#include <math.h>
#include "data_custom.h"
#include "xml_config.h"
#include "config_xml.h"
#include "page_info.h"

#include "widget_button.h"

#include "data_set.h"
#include "edittablewidgetitemdelege.h"
#include "data_custom.h"
#include "lang_switch.h"
class SceneProjectModeParamsCommon : public QWidget, public PieceVisible ,public ActionCustom,
        public XmlConfig,public LangSetText
{
    Q_OBJECT
public:
    SceneProjectModeParamsCommon(QWidget *parent=0);
    void CustomSetText();
    void CustomXmlConfig(const char *file=DEFAULT_XML_FILE, const char *cmd=DEFAULT_XML_LOAD);
    void setCusVisible(bool vis=false, char* para=NULL);
    void ActionHandle(const void* dataPtr=NULL, int name=0);

public slots:
    void setNextPage();
    void setLastPage();
    void button_return_clicked();
    void slotTableWidgetChange(int,int,QString);
    void saveParams(const char *file = DEFAULT_XML_FILE);
    void resetParams(const char *file = FACTORYRESET_XML_FILE);
    void itemName(QLineEdit *edit, int row, int column);

private:
    inline void setUi();
    void insertParams(QString type, QString name, float defaultV, float maxV, float minV, float stepV, QString unit);
    void insertParamsReadOnly(QString type, QString name, float defaultV, float maxV, float minV, float stepV, QString unit);
    void createParams(const QMap<QString, QString> mapV, struct Params_Attribute *attribute);
    void createXMLParams(const struct Params_Attribute attribute, QMap<QString, QString> *mapV);

private:
    QTableWidget * tableWidgetCommParams;
    QWidget *tableParent;
    QPushButton button_last;  //上一页
    QPushButton button_next;  //下一页
    QPushButton button_save;   //确认保存
    QPushButton button_reset;   //出厂设置
    QPushButton button_return;   //调试时返回#/
private:
    struct Params params;
    unsigned char currentPage;
    EditTableWidgetItemDelege *delege;
};

#endif
