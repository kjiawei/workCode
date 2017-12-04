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

#include "userdatarequest.h"
#include "wlan.h"
#include "wpa_ctrl.h"


void UserDataRequest::setupUi(QDialog *UserDataRequest)
{
    if (UserDataRequest->objectName().isEmpty())
        UserDataRequest->setObjectName(QString::fromUtf8("UserDataRequest"));
    UserDataRequest->resize(216, 103);
    UserDataRequest->setSizeGripEnabled(true);
    vboxLayout = new QVBoxLayout(UserDataRequest);
    vboxLayout->setSpacing(6);
    vboxLayout->setContentsMargins(11, 11, 11, 11);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    queryInfo = new QLabel(UserDataRequest);
    queryInfo->setObjectName(QString::fromUtf8("queryInfo"));

    vboxLayout->addWidget(queryInfo);

    hboxLayout = new QHBoxLayout();
    hboxLayout->setSpacing(6);
    hboxLayout->setContentsMargins(0, 0, 0, 0);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    queryField = new QLabel(UserDataRequest);
    queryField->setObjectName(QString::fromUtf8("queryField"));

    hboxLayout->addWidget(queryField);

    queryEdit = new QLineEdit(UserDataRequest);
    queryEdit->setObjectName(QString::fromUtf8("queryEdit"));
    queryEdit->setEnabled(true);
    queryEdit->setEchoMode(QLineEdit::Password);

    hboxLayout->addWidget(queryEdit);


    vboxLayout->addLayout(hboxLayout);

    hboxLayout1 = new QHBoxLayout();
    hboxLayout1->setSpacing(6);
    hboxLayout1->setContentsMargins(0, 0, 0, 0);
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    spacer4 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout1->addItem(spacer4);

    buttonOk = new QPushButton(UserDataRequest);
    buttonOk->setObjectName(QString::fromUtf8("buttonOk"));
    buttonOk->setAutoDefault(true);
    buttonOk->setDefault(true);

    hboxLayout1->addWidget(buttonOk);

    buttonCancel = new QPushButton(UserDataRequest);
    buttonCancel->setObjectName(QString::fromUtf8("buttonCancel"));
    buttonCancel->setAutoDefault(true);

    hboxLayout1->addWidget(buttonCancel);


    vboxLayout->addLayout(hboxLayout1);


    retranslateUi(UserDataRequest);

    QMetaObject::connectSlotsByName(UserDataRequest);
} // setupUi

void UserDataRequest::retranslateUi(QDialog *UserDataRequest)
{
    UserDataRequest->setWindowTitle(QApplication::translate("UserDataRequest", "Authentication credentials required", 0, QApplication::UnicodeUTF8));
    queryInfo->setText(QString());
    queryField->setText(QString());
    buttonOk->setText(QApplication::translate("UserDataRequest", "&OK", 0, QApplication::UnicodeUTF8));
    buttonOk->setShortcut(QString());
    buttonCancel->setText(QApplication::translate("UserDataRequest", "&Cancel", 0, QApplication::UnicodeUTF8));
    buttonCancel->setShortcut(QString());
} // retranslateUi

UserDataRequest::UserDataRequest(QWidget *parent, const char *, bool,
				 Qt::WFlags)
	: QDialog(parent)
{
	setupUi(this);

	connect(buttonOk, SIGNAL(clicked()), this, SLOT(sendReply()));
	connect(buttonCancel, SIGNAL(clicked()), this, SLOT(reject()));
	connect(queryEdit, SIGNAL(returnPressed()), this, SLOT(sendReply()));
}


UserDataRequest::~UserDataRequest()
{
}


void UserDataRequest::languageChange()
{
	retranslateUi(this);
}


int UserDataRequest::setParams(CWlan *_wlan, const char *reqMsg)
{
	char *tmp, *pos, *pos2;
    wlan = _wlan;
	tmp = strdup(reqMsg);
	if (tmp == NULL)
		return -1;
	pos = strchr(tmp, '-');
	if (pos == NULL) {
		free(tmp);
		return -1;
	}
	*pos++ = '\0';
	field = tmp;
	pos2 = strchr(pos, ':');
	if (pos2 == NULL) {
		free(tmp);
		return -1;
	}
	*pos2++ = '\0';

	networkid = atoi(pos);
	queryInfo->setText(pos2);
	if (strcmp(tmp, "PASSWORD") == 0) {
		queryField->setText("Password: ");
		queryEdit->setEchoMode(QLineEdit::Password);
	} else if (strcmp(tmp, "NEW_PASSWORD") == 0) {
		queryField->setText("New password: ");
		queryEdit->setEchoMode(QLineEdit::Password);
	} else if (strcmp(tmp, "IDENTITY") == 0)
		queryField->setText("Identity: ");
	else if (strcmp(tmp, "PASSPHRASE") == 0) {
		queryField->setText("Private key passphrase: ");
		queryEdit->setEchoMode(QLineEdit::Password);
	} else
		queryField->setText(field + ":");
	free(tmp);

	return 0;
}


void UserDataRequest::sendReply()
{
	char reply[10];
	size_t reply_len = sizeof(reply);

    if (wlan == NULL) {
		reject();
		return;
	}

	QString cmd = QString(WPA_CTRL_RSP) + field + '-' +
		QString::number(networkid) + ':' +
		queryEdit->text();
    wlan->ctrlRequest(cmd.toAscii().constData(), reply, &reply_len);
	accept();
}
