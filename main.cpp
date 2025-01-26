
#include <QApplication>
#include "promptmanager.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Widget w;
    // w.show();
    PromptDB db("prompts.db");
    PromptManager manager(&db);
    manager.show();

    return a.exec();
}
