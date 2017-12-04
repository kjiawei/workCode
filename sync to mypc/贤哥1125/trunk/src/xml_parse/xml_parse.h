#ifndef XML_PARSE_H
#define XML_PARSE_H

#include <QMap>
#include <QDomDocument>

int xml_NodeReadValue(QDomDocument *doc, const char* path=NULL, QMap<QString,QString>* mapValue=NULL);
int xml_NodeWriteValue(QDomDocument *domDocument, const char *path=NULL, QMap<QString,QString> *mapValue=NULL);
int waiteToFile(QDomDocument *domDocument, const char *fileName);
int xml_DocReadOpen(QDomDocument *doc, const char *path);

#endif
