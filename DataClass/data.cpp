#include "data.h"

Data &Data::operator+(const QString &oth) noexcept {
    text += oth;
    return *this;
}
