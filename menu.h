#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include <QTextBrowser>
#include <QVBoxLayout>

class Menu : public QWidget
{
    Q_OBJECT

public:
    Menu(QWidget *parent = nullptr);
    ~Menu();
};
#endif // MENU_H
