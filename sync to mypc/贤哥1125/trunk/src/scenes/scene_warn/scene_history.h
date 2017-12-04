/*****************************************************
** 作者： Huangjunxian
** 创始时间：2014-1-12
** 修改人：Huangjunxian
** 修改时间：2014-1-12
** 描述：
**    5058报警记录表格页面 《头文件》
*****************************************************/
#ifndef SCENE_HISTORY_H
#define SCENE_HISTORY_H

#include <QtGui>
#include <QSqlTableModel>
#include <QSqlRecord>
#include "page_info.h"
#include "data_manage.h"
#include "global_var.h"
#include "Alarm_module.h"

#define Widget_Setup_Other_Menu_X             12
#define Widget_Setup_Other_Menu_Y             57
#define Widget_Setup_Other_Menu_Width         147
#define Widget_Setup_Other_Menu_High          696

#define TABLE_PAGE_SHOW_SIZE  15      //每页显示的记录条数

class SceneHistory : public QObject, public PieceVisible, public LangSetText,
        public ActionCustom
{
    Q_OBJECT
public:
    //构造函数#/
    SceneHistory(QWidget *parent=0);
    void setCusVisible(bool vis=false, char* para=NULL);
    //内容更新操作#/
    void ActionHandle(const void* dataPtr=NULL, int name=0);
    void CustomSetText();

signals:
    void selectRow(int);

public slots:
    //通过按钮选择当前或者历史报警#/
    void SlotSceneActivate(int id);
    //删除按键的操作#/
    void buttonDeleteClick();
    //根据选中行，在表格下方显示报警内容#/
    void currentTableWidgetClick(QTableWidgetItem*);
    void selectTableRow(int);
    //根据选中行，将该行对象保存，作为删除选中行的依据#/
    void historyTableWidgetClick(QTableWidgetItem * item);
    void buttonsHandle(int key);
    void firstPageHandle();   //第一页按钮处理
    void lastPageHandle();   //最后一页
    void upPageHandle();  //上一页
    void downPageHandle();   //下一页

private:
    inline void setUi();
    //隐藏弹出消息框，在浏览报警信息页面时，不弹出提示框#/
    //将一条报警内容插入当前报警表#/
    void insertCurrent(int row,AlarmInfor alarm);
    //将一条报警内容插入历史记录报警表#/
    void insertHistory(int row,int id,AlarmInfor alarm);
    //更新当前报警表#/
    void updateCurrentTable();
    //更新历史记录报警表#/
    void updateHistoryTable();
    //清空释放表格#/
    void clearTableContents(QTableWidget *table);
    void setLabelPageInfor(int cur,int sum);  //更新下方页码的信息
    void showCurrentTable(int page);  //显示当前报警表指定页
    void showHistoryTable(int page);  //显示历史报警表指定页

private:
    QLabel labelTitle;
    QWidget widget; ///左侧菜单栏#/
    WidgetButton buttonCurrent;
    WidgetButton buttonHistory;
    QPushButton buttonFirstPage;  //第一页#/
    QPushButton buttonLastPage; //最后一页#/
    QPushButton buttonUpPage;   //上一页#/
    QPushButton buttonDownPage; //下一页#/
    QPushButton buttonDelete;   //删除按键#/
    QButtonGroup buttonGroup;
    QLabel labelPageInfo;    //页码信息#/
    QLabel labelOperator;
    QTableWidget * tableWidgetHistory;  //历史记录表对象#/
    QTableWidget * tableWidgetCurrent;  //当前报警表对象#/

private:
    int currentTablePageNum;   //当前记录第几页#/
    int currentTablePageSum;   //当前记录总页数#/
    int historyTablePageNum;   //历史记录第几页#/
    int historyTablePageSum;   //历史记录总页数#/
    QTableWidgetItem *seldel;

};

#endif
