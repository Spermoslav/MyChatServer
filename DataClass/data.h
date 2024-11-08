#ifndef DATA_H
#define DATA_H

#include <QString>
#include <QFile>
#include <QDebug>
#include <optional>

enum DataType
{
    Message,
    Info,
    Login,
    Registration
};

struct Data
{
    inline explicit Data(const QString& text = "", const DataType type = Message)
        : text(text), type(type) {}

    Data& operator+(const QString& oth) noexcept;

    QString text;
    DataType type;
};
