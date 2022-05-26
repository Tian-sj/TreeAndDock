#include "TreeAndDock.h"
#include <QFileDialog>
#include <QMessageBox>

TreeAndDock::TreeAndDock(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->setCentralWidget(ui.scrollArea);
	iniTree();
	ui.statusBar->addWidget(&labFileName);
}

void TreeAndDock::iniTree()
{
	ui.treeWidget->clear();
	QIcon icon(":/images/icons/15.ico");
	QTreeWidgetItem* item = new QTreeWidgetItem(itTopItem);
	item->setIcon(colItem, icon);
	item->setText(colItem, "图片文件");
	item->setText(colItemType, "根目录");
	item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsAutoTristate);
	item->setCheckState(colItem, Qt::Checked);
	item->setData(colItem, Qt::UserRole, QVariant(""));
	ui.treeWidget->addTopLevelItem(item);
}

void TreeAndDock::changeItemCaption(QTreeWidgetItem* item)
{
	QString str = "*" + item->text(colItem);
	item->setText(colItem, str);
	for (int i = 0; i < item->childCount(); ++i) {
		changeItemCaption(item->child(i));
	}
}

void TreeAndDock::on_actDockVisible_toggled(bool arg1)
{
	ui.dockWidget->setVisible(arg1);
}

void TreeAndDock::on_dockWidget_topLevelChanged(bool toPleve)
{
	ui.actDockFloat->setChecked(toPleve);
}

void TreeAndDock::on_dockWidget_visibilityChanged(bool visible)
{
	ui.actDockVisible->setChecked(visible);
}

void TreeAndDock::on_treeWidget_currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous)
{
	Q_UNUSED(previous);
	if (current == NULL) return;
	int var = current->type();
	switch (var)
	{
	case itTopItem:
		ui.actAddFolder->setEnabled(true);
		ui.actAddFiles->setEnabled(true);
		ui.actDeleteItem->setEnabled(false);
		labFileName.setText(current->data(colItem, Qt::UserRole).toString());
		break;
	case itGroupItem:
		ui.actAddFolder->setEnabled(true);
		ui.actAddFiles->setEnabled(true);
		ui.actDeleteItem->setEnabled(true);
		labFileName.setText(current->data(colItem, Qt::UserRole).toString());
		break;
	case itImageItem:
		ui.actAddFolder->setEnabled(false);
		ui.actAddFiles->setEnabled(true);
		ui.actDeleteItem->setEnabled(true);
		ui.actZoomIn->setEnabled(true);
		ui.actZoomOut->setEnabled(true);
		ui.actZoomRealSize->setEnabled(true);
		ui.actZoomFitH->setEnabled(true);
		ui.actZoomFitW->setEnabled(true);
		labFileName.setText(current->data(colItem, Qt::UserRole).toString());
		curPixmap.load(current->data(colItem, Qt::UserRole).toString());
		on_actZoomFitH_triggered();
		break;
	}
}

void TreeAndDock::on_actZoomIn_triggered()
{
	pixRatio *= 1.2;

	int w = pixRatio * curPixmap.width();
	int h = pixRatio * curPixmap.height();

	QPixmap pix = curPixmap.scaled(w, h);
	ui.label->setPixmap(pix);
}

void TreeAndDock::on_actScanItems_triggered()
{
	for (int i = 0; i < ui.treeWidget->topLevelItemCount(); ++i) {
		changeItemCaption(ui.treeWidget->topLevelItem(i));
	}
}

void TreeAndDock::on_actZoomOut_triggered()
{
	pixRatio *= 0.8;

	int w = pixRatio * curPixmap.width();
	int h = pixRatio * curPixmap.height();

	QPixmap pix = curPixmap.scaled(w, h);
	ui.label->setPixmap(pix);
}

void TreeAndDock::on_label_customContextMenuRequested(QPoint pos)
{
	Q_UNUSED(pos);
	QMenu* menuList = new QMenu(this);

	menuList->addAction(ui.actZoomIn);
	menuList->addAction(ui.actZoomOut);
	menuList->addAction(ui.actZoomRealSize);
	menuList->addAction(ui.actZoomFitW);
	menuList->addAction(ui.actZoomFitH);
	menuList->addSeparator();
	menuList->addAction(ui.actDeleteItem);

	menuList->exec(QCursor::pos());
	delete menuList;
}

