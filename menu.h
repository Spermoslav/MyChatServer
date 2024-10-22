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

    void textBrowserAppend(const QString& text);

private:
    QTextBrowser* textBrowser;
    QVBoxLayout* mainLay;
};
#endif // MENU_H
