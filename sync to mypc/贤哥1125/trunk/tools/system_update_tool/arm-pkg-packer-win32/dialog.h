#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <stdint.h>
#include <QErrorMessage>
#include "module.h"
#include <QtGui>

namespace Ui {
class dialog;
}

class dialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit dialog(QWidget *parent = 0);
    ~dialog();
    
private:
    Ui::dialog *ui;
    QErrorMessage *errorMessageDialog;
    uint32_t get_file_size(const char *filename);
    int pkg(struct Args_t args);

    QLabel * product_label;
    QComboBox * product_combo;
    struct TMP_MODULE module[256];
    QLabel * output_label;
    QLineEdit * output_Edit;
    QPushButton * output_Button;
    QPushButton * pack_Button;

    int count;

private slots:void module_Button_clicked(int i);
private slots:void output_Button_clicked();
private slots:void pack_Button_clicked();

};

#endif // DIALOG_H
