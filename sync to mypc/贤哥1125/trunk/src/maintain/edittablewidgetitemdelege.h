#ifndef EDITTABLEWIDGETITEMDELEGE_H
#define EDITTABLEWIDGETITEMDELEGE_H
#include <QtGui>
#include <QItemDelegate>

#include "keysetready.h"
class EditTableWidgetItemDelege : public QItemDelegate, public KeySetReady
{
    Q_OBJECT

public:
    EditTableWidgetItemDelege(QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void lineEdit_init() { }

    void lineEdit_data_set(const char *name = NULL, const char *data = NULL);

    void lineEdit_close(const char *name);

signals:
    void sendKeyListener(QLineEdit *edit, int row, int column) const;
    void tableWidgetEditListener(int row, int column, QString text);
public slots:
    void addKeyListener(QLineEdit *edit, int row, int column);
    void commitAndCloseEditor();
private:
    QLineEdit *currentEdit;
    int row;
    int column;
};

#endif // EDITTABLEWIDGETITEMDELEGE_H
