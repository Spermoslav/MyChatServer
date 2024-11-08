#include "data.h"

Data &Data::operator+(const QString &oth) noexcept {
    text += oth;
    return *this;
}


bool writeNewAccount(const QString &data) {
    QString nick = data.split(' ')[0];

    if(findAccount(nick) == std::nullopt) {
        QFile dataF("Data/clientdata.txt");
        dataF.open(QIODevice::Append | QIODevice::Text);
        dataF.write(data.toLocal8Bit());
        dataF.write("\n");
        dataF.flush();
        return true;
    }
    else {
        return false;
    }
}

std::optional<QString> findAccount(const QString &nickname) {
    QFile dataF("Data/clientdata.txt");
    dataF.open(QIODevice::ReadOnly | QIODevice::Text);
    QStringList dataStr = QString::fromLocal8Bit(dataF.readAll()).split("\n");

    std::optional<QString> account = std::nullopt;
    for(QString& s : dataStr) {
        if(s == "") break;

        qsizetype nIt = 0;
        bool isFound = false;

        for(qsizetype j = 0; s[j] != ' '; ++j) {
            if(s[j] == nickname[nIt]) {
                ++nIt;

                if(nIt == nickname.size()) {
                    if(s[j + 1] != ' ') break;
                    isFound = true;
                    account = std::optional<QString> (s);
                    break;
                }
            }
            else {
                break;
            }
        }
        if(isFound) break;
    }
    return account;
}

std::pair<QString, QString> accDataSplit(const QString &accData)
{
    QStringList sl = accData.split(' ');
    return std::pair<QString, QString> (sl[0], sl[1]);
}
