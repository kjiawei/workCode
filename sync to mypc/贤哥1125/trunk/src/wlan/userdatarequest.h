/*
 * wpa_gui - UserDataRequest class
 * Copyright (c) 2005-2006, Jouni Malinen <j@w1.fi>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Alternatively, this software may be distributed under the terms of BSD
 * license.
 *
 * See README and COPYING for more details.
 */

#ifndef USERDATAREQUEST_H
#define USERDATAREQUEST_H

#include <QObject>
#include <QtGui>

class CWlan;

class UserDataRequest : public QDialog
{
	Q_OBJECT

public:
	UserDataRequest(QWidget *parent = 0, const char *name = 0,
			bool modal = false, Qt::WFlags fl = 0);
	~UserDataRequest();

    int setParams(CWlan *_wlan, const char *reqMsg);

public slots:
	virtual void sendReply();

protected slots:
	virtual void languageChange();

private:
    CWlan *wlan;
	int networkid;
	QString field;

    QVBoxLayout *vboxLayout;
    QLabel *queryInfo;
    QHBoxLayout *hboxLayout;
    QLabel *queryField;
    QLineEdit *queryEdit;
    QHBoxLayout *hboxLayout1;
    QSpacerItem *spacer4;
    QPushButton *buttonOk;
    QPushButton *buttonCancel;

    void setupUi(QDialog *UserDataRequest);
    void retranslateUi(QDialog *UserDataRequest);
};

#endif /* USERDATAREQUEST_H */
