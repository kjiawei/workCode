#ifndef XML_CONFIG_H
#define XML_CONFIG_H

#include <stdio.h>
#include "config_xml.h"

/*!
 * \brief The XmlConfig class
 */
class XmlConfig
{
public:
    XmlConfig();
    virtual void CustomXmlConfig(const char* file, const char* cmd)=0;
};

void XmlConfigRead(const char* file=DEFAULT_XML_FILE);
void ReUpdateXmlConfig(const char* file=DEFAULT_XML_FILE, const char* cmd=DEFAULT_XML_LOAD);

#endif
