#include <QFile>
#include <string.h>
#include <QTextStream>
#include <QDebug>
#include "xml_parse.h"

int xml_DocReadOpen(QDomDocument *doc, const char *path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return -1;
    if (!doc->setContent(&file)) {
        file.close();
        return -1;
    }
    file.close();
    return 0;
}

QDomNode xml_NodeFindSible(QDomNode node, char* name)
{
    QDomNode nodePtr = node;
    while (nodePtr.isNull() == false) {
        if (nodePtr.nodeName() == name) {
            return nodePtr;
        }
        nodePtr = nodePtr.nextSibling();
    }
    return QDomNode();
}

QDomElement xml_ElementFindSible(QDomElement node, char* name)
{
    QDomElement nodePtr = node;
    while (nodePtr.isNull() == false) {
        if (nodePtr.nodeName() == name) {
            return nodePtr;
        }
        nodePtr = nodePtr.nextSiblingElement();
    }
    return QDomElement();
}

QDomNode xml_NodeFind(QDomNode node, const char* path)
{
    if ((node.isNull() == true) || (path == NULL)) {
        return QDomNode();
    }

    char ps[100], *ptr=ps;
    int index=1;
    int count = strlen(path);
    QDomNode nodeptr = node;
    QDomNode nodeTmp;
    strcpy(ps, path);
    while (*ptr != 0) {
        ptr = ps+index;
        index += strcspn(ptr, "/");
        ps[index] = 0;
        nodeTmp = xml_NodeFindSible(nodeptr, ptr);
        if (nodeTmp.isNull() == true) {
            return QDomNode();
        }

        index++;
        if (index >= count) {
            return nodeTmp;
        } else if (nodeTmp.hasChildNodes() == false) {
            break;
        }

        nodeptr = nodeTmp.firstChild();
    }

    return QDomNode();
}

int xml_NodeReadValue(QDomDocument *doc, const char* path, QMap<QString,QString>* mapValue)
{
    if ((doc == NULL) || (path == NULL) || (mapValue == NULL)) {
        return -1;
    }

    int len, index;
    QDomNode nodeRet;
    QDomNamedNodeMap nodeMap;

    nodeRet = xml_NodeFind(doc->firstChild(), path);
    if (nodeRet.isNull() == true) {
        return -1;
    }

    nodeMap = nodeRet.attributes();
    len = nodeMap.length();
    for(index=0; index<len; index++){
        mapValue->insert(nodeMap.item(index).nodeName(),
                nodeMap.item(index).nodeValue());
    }
    return 0;
}

int xml_WriteAttr(QDomElement domElement, QMap<QString,QString> *mapValue)
{
    if (mapValue->empty() == true) {
        return -1;
    }

    QMap<QString,QString>::iterator it;
    for ( it = mapValue->begin(); it != mapValue->end(); ++it ) {
        domElement.setAttribute(it.key(), it.value());
    }
    return 0;
}

int xml_NodeWriteValue(QDomDocument *domDocument,const char *path, QMap<QString,QString> *mapValue)
{
    if ((domDocument == NULL) || (path == NULL) || (mapValue == NULL)) {
        return -1;
    }

    QDomElement nodeTmp, nodeFather;
    char ps[128], *ptr=ps;
    int index=1;
    int count = strlen(path);

    strcpy(ps, path);
    index += strcspn(ps+1, "/");
    ps[index] = 0;

    nodeFather = domDocument->firstChildElement();
    if (nodeFather.isNull() == true) {
        nodeFather = domDocument->createElement(ps+1);
        domDocument->appendChild(nodeFather);
    } else {
        nodeFather = xml_ElementFindSible(nodeFather, ps+1);
        if (nodeFather.isNull() == true) {
            return -1;
        }
    }
    index++;
    ptr = ps+index;

    while (*ptr != 0) {
        ptr = ps+index;
        index += strcspn(ptr, "/");
        ps[index] = 0;
        nodeTmp = xml_ElementFindSible(nodeFather.firstChildElement(), ptr);
        if (nodeTmp.isNull() == true) {
            nodeTmp = domDocument->createElement(ptr);
            nodeFather.appendChild(nodeTmp);
        }
        nodeFather = nodeTmp;
        index++;
        if (index >= count) {
            break;
        }
    }

    xml_WriteAttr(nodeFather, mapValue);

    return 0;
}


int waiteToFile(QDomDocument *domDocument, const char *fileName)
{
    QFile file(fileName);

    if (domDocument == NULL)
        return -1;

    if (!file.open(QFile::WriteOnly | QFile::Text)){
        return -1;
    }
    QTextStream streamW(&file);
    domDocument->save(streamW, 4);
    file.close();
    return 0;
}

#if 0
#include <QApplication>
int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    QMap<QString, QString> mapV;

    mapV.insert("treatMode", "HD");
    mapV.insert("needle", "double");
    mapV.insert("dilute", "pre-dilute");
    mapV.insert("fluidMode", "A+B");

    xml_NodeWriteValue("config.xml", "/Config/WorkMode/Default11/aa", &mapV);

    mapV.clear();
    return 0;
}
#endif