void TreeAndDock::on_treeWidget_customContextMenuRequested(QPoint pos)
{
	Q_UNUSED(pos);
	QMenu* menuList = new QMenu(this);

	menuList->addAction(ui.actAddFolder);
	menuList->addAction(ui.actAddFiles);
	menuList->addAction(ui.actDeleteItem);
	menuList->addAction(ui.actScanItems);
	menuList->addSeparator();
	menuList->addAction(ui.actDockFloat);

	menuList->exec(QCursor::pos());
	delete menuList;
}

void TreeAndDock::on_actZoomRealSize_triggered()
{
	pixRatio = 1;
	ui.label->setPixmap(curPixmap);
}

void TreeAndDock::on_actZoomFitH_triggered()
{
	int H = ui.scrollArea->height();
	int realH = curPixmap.height();
	pixRatio = (double)H / realH;
	QPixmap pix = curPixmap.scaledToHeight(H - 30);
	ui.label->setPixmap(pix);
}

void TreeAndDock::on_actDeleteItem_triggered()
{
	QTreeWidgetItem* item = ui.treeWidget->currentItem();
	QTreeWidgetItem* parItem = item->parent();
	parItem->removeChild(item);
	delete item;
}

void TreeAndDock::on_actZoomFitW_triggered()
{
	int W = ui.scrollArea->width();
	int realW = curPixmap.width();
	pixRatio = (double)W / realW;
	QPixmap pix = curPixmap.scaledToWidth(W - 30);
	ui.label->setPixmap(pix);
}

void TreeAndDock::on_actAddFolder_triggered()
{
	if (!ui.treeWidget->currentIndex().isValid()) {
		QMessageBox::warning(this, tr("警告"), tr("没有选中根目录"));
		return;
	}
	QString fullname = QFileDialog::getExistingDirectory();
	if (!fullname.isEmpty()) {
		QString dir = fullname.right(fullname.length() - fullname.lastIndexOf("/") - 1);
		QTreeWidgetItem* parItem = ui.treeWidget->currentItem();
		QTreeWidgetItem* item = new QTreeWidgetItem(itGroupItem);
		QIcon icon(":/images/icons/open3.bmp");
		item->setIcon(colItem, icon);
		item->setText(colItem, dir);
		item->setText(colItemType, "type=itGroupItem");
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsAutoTristate);
		item->setCheckState(colItem, Qt::Checked);
		item->setData(colItem, Qt::UserRole, QVariant(fullname));

		parItem->addChild(item);
	}
}

void TreeAndDock::on_actAddFiles_triggered()
{
	if (!ui.treeWidget->currentIndex().isValid()) {
		QMessageBox::warning(this, tr("警告"), tr("没有选中根目录"));
		return;
	}
	QStringList files = QFileDialog::getOpenFileNames(this, "选择一个或多个文件", "", "Images(*.jpg *.png)");

	if (files.isEmpty())	return;

	QTreeWidgetItem* parItem;
	QTreeWidgetItem* item = ui.treeWidget->currentItem();
	if (item->type() == itImageItem)
		parItem = item->parent();
	else
		parItem = item;
	foreach(auto file, files) {
		item = new QTreeWidgetItem(itImageItem);
		QString str = file.right(file.length() - 1 - file.lastIndexOf("/"));
		QIcon icon(":/images/icons/31.ico");
		item->setIcon(colItem, icon);
		item->setText(colItem, str);
		item->setText(colItemType, tr("图片"));
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsAutoTristate | Qt::ItemIsUserCheckable);
		item->setCheckState(colItem, Qt::Checked);
		item->setData(colItem, Qt::UserRole, file);
		parItem->addChild(item);
	}
}

void TreeAndDock::on_actDockFloat_triggered(bool checked)
{
	ui.dockWidget->setFloating(checked);
}
