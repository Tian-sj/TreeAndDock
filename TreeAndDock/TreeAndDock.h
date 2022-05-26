#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_TreeAndDock.h"

class TreeAndDock : public QMainWindow
{
	Q_OBJECT

private:
	enum treeItemType { itTopItem = 1001, itGroupItem, itImageItem };
	enum treeColNum { colItem, colItemType };
	QPixmap curPixmap;
	double pixRatio;
	QLabel labFileName;

private:
	void iniTree();
	void changeItemCaption(QTreeWidgetItem* item);

public:
	TreeAndDock(QWidget* parent = Q_NULLPTR);

private:
	Ui::TreeAndDockClass ui;

private slots:
	void on_actDockFloat_triggered(bool checked);
	void on_actDockVisible_toggled(bool arg1);
	void on_dockWidget_topLevelChanged(bool toPleve);
	void on_dockWidget_visibilityChanged(bool visible);

	void on_actAddFolder_triggered();
	void on_actAddFiles_triggered();
	void on_treeWidget_currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous);
	void on_actDeleteItem_triggered();
	void on_actScanItems_triggered();

	void on_actZoomIn_triggered();
	void on_actZoomOut_triggered();
	void on_actZoomFitH_triggered();
	void on_actZoomFitW_triggered();
	void on_actZoomRealSize_triggered();

	void on_label_customContextMenuRequested(QPoint pos);
	void on_treeWidget_customContextMenuRequested(QPoint pos);
};
