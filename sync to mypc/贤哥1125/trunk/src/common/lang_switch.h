#ifndef LANG_SWITCH_H
#define LANG_SWITCH_H

class LangSetText
{
public:
    LangSetText();
    virtual void CustomSetText()=0;
};

void ReUpdateLangText();

#endif
