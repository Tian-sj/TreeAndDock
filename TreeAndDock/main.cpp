#include "TreeAndDock.h"
#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	TreeAndDock w;
	w.show();
	return a.exec();
}
