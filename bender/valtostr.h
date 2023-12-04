#ifndef VALTOSTR_H
#define VALTOSTR_H

#include <QString>

class ValToStr {
  public:
    ValToStr();
    QString valXToString(uint32_t val);
    QString valYToString(uint32_t val);
};

#endif // VALTOSTR_H
