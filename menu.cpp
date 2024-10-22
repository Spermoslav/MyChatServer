#include "menu.h"
#include "server.h"

Menu::Menu(QWidget *parent)
    : QWidget(parent),
      mainLay(new QVBoxLayout(this)),
      textBrowser(new QTextBrowser),
      server(new Server(this))
{
    mainLay->addWidget(textBrowser);
}

Menu::~Menu()
{
}

void Menu::textBrowserAppend(const QString &text)
{
    textBrowser->append(text);
}

