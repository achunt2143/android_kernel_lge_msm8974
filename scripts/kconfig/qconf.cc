<<<<<<< HEAD
/*
 * Copyright (C) 2002 Roman Zippel <zippel@linux-m68k.org>
 * Released under the terms of the GNU GPL v2.0.
 */

#include <qglobal.h>

#if QT_VERSION < 0x040000
#include <qmainwindow.h>
#include <qvbox.h>
#include <qvaluelist.h>
#include <qtextbrowser.h>
#include <qaction.h>
#include <qheader.h>
#include <qfiledialog.h>
#include <qdragobject.h>
#include <qpopupmenu.h>
#else
#include <q3mainwindow.h>
#include <q3vbox.h>
#include <q3valuelist.h>
#include <q3textbrowser.h>
#include <q3action.h>
#include <q3header.h>
#include <q3filedialog.h>
#include <q3dragobject.h>
#include <q3popupmenu.h>
#endif

#include <qapplication.h>
#include <qdesktopwidget.h>
#include <qtoolbar.h>
#include <qlayout.h>
#include <qsplitter.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qmenubar.h>
#include <qmessagebox.h>
#include <qregexp.h>
#include <qevent.h>
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2002 Roman Zippel <zippel@linux-m68k.org>
 * Copyright (C) 2015 Boris Barbulovski <bbarbulovski@gmail.com>
 */

#include <QAction>
#include <QActionGroup>
#include <QApplication>
#include <QCloseEvent>
#include <QDebug>
#include <QFileDialog>
#include <QLabel>
#include <QLayout>
#include <QList>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QRegularExpression>
#include <QScreen>
#include <QToolBar>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <stdlib.h>

#include "lkc.h"
#include "qconf.h"

<<<<<<< HEAD
#include "qconf.moc"
#include "images.c"

#ifdef _
# undef _
# define _ qgettext
#endif
=======
#include "images.h"

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static QApplication *configApp;
static ConfigSettings *configSettings;

<<<<<<< HEAD
Q3Action *ConfigMainWindow::saveAction;

static inline QString qgettext(const char* str)
{
	return QString::fromLocal8Bit(gettext(str));
}

static inline QString qgettext(const QString& str)
{
	return QString::fromLocal8Bit(gettext(str.latin1()));
=======
QAction *ConfigMainWindow::saveAction;

ConfigSettings::ConfigSettings()
	: QSettings("kernel.org", "qconf")
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * Reads a list of integer values from the application settings.
 */
<<<<<<< HEAD
Q3ValueList<int> ConfigSettings::readSizes(const QString& key, bool *ok)
{
	Q3ValueList<int> result;
	QStringList entryList = readListEntry(key, ok);
	QStringList::Iterator it;

	for (it = entryList.begin(); it != entryList.end(); ++it)
		result.push_back((*it).toInt());
=======
QList<int> ConfigSettings::readSizes(const QString& key, bool *ok)
{
	QList<int> result;

	if (contains(key))
	{
		QStringList entryList = value(key).toStringList();
		QStringList::Iterator it;

		for (it = entryList.begin(); it != entryList.end(); ++it)
			result.push_back((*it).toInt());

		*ok = true;
	}
	else
		*ok = false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return result;
}

/**
 * Writes a list of integer values to the application settings.
 */
<<<<<<< HEAD
bool ConfigSettings::writeSizes(const QString& key, const Q3ValueList<int>& value)
{
	QStringList stringList;
	Q3ValueList<int>::ConstIterator it;

	for (it = value.begin(); it != value.end(); ++it)
		stringList.push_back(QString::number(*it));
	return writeEntry(key, stringList);
}


/*
 * set the new data
 * TODO check the value
 */
void ConfigItem::okRename(int col)
{
	Parent::okRename(col);
	sym_set_string_value(menu->sym, text(dataColIdx).latin1());
	listView()->updateList(this);
}
=======
bool ConfigSettings::writeSizes(const QString& key, const QList<int>& value)
{
	QStringList stringList;
	QList<int>::ConstIterator it;

	for (it = value.begin(); it != value.end(); ++it)
		stringList.push_back(QString::number(*it));
	setValue(key, stringList);

	return true;
}

QIcon ConfigItem::symbolYesIcon;
QIcon ConfigItem::symbolModIcon;
QIcon ConfigItem::symbolNoIcon;
QIcon ConfigItem::choiceYesIcon;
QIcon ConfigItem::choiceNoIcon;
QIcon ConfigItem::menuIcon;
QIcon ConfigItem::menubackIcon;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * update the displayed of a menu entry
 */
void ConfigItem::updateMenu(void)
{
	ConfigList* list;
	struct symbol* sym;
	struct property *prop;
	QString prompt;
	int type;
	tristate expr;

	list = listView();
	if (goParent) {
<<<<<<< HEAD
		setPixmap(promptColIdx, list->menuBackPix);
=======
		setIcon(promptColIdx, menubackIcon);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		prompt = "..";
		goto set_prompt;
	}

	sym = menu->sym;
	prop = menu->prompt;
<<<<<<< HEAD
	prompt = _(menu_get_prompt(menu));
=======
	prompt = menu_get_prompt(menu);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (prop) switch (prop->type) {
	case P_MENU:
		if (list->mode == singleMode || list->mode == symbolMode) {
			/* a menuconfig entry is displayed differently
			 * depending whether it's at the view root or a child.
			 */
			if (sym && list->rootEntry == menu)
				break;
<<<<<<< HEAD
			setPixmap(promptColIdx, list->menuPix);
		} else {
			if (sym)
				break;
			setPixmap(promptColIdx, 0);
		}
		goto set_prompt;
	case P_COMMENT:
		setPixmap(promptColIdx, 0);
=======
			setIcon(promptColIdx, menuIcon);
		} else {
			if (sym)
				break;
			setIcon(promptColIdx, QIcon());
		}
		goto set_prompt;
	case P_COMMENT:
		setIcon(promptColIdx, QIcon());
		prompt = "*** " + prompt + " ***";
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto set_prompt;
	default:
		;
	}
	if (!sym)
		goto set_prompt;

<<<<<<< HEAD
	setText(nameColIdx, QString::fromLocal8Bit(sym->name));
=======
	setText(nameColIdx, sym->name);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	type = sym_get_type(sym);
	switch (type) {
	case S_BOOLEAN:
	case S_TRISTATE:
		char ch;

<<<<<<< HEAD
		if (!sym_is_changable(sym) && list->optMode == normalOpt) {
			setPixmap(promptColIdx, 0);
			setText(noColIdx, QString::null);
			setText(modColIdx, QString::null);
			setText(yesColIdx, QString::null);
=======
		if (!sym_is_changeable(sym) && list->optMode == normalOpt) {
			setIcon(promptColIdx, QIcon());
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		}
		expr = sym_get_tristate_value(sym);
		switch (expr) {
		case yes:
			if (sym_is_choice_value(sym) && type == S_BOOLEAN)
<<<<<<< HEAD
				setPixmap(promptColIdx, list->choiceYesPix);
			else
				setPixmap(promptColIdx, list->symbolYesPix);
			setText(yesColIdx, "Y");
			ch = 'Y';
			break;
		case mod:
			setPixmap(promptColIdx, list->symbolModPix);
			setText(modColIdx, "M");
=======
				setIcon(promptColIdx, choiceYesIcon);
			else
				setIcon(promptColIdx, symbolYesIcon);
			ch = 'Y';
			break;
		case mod:
			setIcon(promptColIdx, symbolModIcon);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			ch = 'M';
			break;
		default:
			if (sym_is_choice_value(sym) && type == S_BOOLEAN)
<<<<<<< HEAD
				setPixmap(promptColIdx, list->choiceNoPix);
			else
				setPixmap(promptColIdx, list->symbolNoPix);
			setText(noColIdx, "N");
			ch = 'N';
			break;
		}
		if (expr != no)
			setText(noColIdx, sym_tristate_within_range(sym, no) ? "_" : 0);
		if (expr != mod)
			setText(modColIdx, sym_tristate_within_range(sym, mod) ? "_" : 0);
		if (expr != yes)
			setText(yesColIdx, sym_tristate_within_range(sym, yes) ? "_" : 0);
=======
				setIcon(promptColIdx, choiceNoIcon);
			else
				setIcon(promptColIdx, symbolNoIcon);
			ch = 'N';
			break;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		setText(dataColIdx, QChar(ch));
		break;
	case S_INT:
	case S_HEX:
	case S_STRING:
<<<<<<< HEAD
		const char* data;

		data = sym_get_string_value(sym);

		int i = list->mapIdx(dataColIdx);
		if (i >= 0)
			setRenameEnabled(i, TRUE);
		setText(dataColIdx, data);
		if (type == S_STRING)
			prompt = QString("%1: %2").arg(prompt).arg(data);
		else
			prompt = QString("(%2) %1").arg(prompt).arg(data);
		break;
	}
	if (!sym_has_value(sym) && visible)
		prompt += _(" (NEW)");
=======
		setText(dataColIdx, sym_get_string_value(sym));
		break;
	}
	if (!sym_has_value(sym) && visible)
		prompt += " (NEW)";
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
set_prompt:
	setText(promptColIdx, prompt);
}

void ConfigItem::testUpdateMenu(bool v)
{
	ConfigItem* i;

	visible = v;
	if (!menu)
		return;

	sym_calc_value(menu->sym);
	if (menu->flags & MENU_CHANGED) {
		/* the menu entry changed, so update all list items */
		menu->flags &= ~MENU_CHANGED;
		for (i = (ConfigItem*)menu->data; i; i = i->nextItem)
			i->updateMenu();
	} else if (listView()->updateAll)
		updateMenu();
}

<<<<<<< HEAD
void ConfigItem::paintCell(QPainter* p, const QColorGroup& cg, int column, int width, int align)
{
	ConfigList* list = listView();

	if (visible) {
		if (isSelected() && !list->hasFocus() && list->mode == menuMode)
			Parent::paintCell(p, list->inactivedColorGroup, column, width, align);
		else
			Parent::paintCell(p, cg, column, width, align);
	} else
		Parent::paintCell(p, list->disabledColorGroup, column, width, align);
}
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * construct a menu entry
 */
void ConfigItem::init(void)
{
	if (menu) {
		ConfigList* list = listView();
		nextItem = (ConfigItem*)menu->data;
		menu->data = this;

		if (list->mode != fullMode)
<<<<<<< HEAD
			setOpen(TRUE);
		sym_calc_value(menu->sym);
=======
			setExpanded(true);
		sym_calc_value(menu->sym);

		if (menu->sym) {
			enum symbol_type type = menu->sym->type;

			// Allow to edit "int", "hex", and "string" in-place in
			// the data column. Unfortunately, you cannot specify
			// the flags per column. Set ItemIsEditable for all
			// columns here, and check the column in createEditor().
			if (type == S_INT || type == S_HEX || type == S_STRING)
				setFlags(flags() | Qt::ItemIsEditable);
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	updateMenu();
}

/*
 * destruct a menu entry
 */
ConfigItem::~ConfigItem(void)
{
	if (menu) {
		ConfigItem** ip = (ConfigItem**)&menu->data;
		for (; *ip; ip = &(*ip)->nextItem) {
			if (*ip == this) {
				*ip = nextItem;
				break;
			}
		}
	}
}

<<<<<<< HEAD
ConfigLineEdit::ConfigLineEdit(ConfigView* parent)
	: Parent(parent)
{
	connect(this, SIGNAL(lostFocus()), SLOT(hide()));
}

void ConfigLineEdit::show(ConfigItem* i)
{
	item = i;
	if (sym_get_string_value(item->menu->sym))
		setText(QString::fromLocal8Bit(sym_get_string_value(item->menu->sym)));
	else
		setText(QString::null);
	Parent::show();
	setFocus();
}

void ConfigLineEdit::keyPressEvent(QKeyEvent* e)
{
	switch (e->key()) {
	case Qt::Key_Escape:
		break;
	case Qt::Key_Return:
	case Qt::Key_Enter:
		sym_set_string_value(item->menu->sym, text().latin1());
		parent()->updateList(item);
		break;
	default:
		Parent::keyPressEvent(e);
		return;
	}
	e->accept();
	parent()->list->setFocus();
	hide();
}

ConfigList::ConfigList(ConfigView* p, const char *name)
	: Parent(p, name),
	  updateAll(false),
	  symbolYesPix(xpm_symbol_yes), symbolModPix(xpm_symbol_mod), symbolNoPix(xpm_symbol_no),
	  choiceYesPix(xpm_choice_yes), choiceNoPix(xpm_choice_no),
	  menuPix(xpm_menu), menuInvPix(xpm_menu_inv), menuBackPix(xpm_menuback), voidPix(xpm_void),
	  showName(false), showRange(false), showData(false), optMode(normalOpt),
	  rootEntry(0), headerPopup(0)
{
	int i;

	setSorting(-1);
	setRootIsDecorated(TRUE);
	disabledColorGroup = palette().active();
	disabledColorGroup.setColor(QColorGroup::Text, palette().disabled().text());
	inactivedColorGroup = palette().active();
	inactivedColorGroup.setColor(QColorGroup::Highlight, palette().disabled().highlight());

	connect(this, SIGNAL(selectionChanged(void)),
		SLOT(updateSelection(void)));

	if (name) {
		configSettings->beginGroup(name);
		showName = configSettings->readBoolEntry("/showName", false);
		showRange = configSettings->readBoolEntry("/showRange", false);
		showData = configSettings->readBoolEntry("/showData", false);
		optMode = (enum optionMode)configSettings->readNumEntry("/optionMode", false);
		configSettings->endGroup();
		connect(configApp, SIGNAL(aboutToQuit()), SLOT(saveSettings()));
	}

	for (i = 0; i < colNr; i++)
		colMap[i] = colRevMap[i] = -1;
	addColumn(promptColIdx, _("Option"));
=======
QWidget *ConfigItemDelegate::createEditor(QWidget *parent,
					  const QStyleOptionViewItem &option,
					  const QModelIndex &index) const
{
	ConfigItem *item;

	// Only the data column is editable
	if (index.column() != dataColIdx)
		return nullptr;

	// You cannot edit invisible menus
	item = static_cast<ConfigItem *>(index.internalPointer());
	if (!item || !item->menu || !menu_is_visible(item->menu))
		return nullptr;

	return QStyledItemDelegate::createEditor(parent, option, index);
}

void ConfigItemDelegate::setModelData(QWidget *editor,
				      QAbstractItemModel *model,
				      const QModelIndex &index) const
{
	QLineEdit *lineEdit;
	ConfigItem *item;
	struct symbol *sym;
	bool success;

	lineEdit = qobject_cast<QLineEdit *>(editor);
	// If this is not a QLineEdit, use the parent's default.
	// (does this happen?)
	if (!lineEdit)
		goto parent;

	item = static_cast<ConfigItem *>(index.internalPointer());
	if (!item || !item->menu)
		goto parent;

	sym = item->menu->sym;
	if (!sym)
		goto parent;

	success = sym_set_string_value(sym, lineEdit->text().toUtf8().data());
	if (success) {
		ConfigList::updateListForAll();
	} else {
		QMessageBox::information(editor, "qconf",
			"Cannot set the data (maybe due to out of range).\n"
			"Setting the old value.");
		lineEdit->setText(sym_get_string_value(sym));
	}

parent:
	QStyledItemDelegate::setModelData(editor, model, index);
}

ConfigList::ConfigList(QWidget *parent, const char *name)
	: QTreeWidget(parent),
	  updateAll(false),
	  showName(false), mode(singleMode), optMode(normalOpt),
	  rootEntry(0), headerPopup(0)
{
	setObjectName(name);
	setSortingEnabled(false);
	setRootIsDecorated(true);

	setVerticalScrollMode(ScrollPerPixel);
	setHorizontalScrollMode(ScrollPerPixel);

	setHeaderLabels(QStringList() << "Option" << "Name" << "Value");

	connect(this, &ConfigList::itemSelectionChanged,
		this, &ConfigList::updateSelection);

	if (name) {
		configSettings->beginGroup(name);
		showName = configSettings->value("/showName", false).toBool();
		optMode = (enum optionMode)configSettings->value("/optionMode", 0).toInt();
		configSettings->endGroup();
		connect(configApp, &QApplication::aboutToQuit,
			this, &ConfigList::saveSettings);
	}

	showColumn(promptColIdx);

	setItemDelegate(new ConfigItemDelegate(this));

	allLists.append(this);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	reinit();
}

<<<<<<< HEAD
=======
ConfigList::~ConfigList()
{
	allLists.removeOne(this);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
bool ConfigList::menuSkip(struct menu *menu)
{
	if (optMode == normalOpt && menu_is_visible(menu))
		return false;
	if (optMode == promptOpt && menu_has_prompt(menu))
		return false;
	if (optMode == allOpt)
		return false;
	return true;
}

void ConfigList::reinit(void)
{
<<<<<<< HEAD
	removeColumn(dataColIdx);
	removeColumn(yesColIdx);
	removeColumn(modColIdx);
	removeColumn(noColIdx);
	removeColumn(nameColIdx);

	if (showName)
		addColumn(nameColIdx, _("Name"));
	if (showRange) {
		addColumn(noColIdx, "N");
		addColumn(modColIdx, "M");
		addColumn(yesColIdx, "Y");
	}
	if (showData)
		addColumn(dataColIdx, _("Value"));
=======
	hideColumn(nameColIdx);

	if (showName)
		showColumn(nameColIdx);

	updateListAll();
}

void ConfigList::setOptionMode(QAction *action)
{
	if (action == showNormalAction)
		optMode = normalOpt;
	else if (action == showAllAction)
		optMode = allOpt;
	else
		optMode = promptOpt;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	updateListAll();
}

void ConfigList::saveSettings(void)
{
<<<<<<< HEAD
	if (name()) {
		configSettings->beginGroup(name());
		configSettings->writeEntry("/showName", showName);
		configSettings->writeEntry("/showRange", showRange);
		configSettings->writeEntry("/showData", showData);
		configSettings->writeEntry("/optionMode", (int)optMode);
=======
	if (!objectName().isEmpty()) {
		configSettings->beginGroup(objectName());
		configSettings->setValue("/showName", showName);
		configSettings->setValue("/optionMode", (int)optMode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		configSettings->endGroup();
	}
}

ConfigItem* ConfigList::findConfigItem(struct menu *menu)
{
	ConfigItem* item = (ConfigItem*)menu->data;

	for (; item; item = item->nextItem) {
		if (this == item->listView())
			break;
	}

	return item;
}

void ConfigList::updateSelection(void)
{
	struct menu *menu;
	enum prop_type type;

<<<<<<< HEAD
	ConfigItem* item = (ConfigItem*)selectedItem();
=======
	if (selectedItems().count() == 0)
		return;

	ConfigItem* item = (ConfigItem*)selectedItems().first();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!item)
		return;

	menu = item->menu;
	emit menuChanged(menu);
	if (!menu)
		return;
	type = menu->prompt ? menu->prompt->type : P_UNKNOWN;
	if (mode == menuMode && type == P_MENU)
		emit menuSelected(menu);
}

<<<<<<< HEAD
void ConfigList::updateList(ConfigItem* item)
{
	ConfigItem* last = 0;
=======
void ConfigList::updateList()
{
	ConfigItem* last = 0;
	ConfigItem *item;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!rootEntry) {
		if (mode != listMode)
			goto update;
<<<<<<< HEAD
		Q3ListViewItemIterator it(this);
		ConfigItem* item;

		for (; it.current(); ++it) {
			item = (ConfigItem*)it.current();
			if (!item->menu)
				continue;
			item->testUpdateMenu(menu_is_visible(item->menu));
=======
		QTreeWidgetItemIterator it(this);

		while (*it) {
			item = (ConfigItem*)(*it);
			if (!item->menu)
				continue;
			item->testUpdateMenu(menu_is_visible(item->menu));

			++it;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
		return;
	}

	if (rootEntry != &rootmenu && (mode == singleMode ||
	    (mode == symbolMode && rootEntry->parent != &rootmenu))) {
<<<<<<< HEAD
		item = firstChild();
=======
		item = (ConfigItem *)topLevelItem(0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!item)
			item = new ConfigItem(this, 0, true);
		last = item;
	}
	if ((mode == singleMode || (mode == symbolMode && !(rootEntry->flags & MENU_ROOT))) &&
	    rootEntry->sym && rootEntry->prompt) {
<<<<<<< HEAD
		item = last ? last->nextSibling() : firstChild();
=======
		item = last ? last->nextSibling() : nullptr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!item)
			item = new ConfigItem(this, last, rootEntry, true);
		else
			item->testUpdateMenu(true);

		updateMenuList(item, rootEntry);
<<<<<<< HEAD
		triggerUpdate();
		return;
	}
update:
	updateMenuList(this, rootEntry);
	triggerUpdate();
=======
		update();
		resizeColumnToContents(0);
		return;
	}
update:
	updateMenuList(rootEntry);
	update();
	resizeColumnToContents(0);
}

void ConfigList::updateListForAll()
{
	QListIterator<ConfigList *> it(allLists);

	while (it.hasNext()) {
		ConfigList *list = it.next();

		list->updateList();
	}
}

void ConfigList::updateListAllForAll()
{
	QListIterator<ConfigList *> it(allLists);

	while (it.hasNext()) {
		ConfigList *list = it.next();

		list->updateList();
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void ConfigList::setValue(ConfigItem* item, tristate val)
{
	struct symbol* sym;
	int type;
	tristate oldval;

	sym = item->menu ? item->menu->sym : 0;
	if (!sym)
		return;

	type = sym_get_type(sym);
	switch (type) {
	case S_BOOLEAN:
	case S_TRISTATE:
		oldval = sym_get_tristate_value(sym);

		if (!sym_set_tristate_value(sym, val))
			return;
		if (oldval == no && item->menu->list)
<<<<<<< HEAD
			item->setOpen(TRUE);
		parent()->updateList(item);
=======
			item->setExpanded(true);
		ConfigList::updateListForAll();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	}
}

void ConfigList::changeValue(ConfigItem* item)
{
	struct symbol* sym;
	struct menu* menu;
	int type, oldexpr, newexpr;

	menu = item->menu;
	if (!menu)
		return;
	sym = menu->sym;
	if (!sym) {
		if (item->menu->list)
<<<<<<< HEAD
			item->setOpen(!item->isOpen());
=======
			item->setExpanded(!item->isExpanded());
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;
	}

	type = sym_get_type(sym);
	switch (type) {
	case S_BOOLEAN:
	case S_TRISTATE:
		oldexpr = sym_get_tristate_value(sym);
		newexpr = sym_toggle_tristate_value(sym);
		if (item->menu->list) {
			if (oldexpr == newexpr)
<<<<<<< HEAD
				item->setOpen(!item->isOpen());
			else if (oldexpr == no)
				item->setOpen(TRUE);
		}
		if (oldexpr != newexpr)
			parent()->updateList(item);
		break;
	case S_INT:
	case S_HEX:
	case S_STRING:
		if (colMap[dataColIdx] >= 0)
			item->startRename(colMap[dataColIdx]);
		else
			parent()->lineEdit->show(item);
=======
				item->setExpanded(!item->isExpanded());
			else if (oldexpr == no)
				item->setExpanded(true);
		}
		if (oldexpr != newexpr)
			ConfigList::updateListForAll();
		break;
	default:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	}
}

void ConfigList::setRootMenu(struct menu *menu)
{
	enum prop_type type;

	if (rootEntry == menu)
		return;
	type = menu && menu->prompt ? menu->prompt->type : P_UNKNOWN;
	if (type != P_MENU)
		return;
<<<<<<< HEAD
	updateMenuList(this, 0);
	rootEntry = menu;
	updateListAll();
	setSelected(currentItem(), hasFocus());
	ensureItemVisible(currentItem());
=======
	updateMenuList(0);
	rootEntry = menu;
	updateListAll();
	if (currentItem()) {
		setSelected(currentItem(), hasFocus());
		scrollToItem(currentItem());
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void ConfigList::setParentMenu(void)
{
	ConfigItem* item;
	struct menu *oldroot;

	oldroot = rootEntry;
	if (rootEntry == &rootmenu)
		return;
	setRootMenu(menu_get_parent_menu(rootEntry->parent));

<<<<<<< HEAD
	Q3ListViewItemIterator it(this);
	for (; (item = (ConfigItem*)it.current()); it++) {
		if (item->menu == oldroot) {
			setCurrentItem(item);
			ensureItemVisible(item);
			break;
		}
=======
	QTreeWidgetItemIterator it(this);
	while (*it) {
		item = (ConfigItem *)(*it);
		if (item->menu == oldroot) {
			setCurrentItem(item);
			scrollToItem(item);
			break;
		}

		++it;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

/*
 * update all the children of a menu entry
 *   removes/adds the entries from the parent widget as necessary
 *
 * parent: either the menu list widget or a menu entry widget
 * menu: entry to be updated
 */
<<<<<<< HEAD
template <class P>
void ConfigList::updateMenuList(P* parent, struct menu* menu)
=======
void ConfigList::updateMenuList(ConfigItem *parent, struct menu* menu)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct menu* child;
	ConfigItem* item;
	ConfigItem* last;
	bool visible;
	enum prop_type type;

	if (!menu) {
<<<<<<< HEAD
		while ((item = parent->firstChild()))
			delete item;
=======
		while (parent->childCount() > 0)
		{
			delete parent->takeChild(0);
		}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;
	}

	last = parent->firstChild();
	if (last && !last->goParent)
		last = 0;
	for (child = menu->list; child; child = child->next) {
		item = last ? last->nextSibling() : parent->firstChild();
		type = child->prompt ? child->prompt->type : P_UNKNOWN;

		switch (mode) {
		case menuMode:
			if (!(child->flags & MENU_ROOT))
				goto hide;
			break;
		case symbolMode:
			if (child->flags & MENU_ROOT)
				goto hide;
			break;
		default:
			break;
		}

		visible = menu_is_visible(child);
		if (!menuSkip(child)) {
			if (!child->sym && !child->list && !child->prompt)
				continue;
			if (!item || item->menu != child)
				item = new ConfigItem(parent, last, child, visible);
			else
				item->testUpdateMenu(visible);

			if (mode == fullMode || mode == menuMode || type != P_MENU)
				updateMenuList(item, child);
			else
				updateMenuList(item, 0);
			last = item;
			continue;
		}
<<<<<<< HEAD
	hide:
=======
hide:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (item && item->menu == child) {
			last = parent->firstChild();
			if (last == item)
				last = 0;
			else while (last->nextSibling() != item)
				last = last->nextSibling();
			delete item;
		}
	}
}

<<<<<<< HEAD
void ConfigList::keyPressEvent(QKeyEvent* ev)
{
	Q3ListViewItem* i = currentItem();
=======
void ConfigList::updateMenuList(struct menu *menu)
{
	struct menu* child;
	ConfigItem* item;
	ConfigItem* last;
	bool visible;
	enum prop_type type;

	if (!menu) {
		while (topLevelItemCount() > 0)
		{
			delete takeTopLevelItem(0);
		}

		return;
	}

	last = (ConfigItem *)topLevelItem(0);
	if (last && !last->goParent)
		last = 0;
	for (child = menu->list; child; child = child->next) {
		item = last ? last->nextSibling() : (ConfigItem *)topLevelItem(0);
		type = child->prompt ? child->prompt->type : P_UNKNOWN;

		switch (mode) {
		case menuMode:
			if (!(child->flags & MENU_ROOT))
				goto hide;
			break;
		case symbolMode:
			if (child->flags & MENU_ROOT)
				goto hide;
			break;
		default:
			break;
		}

		visible = menu_is_visible(child);
		if (!menuSkip(child)) {
			if (!child->sym && !child->list && !child->prompt)
				continue;
			if (!item || item->menu != child)
				item = new ConfigItem(this, last, child, visible);
			else
				item->testUpdateMenu(visible);

			if (mode == fullMode || mode == menuMode || type != P_MENU)
				updateMenuList(item, child);
			else
				updateMenuList(item, 0);
			last = item;
			continue;
		}
hide:
		if (item && item->menu == child) {
			last = (ConfigItem *)topLevelItem(0);
			if (last == item)
				last = 0;
			else while (last->nextSibling() != item)
				last = last->nextSibling();
			delete item;
		}
	}
}

void ConfigList::keyPressEvent(QKeyEvent* ev)
{
	QTreeWidgetItem* i = currentItem();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ConfigItem* item;
	struct menu *menu;
	enum prop_type type;

	if (ev->key() == Qt::Key_Escape && mode != fullMode && mode != listMode) {
		emit parentSelected();
		ev->accept();
		return;
	}

	if (!i) {
		Parent::keyPressEvent(ev);
		return;
	}
	item = (ConfigItem*)i;

	switch (ev->key()) {
	case Qt::Key_Return:
	case Qt::Key_Enter:
		if (item->goParent) {
			emit parentSelected();
			break;
		}
		menu = item->menu;
		if (!menu)
			break;
		type = menu->prompt ? menu->prompt->type : P_UNKNOWN;
		if (type == P_MENU && rootEntry != menu &&
		    mode != fullMode && mode != menuMode) {
<<<<<<< HEAD
			emit menuSelected(menu);
=======
			if (mode == menuMode)
				emit menuSelected(menu);
			else
				emit itemSelected(menu);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		}
	case Qt::Key_Space:
		changeValue(item);
		break;
	case Qt::Key_N:
		setValue(item, no);
		break;
	case Qt::Key_M:
		setValue(item, mod);
		break;
	case Qt::Key_Y:
		setValue(item, yes);
		break;
	default:
		Parent::keyPressEvent(ev);
		return;
	}
	ev->accept();
}

<<<<<<< HEAD
void ConfigList::contentsMousePressEvent(QMouseEvent* e)
{
	//QPoint p(contentsToViewport(e->pos()));
	//printf("contentsMousePressEvent: %d,%d\n", p.x(), p.y());
	Parent::contentsMousePressEvent(e);
}

void ConfigList::contentsMouseReleaseEvent(QMouseEvent* e)
{
	QPoint p(contentsToViewport(e->pos()));
	ConfigItem* item = (ConfigItem*)itemAt(p);
	struct menu *menu;
	enum prop_type ptype;
	const QPixmap* pm;
=======
void ConfigList::mousePressEvent(QMouseEvent* e)
{
	//QPoint p(contentsToViewport(e->pos()));
	//printf("contentsMousePressEvent: %d,%d\n", p.x(), p.y());
	Parent::mousePressEvent(e);
}

void ConfigList::mouseReleaseEvent(QMouseEvent* e)
{
	QPoint p = e->pos();
	ConfigItem* item = (ConfigItem*)itemAt(p);
	struct menu *menu;
	enum prop_type ptype;
	QIcon icon;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int idx, x;

	if (!item)
		goto skip;

	menu = item->menu;
	x = header()->offset() + p.x();
<<<<<<< HEAD
	idx = colRevMap[header()->sectionAt(x)];
	switch (idx) {
	case promptColIdx:
		pm = item->pixmap(promptColIdx);
		if (pm) {
			int off = header()->sectionPos(0) + itemMargin() +
				treeStepSize() * (item->depth() + (rootIsDecorated() ? 1 : 0));
			if (x >= off && x < off + pm->width()) {
=======
	idx = header()->logicalIndexAt(x);
	switch (idx) {
	case promptColIdx:
		icon = item->icon(promptColIdx);
		if (!icon.isNull()) {
			int off = header()->sectionPosition(0) + visualRect(indexAt(p)).x() + 4; // 4 is Hardcoded image offset. There might be a way to do it properly.
			if (x >= off && x < off + icon.availableSizes().first().width()) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				if (item->goParent) {
					emit parentSelected();
					break;
				} else if (!menu)
					break;
				ptype = menu->prompt ? menu->prompt->type : P_UNKNOWN;
				if (ptype == P_MENU && rootEntry != menu &&
<<<<<<< HEAD
				    mode != fullMode && mode != menuMode)
=======
				    mode != fullMode && mode != menuMode &&
                                    mode != listMode)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					emit menuSelected(menu);
				else
					changeValue(item);
			}
		}
		break;
<<<<<<< HEAD
	case noColIdx:
		setValue(item, no);
		break;
	case modColIdx:
		setValue(item, mod);
		break;
	case yesColIdx:
		setValue(item, yes);
		break;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case dataColIdx:
		changeValue(item);
		break;
	}

skip:
	//printf("contentsMouseReleaseEvent: %d,%d\n", p.x(), p.y());
<<<<<<< HEAD
	Parent::contentsMouseReleaseEvent(e);
}

void ConfigList::contentsMouseMoveEvent(QMouseEvent* e)
{
	//QPoint p(contentsToViewport(e->pos()));
	//printf("contentsMouseMoveEvent: %d,%d\n", p.x(), p.y());
	Parent::contentsMouseMoveEvent(e);
}

void ConfigList::contentsMouseDoubleClickEvent(QMouseEvent* e)
{
	QPoint p(contentsToViewport(e->pos()));
=======
	Parent::mouseReleaseEvent(e);
}

void ConfigList::mouseMoveEvent(QMouseEvent* e)
{
	//QPoint p(contentsToViewport(e->pos()));
	//printf("contentsMouseMoveEvent: %d,%d\n", p.x(), p.y());
	Parent::mouseMoveEvent(e);
}

void ConfigList::mouseDoubleClickEvent(QMouseEvent* e)
{
	QPoint p = e->pos();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ConfigItem* item = (ConfigItem*)itemAt(p);
	struct menu *menu;
	enum prop_type ptype;

	if (!item)
		goto skip;
	if (item->goParent) {
		emit parentSelected();
		goto skip;
	}
	menu = item->menu;
	if (!menu)
		goto skip;
	ptype = menu->prompt ? menu->prompt->type : P_UNKNOWN;
<<<<<<< HEAD
	if (ptype == P_MENU && (mode == singleMode || mode == symbolMode))
		emit menuSelected(menu);
	else if (menu->sym)
=======
	if (ptype == P_MENU && mode != listMode) {
		if (mode == singleMode)
			emit itemSelected(menu);
		else if (mode == symbolMode)
			emit menuSelected(menu);
	} else if (menu->sym)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		changeValue(item);

skip:
	//printf("contentsMouseDoubleClickEvent: %d,%d\n", p.x(), p.y());
<<<<<<< HEAD
	Parent::contentsMouseDoubleClickEvent(e);
=======
	Parent::mouseDoubleClickEvent(e);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void ConfigList::focusInEvent(QFocusEvent *e)
{
	struct menu *menu = NULL;

	Parent::focusInEvent(e);

	ConfigItem* item = (ConfigItem *)currentItem();
	if (item) {
<<<<<<< HEAD
		setSelected(item, TRUE);
=======
		setSelected(item, true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		menu = item->menu;
	}
	emit gotFocus(menu);
}

void ConfigList::contextMenuEvent(QContextMenuEvent *e)
{
<<<<<<< HEAD
	if (e->y() <= header()->geometry().bottom()) {
		if (!headerPopup) {
			Q3Action *action;

			headerPopup = new Q3PopupMenu(this);
			action = new Q3Action(NULL, _("Show Name"), 0, this);
			  action->setToggleAction(TRUE);
			  connect(action, SIGNAL(toggled(bool)),
				  parent(), SLOT(setShowName(bool)));
			  connect(parent(), SIGNAL(showNameChanged(bool)),
				  action, SLOT(setOn(bool)));
			  action->setOn(showName);
			  action->addTo(headerPopup);
			action = new Q3Action(NULL, _("Show Range"), 0, this);
			  action->setToggleAction(TRUE);
			  connect(action, SIGNAL(toggled(bool)),
				  parent(), SLOT(setShowRange(bool)));
			  connect(parent(), SIGNAL(showRangeChanged(bool)),
				  action, SLOT(setOn(bool)));
			  action->setOn(showRange);
			  action->addTo(headerPopup);
			action = new Q3Action(NULL, _("Show Data"), 0, this);
			  action->setToggleAction(TRUE);
			  connect(action, SIGNAL(toggled(bool)),
				  parent(), SLOT(setShowData(bool)));
			  connect(parent(), SIGNAL(showDataChanged(bool)),
				  action, SLOT(setOn(bool)));
			  action->setOn(showData);
			  action->addTo(headerPopup);
		}
		headerPopup->exec(e->globalPos());
		e->accept();
	} else
		e->ignore();
}

ConfigView*ConfigView::viewList;
QAction *ConfigView::showNormalAction;
QAction *ConfigView::showAllAction;
QAction *ConfigView::showPromptAction;

ConfigView::ConfigView(QWidget* parent, const char *name)
	: Parent(parent, name)
{
	list = new ConfigList(this, name);
	lineEdit = new ConfigLineEdit(this);
	lineEdit->hide();

	this->nextView = viewList;
	viewList = this;
}

ConfigView::~ConfigView(void)
{
	ConfigView** vp;

	for (vp = &viewList; *vp; vp = &(*vp)->nextView) {
		if (*vp == this) {
			*vp = nextView;
			break;
		}
	}
}

void ConfigView::setOptionMode(QAction *act)
{
	if (act == showNormalAction)
		list->optMode = normalOpt;
	else if (act == showAllAction)
		list->optMode = allOpt;
	else
		list->optMode = promptOpt;

	list->updateListAll();
}

void ConfigView::setShowName(bool b)
{
	if (list->showName != b) {
		list->showName = b;
		list->reinit();
		emit showNameChanged(b);
	}
}

void ConfigView::setShowRange(bool b)
{
	if (list->showRange != b) {
		list->showRange = b;
		list->reinit();
		emit showRangeChanged(b);
	}
}

void ConfigView::setShowData(bool b)
{
	if (list->showData != b) {
		list->showData = b;
		list->reinit();
		emit showDataChanged(b);
	}
}

void ConfigList::setAllOpen(bool open)
{
	Q3ListViewItemIterator it(this);

	for (; it.current(); it++)
		it.current()->setOpen(open);
}

void ConfigView::updateList(ConfigItem* item)
{
	ConfigView* v;

	for (v = viewList; v; v = v->nextView)
		v->list->updateList(item);
}

void ConfigView::updateListAll(void)
{
	ConfigView* v;

	for (v = viewList; v; v = v->nextView)
		v->list->updateListAll();
}

ConfigInfoView::ConfigInfoView(QWidget* parent, const char *name)
	: Parent(parent, name), sym(0), _menu(0)
{
	if (name) {
		configSettings->beginGroup(name);
		_showDebug = configSettings->readBoolEntry("/showDebug", false);
		configSettings->endGroup();
		connect(configApp, SIGNAL(aboutToQuit()), SLOT(saveSettings()));
	}
=======
	if (!headerPopup) {
		QAction *action;

		headerPopup = new QMenu(this);
		action = new QAction("Show Name", this);
		action->setCheckable(true);
		connect(action, &QAction::toggled,
			this, &ConfigList::setShowName);
		connect(this, &ConfigList::showNameChanged,
			action, &QAction::setChecked);
		action->setChecked(showName);
		headerPopup->addAction(action);
	}

	headerPopup->exec(e->globalPos());
	e->accept();
}

void ConfigList::setShowName(bool on)
{
	if (showName == on)
		return;

	showName = on;
	reinit();
	emit showNameChanged(on);
}

QList<ConfigList *> ConfigList::allLists;
QAction *ConfigList::showNormalAction;
QAction *ConfigList::showAllAction;
QAction *ConfigList::showPromptAction;

void ConfigList::setAllOpen(bool open)
{
	QTreeWidgetItemIterator it(this);

	while (*it) {
		(*it)->setExpanded(open);

		++it;
	}
}

ConfigInfoView::ConfigInfoView(QWidget* parent, const char *name)
	: Parent(parent), sym(0), _menu(0)
{
	setObjectName(name);
	setOpenLinks(false);

	if (!objectName().isEmpty()) {
		configSettings->beginGroup(objectName());
		setShowDebug(configSettings->value("/showDebug", false).toBool());
		configSettings->endGroup();
		connect(configApp, &QApplication::aboutToQuit,
			this, &ConfigInfoView::saveSettings);
	}

	contextMenu = createStandardContextMenu();
	QAction *action = new QAction("Show Debug Info", contextMenu);

	action->setCheckable(true);
	connect(action, &QAction::toggled,
		this, &ConfigInfoView::setShowDebug);
	connect(this, &ConfigInfoView::showDebugChanged,
		action, &QAction::setChecked);
	action->setChecked(showDebug());
	contextMenu->addSeparator();
	contextMenu->addAction(action);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void ConfigInfoView::saveSettings(void)
{
<<<<<<< HEAD
	if (name()) {
		configSettings->beginGroup(name());
		configSettings->writeEntry("/showDebug", showDebug());
=======
	if (!objectName().isEmpty()) {
		configSettings->beginGroup(objectName());
		configSettings->setValue("/showDebug", showDebug());
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		configSettings->endGroup();
	}
}

void ConfigInfoView::setShowDebug(bool b)
{
	if (_showDebug != b) {
		_showDebug = b;
		if (_menu)
			menuInfo();
		else if (sym)
			symbolInfo();
		emit showDebugChanged(b);
	}
}

void ConfigInfoView::setInfo(struct menu *m)
{
	if (_menu == m)
		return;
	_menu = m;
	sym = NULL;
	if (!_menu)
		clear();
	else
		menuInfo();
}

void ConfigInfoView::symbolInfo(void)
{
	QString str;

	str += "<big>Symbol: <b>";
	str += print_filter(sym->name);
	str += "</b></big><br><br>value: ";
	str += print_filter(sym_get_string_value(sym));
	str += "<br>visibility: ";
	str += sym->visible == yes ? "y" : sym->visible == mod ? "m" : "n";
	str += "<br>";
	str += debug_info(sym);

	setText(str);
}

void ConfigInfoView::menuInfo(void)
{
	struct symbol* sym;
<<<<<<< HEAD
	QString head, debug, help;
=======
	QString info;
	QTextStream stream(&info);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	sym = _menu->sym;
	if (sym) {
		if (_menu->prompt) {
<<<<<<< HEAD
			head += "<big><b>";
			head += print_filter(_(_menu->prompt->text));
			head += "</b></big>";
			if (sym->name) {
				head += " (";
				if (showDebug())
					head += QString().sprintf("<a href=\"s%p\">", sym);
				head += print_filter(sym->name);
				if (showDebug())
					head += "</a>";
				head += ")";
			}
		} else if (sym->name) {
			head += "<big><b>";
			if (showDebug())
				head += QString().sprintf("<a href=\"s%p\">", sym);
			head += print_filter(sym->name);
			if (showDebug())
				head += "</a>";
			head += "</b></big>";
		}
		head += "<br><br>";

		if (showDebug())
			debug = debug_info(sym);

		struct gstr help_gstr = str_new();
		menu_get_ext_help(_menu, &help_gstr);
		help = print_filter(str_get(&help_gstr));
		str_free(&help_gstr);
	} else if (_menu->prompt) {
		head += "<big><b>";
		head += print_filter(_(_menu->prompt->text));
		head += "</b></big><br><br>";
		if (showDebug()) {
			if (_menu->prompt->visible.expr) {
				debug += "&nbsp;&nbsp;dep: ";
				expr_print(_menu->prompt->visible.expr, expr_print_help, &debug, E_NONE);
				debug += "<br><br>";
			}
		}
	}
	if (showDebug())
		debug += QString().sprintf("defined at %s:%d<br><br>", _menu->file->name, _menu->lineno);

	setText(head + debug + help);
=======
			stream << "<big><b>";
			stream << print_filter(_menu->prompt->text);
			stream << "</b></big>";
			if (sym->name) {
				stream << " (";
				if (showDebug())
					stream << "<a href=\"s" << sym->name << "\">";
				stream << print_filter(sym->name);
				if (showDebug())
					stream << "</a>";
				stream << ")";
			}
		} else if (sym->name) {
			stream << "<big><b>";
			if (showDebug())
				stream << "<a href=\"s" << sym->name << "\">";
			stream << print_filter(sym->name);
			if (showDebug())
				stream << "</a>";
			stream << "</b></big>";
		}
		stream << "<br><br>";

		if (showDebug())
			stream << debug_info(sym);

		struct gstr help_gstr = str_new();

		menu_get_ext_help(_menu, &help_gstr);
		stream << print_filter(str_get(&help_gstr));
		str_free(&help_gstr);
	} else if (_menu->prompt) {
		stream << "<big><b>";
		stream << print_filter(_menu->prompt->text);
		stream << "</b></big><br><br>";
		if (showDebug()) {
			if (_menu->prompt->visible.expr) {
				stream << "&nbsp;&nbsp;dep: ";
				expr_print(_menu->prompt->visible.expr,
					   expr_print_help, &stream, E_NONE);
				stream << "<br><br>";
			}

			stream << "defined at " << _menu->filename << ":"
			       << _menu->lineno << "<br><br>";
		}
	}

	setText(info);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

QString ConfigInfoView::debug_info(struct symbol *sym)
{
	QString debug;
<<<<<<< HEAD

	debug += "type: ";
	debug += print_filter(sym_type_name(sym->type));
	if (sym_is_choice(sym))
		debug += " (choice)";
	debug += "<br>";
	if (sym->rev_dep.expr) {
		debug += "reverse dep: ";
		expr_print(sym->rev_dep.expr, expr_print_help, &debug, E_NONE);
		debug += "<br>";
=======
	QTextStream stream(&debug);

	stream << "type: ";
	stream << print_filter(sym_type_name(sym->type));
	if (sym_is_choice(sym))
		stream << " (choice)";
	debug += "<br>";
	if (sym->rev_dep.expr) {
		stream << "reverse dep: ";
		expr_print(sym->rev_dep.expr, expr_print_help, &stream, E_NONE);
		stream << "<br>";
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	for (struct property *prop = sym->prop; prop; prop = prop->next) {
		switch (prop->type) {
		case P_PROMPT:
		case P_MENU:
<<<<<<< HEAD
			debug += QString().sprintf("prompt: <a href=\"m%p\">", prop->menu);
			debug += print_filter(_(prop->text));
			debug += "</a><br>";
=======
			stream << "prompt: <a href=\"m" << sym->name << "\">";
			stream << print_filter(prop->text);
			stream << "</a><br>";
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		case P_DEFAULT:
		case P_SELECT:
		case P_RANGE:
<<<<<<< HEAD
		case P_ENV:
			debug += prop_get_type_name(prop->type);
			debug += ": ";
			expr_print(prop->expr, expr_print_help, &debug, E_NONE);
			debug += "<br>";
			break;
		case P_CHOICE:
			if (sym_is_choice(sym)) {
				debug += "choice: ";
				expr_print(prop->expr, expr_print_help, &debug, E_NONE);
				debug += "<br>";
			}
			break;
		default:
			debug += "unknown property: ";
			debug += prop_get_type_name(prop->type);
			debug += "<br>";
		}
		if (prop->visible.expr) {
			debug += "&nbsp;&nbsp;&nbsp;&nbsp;dep: ";
			expr_print(prop->visible.expr, expr_print_help, &debug, E_NONE);
			debug += "<br>";
		}
	}
	debug += "<br>";
=======
		case P_COMMENT:
		case P_IMPLY:
		case P_SYMBOL:
			stream << prop_get_type_name(prop->type);
			stream << ": ";
			expr_print(prop->expr, expr_print_help,
				   &stream, E_NONE);
			stream << "<br>";
			break;
		case P_CHOICE:
			if (sym_is_choice(sym)) {
				stream << "choice: ";
				expr_print(prop->expr, expr_print_help,
					   &stream, E_NONE);
				stream << "<br>";
			}
			break;
		default:
			stream << "unknown property: ";
			stream << prop_get_type_name(prop->type);
			stream << "<br>";
		}
		if (prop->visible.expr) {
			stream << "&nbsp;&nbsp;&nbsp;&nbsp;dep: ";
			expr_print(prop->visible.expr, expr_print_help,
				   &stream, E_NONE);
			stream << "<br>";
		}
	}
	stream << "<br>";
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return debug;
}

QString ConfigInfoView::print_filter(const QString &str)
{
<<<<<<< HEAD
	QRegExp re("[<>&\"\\n]");
	QString res = str;
	for (int i = 0; (i = res.find(re, i)) >= 0;) {
		switch (res[i].latin1()) {
=======
	QRegularExpression re("[<>&\"\\n]");
	QString res = str;
	for (int i = 0; (i = res.indexOf(re, i)) >= 0;) {
		switch (res[i].toLatin1()) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		case '<':
			res.replace(i, 1, "&lt;");
			i += 4;
			break;
		case '>':
			res.replace(i, 1, "&gt;");
			i += 4;
			break;
		case '&':
			res.replace(i, 1, "&amp;");
			i += 5;
			break;
		case '"':
			res.replace(i, 1, "&quot;");
			i += 6;
			break;
		case '\n':
			res.replace(i, 1, "<br>");
			i += 4;
			break;
		}
	}
	return res;
}

void ConfigInfoView::expr_print_help(void *data, struct symbol *sym, const char *str)
{
<<<<<<< HEAD
	QString* text = reinterpret_cast<QString*>(data);
	QString str2 = print_filter(str);

	if (sym && sym->name && !(sym->flags & SYMBOL_CONST)) {
		*text += QString().sprintf("<a href=\"s%p\">", sym);
		*text += str2;
		*text += "</a>";
	} else
		*text += str2;
}

Q3PopupMenu* ConfigInfoView::createPopupMenu(const QPoint& pos)
{
	Q3PopupMenu* popup = Parent::createPopupMenu(pos);
	Q3Action* action = new Q3Action(NULL, _("Show Debug Info"), 0, popup);
	  action->setToggleAction(TRUE);
	  connect(action, SIGNAL(toggled(bool)), SLOT(setShowDebug(bool)));
	  connect(this, SIGNAL(showDebugChanged(bool)), action, SLOT(setOn(bool)));
	  action->setOn(showDebug());
	popup->insertSeparator();
	action->addTo(popup);
	return popup;
}

void ConfigInfoView::contentsContextMenuEvent(QContextMenuEvent *e)
{
	Parent::contentsContextMenuEvent(e);
}

ConfigSearchWindow::ConfigSearchWindow(ConfigMainWindow* parent, const char *name)
	: Parent(parent, name), result(NULL)
{
	setCaption("Search Config");

	QVBoxLayout* layout1 = new QVBoxLayout(this, 11, 6);
	QHBoxLayout* layout2 = new QHBoxLayout(0, 0, 6);
	layout2->addWidget(new QLabel(_("Find:"), this));
	editField = new QLineEdit(this);
	connect(editField, SIGNAL(returnPressed()), SLOT(search()));
	layout2->addWidget(editField);
	searchButton = new QPushButton(_("Search"), this);
	searchButton->setAutoDefault(FALSE);
	connect(searchButton, SIGNAL(clicked()), SLOT(search()));
=======
	QTextStream *stream = reinterpret_cast<QTextStream *>(data);

	if (sym && sym->name && !(sym->flags & SYMBOL_CONST)) {
		*stream << "<a href=\"s" << sym->name << "\">";
		*stream << print_filter(str);
		*stream << "</a>";
	} else {
		*stream << print_filter(str);
	}
}

void ConfigInfoView::clicked(const QUrl &url)
{
	QByteArray str = url.toEncoded();
	const std::size_t count = str.size();
	char *data = new char[count + 1];
	struct symbol **result;
	struct menu *m = NULL;

	if (count < 1) {
		delete[] data;
		return;
	}

	memcpy(data, str.constData(), count);
	data[count] = '\0';

	/* Seek for exact match */
	data[0] = '^';
	strcat(data, "$");
	result = sym_re_search(data);
	if (!result) {
		delete[] data;
		return;
	}

	sym = *result;

	/* Seek for the menu which holds the symbol */
	for (struct property *prop = sym->prop; prop; prop = prop->next) {
		    if (prop->type != P_PROMPT && prop->type != P_MENU)
			    continue;
		    m = prop->menu;
		    break;
	}

	if (!m) {
		/* Symbol is not visible as a menu */
		symbolInfo();
		emit showDebugChanged(true);
	} else {
		emit menuSelected(m);
	}

	free(result);
	delete[] data;
}

void ConfigInfoView::contextMenuEvent(QContextMenuEvent *event)
{
	contextMenu->popup(event->globalPos());
	event->accept();
}

ConfigSearchWindow::ConfigSearchWindow(ConfigMainWindow *parent)
	: Parent(parent), result(NULL)
{
	setObjectName("search");
	setWindowTitle("Search Config");

	QVBoxLayout* layout1 = new QVBoxLayout(this);
	layout1->setContentsMargins(11, 11, 11, 11);
	layout1->setSpacing(6);

	QHBoxLayout* layout2 = new QHBoxLayout();
	layout2->setContentsMargins(0, 0, 0, 0);
	layout2->setSpacing(6);
	layout2->addWidget(new QLabel("Find:", this));
	editField = new QLineEdit(this);
	connect(editField, &QLineEdit::returnPressed,
		this, &ConfigSearchWindow::search);
	layout2->addWidget(editField);
	searchButton = new QPushButton("Search", this);
	searchButton->setAutoDefault(false);
	connect(searchButton, &QPushButton::clicked,
		this, &ConfigSearchWindow::search);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	layout2->addWidget(searchButton);
	layout1->addLayout(layout2);

	split = new QSplitter(this);
	split->setOrientation(Qt::Vertical);
<<<<<<< HEAD
	list = new ConfigView(split, name);
	list->list->mode = listMode;
	info = new ConfigInfoView(split, name);
	connect(list->list, SIGNAL(menuChanged(struct menu *)),
		info, SLOT(setInfo(struct menu *)));
	connect(list->list, SIGNAL(menuChanged(struct menu *)),
		parent, SLOT(setMenuLink(struct menu *)));

	layout1->addWidget(split);

	if (name) {
		int x, y, width, height;
		bool ok;

		configSettings->beginGroup(name);
		width = configSettings->readNumEntry("/window width", parent->width() / 2);
		height = configSettings->readNumEntry("/window height", parent->height() / 2);
		resize(width, height);
		x = configSettings->readNumEntry("/window x", 0, &ok);
		if (ok)
			y = configSettings->readNumEntry("/window y", 0, &ok);
		if (ok)
			move(x, y);
		Q3ValueList<int> sizes = configSettings->readSizes("/split", &ok);
		if (ok)
			split->setSizes(sizes);
		configSettings->endGroup();
		connect(configApp, SIGNAL(aboutToQuit()), SLOT(saveSettings()));
	}
=======
	list = new ConfigList(split, "search");
	list->mode = listMode;
	info = new ConfigInfoView(split, "search");
	connect(list, &ConfigList::menuChanged,
		info, &ConfigInfoView::setInfo);
	connect(list, &ConfigList::menuChanged,
		parent, &ConfigMainWindow::setMenuLink);

	layout1->addWidget(split);

	QVariant x, y;
	int width, height;
	bool ok;

	configSettings->beginGroup("search");
	width = configSettings->value("/window width", parent->width() / 2).toInt();
	height = configSettings->value("/window height", parent->height() / 2).toInt();
	resize(width, height);
	x = configSettings->value("/window x");
	y = configSettings->value("/window y");
	if (x.isValid() && y.isValid())
		move(x.toInt(), y.toInt());
	QList<int> sizes = configSettings->readSizes("/split", &ok);
	if (ok)
		split->setSizes(sizes);
	configSettings->endGroup();
	connect(configApp, &QApplication::aboutToQuit,
		this, &ConfigSearchWindow::saveSettings);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void ConfigSearchWindow::saveSettings(void)
{
<<<<<<< HEAD
	if (name()) {
		configSettings->beginGroup(name());
		configSettings->writeEntry("/window x", pos().x());
		configSettings->writeEntry("/window y", pos().y());
		configSettings->writeEntry("/window width", size().width());
		configSettings->writeEntry("/window height", size().height());
=======
	if (!objectName().isEmpty()) {
		configSettings->beginGroup(objectName());
		configSettings->setValue("/window x", pos().x());
		configSettings->setValue("/window y", pos().y());
		configSettings->setValue("/window width", size().width());
		configSettings->setValue("/window height", size().height());
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		configSettings->writeSizes("/split", split->sizes());
		configSettings->endGroup();
	}
}

void ConfigSearchWindow::search(void)
{
	struct symbol **p;
	struct property *prop;
	ConfigItem *lastItem = NULL;

	free(result);
<<<<<<< HEAD
	list->list->clear();
	info->clear();

	result = sym_re_search(editField->text().latin1());
=======
	list->clear();
	info->clear();

	result = sym_re_search(editField->text().toLatin1());
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!result)
		return;
	for (p = result; *p; p++) {
		for_all_prompts((*p), prop)
<<<<<<< HEAD
			lastItem = new ConfigItem(list->list, lastItem, prop->menu,
=======
			lastItem = new ConfigItem(list, lastItem, prop->menu,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
						  menu_is_visible(prop->menu));
	}
}

/*
 * Construct the complete config widget
 */
ConfigMainWindow::ConfigMainWindow(void)
	: searchWindow(0)
{
<<<<<<< HEAD
	QMenuBar* menu;
	bool ok;
	int x, y, width, height;
	char title[256];

	QDesktopWidget *d = configApp->desktop();
	snprintf(title, sizeof(title), "%s%s",
		rootmenu.prompt->text,
#if QT_VERSION < 0x040000
		" (Qt3)"
#else
		""
#endif
		);
	setCaption(title);

	width = configSettings->readNumEntry("/window width", d->width() - 64);
	height = configSettings->readNumEntry("/window height", d->height() - 64);
	resize(width, height);
	x = configSettings->readNumEntry("/window x", 0, &ok);
	if (ok)
		y = configSettings->readNumEntry("/window y", 0, &ok);
	if (ok)
		move(x, y);

	split1 = new QSplitter(this);
	split1->setOrientation(Qt::Horizontal);
	setCentralWidget(split1);

	menuView = new ConfigView(split1, "menu");
	menuList = menuView->list;

	split2 = new QSplitter(split1);
	split2->setOrientation(Qt::Vertical);

	// create config tree
	configView = new ConfigView(split2, "config");
	configList = configView->list;

	helpText = new ConfigInfoView(split2, "help");
	helpText->setTextFormat(Qt::RichText);
=======
	bool ok = true;
	QVariant x, y;
	int width, height;
	char title[256];

	snprintf(title, sizeof(title), "%s%s",
		rootmenu.prompt->text,
		""
		);
	setWindowTitle(title);

	QRect g = configApp->primaryScreen()->geometry();
	width = configSettings->value("/window width", g.width() - 64).toInt();
	height = configSettings->value("/window height", g.height() - 64).toInt();
	resize(width, height);
	x = configSettings->value("/window x");
	y = configSettings->value("/window y");
	if ((x.isValid())&&(y.isValid()))
		move(x.toInt(), y.toInt());

	// set up icons
	ConfigItem::symbolYesIcon = QIcon(QPixmap(xpm_symbol_yes));
	ConfigItem::symbolModIcon = QIcon(QPixmap(xpm_symbol_mod));
	ConfigItem::symbolNoIcon = QIcon(QPixmap(xpm_symbol_no));
	ConfigItem::choiceYesIcon = QIcon(QPixmap(xpm_choice_yes));
	ConfigItem::choiceNoIcon = QIcon(QPixmap(xpm_choice_no));
	ConfigItem::menuIcon = QIcon(QPixmap(xpm_menu));
	ConfigItem::menubackIcon = QIcon(QPixmap(xpm_menuback));

	QWidget *widget = new QWidget(this);
	QVBoxLayout *layout = new QVBoxLayout(widget);
	setCentralWidget(widget);

	split1 = new QSplitter(widget);
	split1->setOrientation(Qt::Horizontal);
	split1->setChildrenCollapsible(false);

	menuList = new ConfigList(widget, "menu");

	split2 = new QSplitter(widget);
	split2->setChildrenCollapsible(false);
	split2->setOrientation(Qt::Vertical);

	// create config tree
	configList = new ConfigList(widget, "config");

	helpText = new ConfigInfoView(widget, "help");

	layout->addWidget(split2);
	split2->addWidget(split1);
	split1->addWidget(configList);
	split1->addWidget(menuList);
	split2->addWidget(helpText);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	setTabOrder(configList, helpText);
	configList->setFocus();

<<<<<<< HEAD
	menu = menuBar();
	toolBar = new Q3ToolBar("Tools", this);

	backAction = new Q3Action("Back", QPixmap(xpm_back), _("Back"), 0, this);
	  connect(backAction, SIGNAL(activated()), SLOT(goBack()));
	  backAction->setEnabled(FALSE);
	Q3Action *quitAction = new Q3Action("Quit", _("&Quit"), Qt::CTRL + Qt::Key_Q, this);
	  connect(quitAction, SIGNAL(activated()), SLOT(close()));
	Q3Action *loadAction = new Q3Action("Load", QPixmap(xpm_load), _("&Load"), Qt::CTRL + Qt::Key_L, this);
	  connect(loadAction, SIGNAL(activated()), SLOT(loadConfig()));
	saveAction = new Q3Action("Save", QPixmap(xpm_save), _("&Save"), Qt::CTRL + Qt::Key_S, this);
	  connect(saveAction, SIGNAL(activated()), SLOT(saveConfig()));
	conf_set_changed_callback(conf_changed);
	// Set saveAction's initial state
	conf_changed();
	Q3Action *saveAsAction = new Q3Action("Save As...", _("Save &As..."), 0, this);
	  connect(saveAsAction, SIGNAL(activated()), SLOT(saveConfigAs()));
	Q3Action *searchAction = new Q3Action("Find", _("&Find"), Qt::CTRL + Qt::Key_F, this);
	  connect(searchAction, SIGNAL(activated()), SLOT(searchConfig()));
	Q3Action *singleViewAction = new Q3Action("Single View", QPixmap(xpm_single_view), _("Single View"), 0, this);
	  connect(singleViewAction, SIGNAL(activated()), SLOT(showSingleView()));
	Q3Action *splitViewAction = new Q3Action("Split View", QPixmap(xpm_split_view), _("Split View"), 0, this);
	  connect(splitViewAction, SIGNAL(activated()), SLOT(showSplitView()));
	Q3Action *fullViewAction = new Q3Action("Full View", QPixmap(xpm_tree_view), _("Full View"), 0, this);
	  connect(fullViewAction, SIGNAL(activated()), SLOT(showFullView()));

	Q3Action *showNameAction = new Q3Action(NULL, _("Show Name"), 0, this);
	  showNameAction->setToggleAction(TRUE);
	  connect(showNameAction, SIGNAL(toggled(bool)), configView, SLOT(setShowName(bool)));
	  connect(configView, SIGNAL(showNameChanged(bool)), showNameAction, SLOT(setOn(bool)));
	  showNameAction->setOn(configView->showName());
	Q3Action *showRangeAction = new Q3Action(NULL, _("Show Range"), 0, this);
	  showRangeAction->setToggleAction(TRUE);
	  connect(showRangeAction, SIGNAL(toggled(bool)), configView, SLOT(setShowRange(bool)));
	  connect(configView, SIGNAL(showRangeChanged(bool)), showRangeAction, SLOT(setOn(bool)));
	  showRangeAction->setOn(configList->showRange);
	Q3Action *showDataAction = new Q3Action(NULL, _("Show Data"), 0, this);
	  showDataAction->setToggleAction(TRUE);
	  connect(showDataAction, SIGNAL(toggled(bool)), configView, SLOT(setShowData(bool)));
	  connect(configView, SIGNAL(showDataChanged(bool)), showDataAction, SLOT(setOn(bool)));
	  showDataAction->setOn(configList->showData);

	QActionGroup *optGroup = new QActionGroup(this);
	optGroup->setExclusive(TRUE);
	connect(optGroup, SIGNAL(selected(QAction *)), configView,
		SLOT(setOptionMode(QAction *)));
	connect(optGroup, SIGNAL(selected(QAction *)), menuView,
		SLOT(setOptionMode(QAction *)));

#if QT_VERSION >= 0x040000
	configView->showNormalAction = new QAction(_("Show Normal Options"), optGroup);
	configView->showAllAction = new QAction(_("Show All Options"), optGroup);
	configView->showPromptAction = new QAction(_("Show Prompt Options"), optGroup);
#else
	configView->showNormalAction = new QAction(_("Show Normal Options"), 0, optGroup);
	configView->showAllAction = new QAction(_("Show All Options"), 0, optGroup);
	configView->showPromptAction = new QAction(_("Show Prompt Options"), 0, optGroup);
#endif
	configView->showNormalAction->setToggleAction(TRUE);
	configView->showNormalAction->setOn(configList->optMode == normalOpt);
	configView->showAllAction->setToggleAction(TRUE);
	configView->showAllAction->setOn(configList->optMode == allOpt);
	configView->showPromptAction->setToggleAction(TRUE);
	configView->showPromptAction->setOn(configList->optMode == promptOpt);

	Q3Action *showDebugAction = new Q3Action(NULL, _("Show Debug Info"), 0, this);
	  showDebugAction->setToggleAction(TRUE);
	  connect(showDebugAction, SIGNAL(toggled(bool)), helpText, SLOT(setShowDebug(bool)));
	  connect(helpText, SIGNAL(showDebugChanged(bool)), showDebugAction, SLOT(setOn(bool)));
	  showDebugAction->setOn(helpText->showDebug());

	Q3Action *showIntroAction = new Q3Action(NULL, _("Introduction"), 0, this);
	  connect(showIntroAction, SIGNAL(activated()), SLOT(showIntro()));
	Q3Action *showAboutAction = new Q3Action(NULL, _("About"), 0, this);
	  connect(showAboutAction, SIGNAL(activated()), SLOT(showAbout()));

	// init tool bar
	backAction->addTo(toolBar);
	toolBar->addSeparator();
	loadAction->addTo(toolBar);
	saveAction->addTo(toolBar);
	toolBar->addSeparator();
	singleViewAction->addTo(toolBar);
	splitViewAction->addTo(toolBar);
	fullViewAction->addTo(toolBar);

	// create config menu
	Q3PopupMenu* config = new Q3PopupMenu(this);
	menu->insertItem(_("&File"), config);
	loadAction->addTo(config);
	saveAction->addTo(config);
	saveAsAction->addTo(config);
	config->insertSeparator();
	quitAction->addTo(config);

	// create edit menu
	Q3PopupMenu* editMenu = new Q3PopupMenu(this);
	menu->insertItem(_("&Edit"), editMenu);
	searchAction->addTo(editMenu);

	// create options menu
	Q3PopupMenu* optionMenu = new Q3PopupMenu(this);
	menu->insertItem(_("&Option"), optionMenu);
	showNameAction->addTo(optionMenu);
	showRangeAction->addTo(optionMenu);
	showDataAction->addTo(optionMenu);
	optionMenu->insertSeparator();
	optGroup->addTo(optionMenu);
	optionMenu->insertSeparator();

	// create help menu
	Q3PopupMenu* helpMenu = new Q3PopupMenu(this);
	menu->insertSeparator();
	menu->insertItem(_("&Help"), helpMenu);
	showIntroAction->addTo(helpMenu);
	showAboutAction->addTo(helpMenu);

	connect(configList, SIGNAL(menuChanged(struct menu *)),
		helpText, SLOT(setInfo(struct menu *)));
	connect(configList, SIGNAL(menuSelected(struct menu *)),
		SLOT(changeMenu(struct menu *)));
	connect(configList, SIGNAL(parentSelected()),
		SLOT(goBack()));
	connect(menuList, SIGNAL(menuChanged(struct menu *)),
		helpText, SLOT(setInfo(struct menu *)));
	connect(menuList, SIGNAL(menuSelected(struct menu *)),
		SLOT(changeMenu(struct menu *)));

	connect(configList, SIGNAL(gotFocus(struct menu *)),
		helpText, SLOT(setInfo(struct menu *)));
	connect(menuList, SIGNAL(gotFocus(struct menu *)),
		helpText, SLOT(setInfo(struct menu *)));
	connect(menuList, SIGNAL(gotFocus(struct menu *)),
		SLOT(listFocusChanged(void)));
	connect(helpText, SIGNAL(menuSelected(struct menu *)),
		SLOT(setMenuLink(struct menu *)));

	QString listMode = configSettings->readEntry("/listMode", "symbol");
=======
	backAction = new QAction(QPixmap(xpm_back), "Back", this);
	connect(backAction, &QAction::triggered,
		this, &ConfigMainWindow::goBack);

	QAction *quitAction = new QAction("&Quit", this);
	quitAction->setShortcut(Qt::CTRL | Qt::Key_Q);
	connect(quitAction, &QAction::triggered,
		this, &ConfigMainWindow::close);

	QAction *loadAction = new QAction(QPixmap(xpm_load), "&Load", this);
	loadAction->setShortcut(Qt::CTRL | Qt::Key_L);
	connect(loadAction, &QAction::triggered,
		this, &ConfigMainWindow::loadConfig);

	saveAction = new QAction(QPixmap(xpm_save), "&Save", this);
	saveAction->setShortcut(Qt::CTRL | Qt::Key_S);
	connect(saveAction, &QAction::triggered,
		this, &ConfigMainWindow::saveConfig);

	conf_set_changed_callback(conf_changed);

	// Set saveAction's initial state
	conf_changed();
	configname = xstrdup(conf_get_configname());

	QAction *saveAsAction = new QAction("Save &As...", this);
	connect(saveAsAction, &QAction::triggered,
		this, &ConfigMainWindow::saveConfigAs);
	QAction *searchAction = new QAction("&Find", this);
	searchAction->setShortcut(Qt::CTRL | Qt::Key_F);
	connect(searchAction, &QAction::triggered,
		this, &ConfigMainWindow::searchConfig);
	singleViewAction = new QAction(QPixmap(xpm_single_view), "Single View", this);
	singleViewAction->setCheckable(true);
	connect(singleViewAction, &QAction::triggered,
		this, &ConfigMainWindow::showSingleView);
	splitViewAction = new QAction(QPixmap(xpm_split_view), "Split View", this);
	splitViewAction->setCheckable(true);
	connect(splitViewAction, &QAction::triggered,
		this, &ConfigMainWindow::showSplitView);
	fullViewAction = new QAction(QPixmap(xpm_tree_view), "Full View", this);
	fullViewAction->setCheckable(true);
	connect(fullViewAction, &QAction::triggered,
		this, &ConfigMainWindow::showFullView);

	QAction *showNameAction = new QAction("Show Name", this);
	  showNameAction->setCheckable(true);
	connect(showNameAction, &QAction::toggled,
		configList, &ConfigList::setShowName);
	showNameAction->setChecked(configList->showName);

	QActionGroup *optGroup = new QActionGroup(this);
	optGroup->setExclusive(true);
	connect(optGroup, &QActionGroup::triggered,
		configList, &ConfigList::setOptionMode);
	connect(optGroup, &QActionGroup::triggered,
		menuList, &ConfigList::setOptionMode);

	ConfigList::showNormalAction = new QAction("Show Normal Options", optGroup);
	ConfigList::showNormalAction->setCheckable(true);
	ConfigList::showAllAction = new QAction("Show All Options", optGroup);
	ConfigList::showAllAction->setCheckable(true);
	ConfigList::showPromptAction = new QAction("Show Prompt Options", optGroup);
	ConfigList::showPromptAction->setCheckable(true);

	QAction *showDebugAction = new QAction("Show Debug Info", this);
	  showDebugAction->setCheckable(true);
	connect(showDebugAction, &QAction::toggled,
		helpText, &ConfigInfoView::setShowDebug);
	  showDebugAction->setChecked(helpText->showDebug());

	QAction *showIntroAction = new QAction("Introduction", this);
	connect(showIntroAction, &QAction::triggered,
		this, &ConfigMainWindow::showIntro);
	QAction *showAboutAction = new QAction("About", this);
	connect(showAboutAction, &QAction::triggered,
		this, &ConfigMainWindow::showAbout);

	// init tool bar
	QToolBar *toolBar = addToolBar("Tools");
	toolBar->addAction(backAction);
	toolBar->addSeparator();
	toolBar->addAction(loadAction);
	toolBar->addAction(saveAction);
	toolBar->addSeparator();
	toolBar->addAction(singleViewAction);
	toolBar->addAction(splitViewAction);
	toolBar->addAction(fullViewAction);

	// create file menu
	QMenu *menu = menuBar()->addMenu("&File");
	menu->addAction(loadAction);
	menu->addAction(saveAction);
	menu->addAction(saveAsAction);
	menu->addSeparator();
	menu->addAction(quitAction);

	// create edit menu
	menu = menuBar()->addMenu("&Edit");
	menu->addAction(searchAction);

	// create options menu
	menu = menuBar()->addMenu("&Option");
	menu->addAction(showNameAction);
	menu->addSeparator();
	menu->addActions(optGroup->actions());
	menu->addSeparator();
	menu->addAction(showDebugAction);

	// create help menu
	menu = menuBar()->addMenu("&Help");
	menu->addAction(showIntroAction);
	menu->addAction(showAboutAction);

	connect(helpText, &ConfigInfoView::anchorClicked,
		helpText, &ConfigInfoView::clicked);

	connect(configList, &ConfigList::menuChanged,
		helpText, &ConfigInfoView::setInfo);
	connect(configList, &ConfigList::menuSelected,
		this, &ConfigMainWindow::changeMenu);
	connect(configList, &ConfigList::itemSelected,
		this, &ConfigMainWindow::changeItens);
	connect(configList, &ConfigList::parentSelected,
		this, &ConfigMainWindow::goBack);
	connect(menuList, &ConfigList::menuChanged,
		helpText, &ConfigInfoView::setInfo);
	connect(menuList, &ConfigList::menuSelected,
		this, &ConfigMainWindow::changeMenu);

	connect(configList, &ConfigList::gotFocus,
		helpText, &ConfigInfoView::setInfo);
	connect(menuList, &ConfigList::gotFocus,
		helpText, &ConfigInfoView::setInfo);
	connect(menuList, &ConfigList::gotFocus,
		this, &ConfigMainWindow::listFocusChanged);
	connect(helpText, &ConfigInfoView::menuSelected,
		this, &ConfigMainWindow::setMenuLink);

	QString listMode = configSettings->value("/listMode", "symbol").toString();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (listMode == "single")
		showSingleView();
	else if (listMode == "full")
		showFullView();
	else /*if (listMode == "split")*/
		showSplitView();

	// UI setup done, restore splitter positions
<<<<<<< HEAD
	Q3ValueList<int> sizes = configSettings->readSizes("/split1", &ok);
=======
	QList<int> sizes = configSettings->readSizes("/split1", &ok);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ok)
		split1->setSizes(sizes);

	sizes = configSettings->readSizes("/split2", &ok);
	if (ok)
		split2->setSizes(sizes);
}

void ConfigMainWindow::loadConfig(void)
{
<<<<<<< HEAD
	QString s = Q3FileDialog::getOpenFileName(conf_get_configname(), NULL, this);
	if (s.isNull())
		return;
	if (conf_read(QFile::encodeName(s)))
		QMessageBox::information(this, "qconf", _("Unable to load configuration!"));
	ConfigView::updateListAll();
=======
	QString str;
	QByteArray ba;
	const char *name;

	str = QFileDialog::getOpenFileName(this, "", configname);
	if (str.isNull())
		return;

	ba = str.toLocal8Bit();
	name = ba.data();

	if (conf_read(name))
		QMessageBox::information(this, "qconf", "Unable to load configuration!");

	free(configname);
	configname = xstrdup(name);

	ConfigList::updateListAllForAll();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

bool ConfigMainWindow::saveConfig(void)
{
<<<<<<< HEAD
	if (conf_write(NULL)) {
		QMessageBox::information(this, "qconf", _("Unable to save configuration!"));
		return false;
	}
=======
	if (conf_write(configname)) {
		QMessageBox::information(this, "qconf", "Unable to save configuration!");
		return false;
	}
	conf_write_autoconf(0);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return true;
}

void ConfigMainWindow::saveConfigAs(void)
{
<<<<<<< HEAD
	QString s = Q3FileDialog::getSaveFileName(conf_get_configname(), NULL, this);
	if (s.isNull())
		return;
	saveConfig();
=======
	QString str;
	QByteArray ba;
	const char *name;

	str = QFileDialog::getSaveFileName(this, "", configname);
	if (str.isNull())
		return;

	ba = str.toLocal8Bit();
	name = ba.data();

	if (conf_write(name)) {
		QMessageBox::information(this, "qconf", "Unable to save configuration!");
	}
	conf_write_autoconf(0);

	free(configname);
	configname = xstrdup(name);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void ConfigMainWindow::searchConfig(void)
{
	if (!searchWindow)
<<<<<<< HEAD
		searchWindow = new ConfigSearchWindow(this, "search");
	searchWindow->show();
}

void ConfigMainWindow::changeMenu(struct menu *menu)
{
	configList->setRootMenu(menu);
	if (configList->rootEntry->parent == &rootmenu)
		backAction->setEnabled(FALSE);
	else
		backAction->setEnabled(TRUE);
=======
		searchWindow = new ConfigSearchWindow(this);
	searchWindow->show();
}

void ConfigMainWindow::changeItens(struct menu *menu)
{
	configList->setRootMenu(menu);
}

void ConfigMainWindow::changeMenu(struct menu *menu)
{
	menuList->setRootMenu(menu);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void ConfigMainWindow::setMenuLink(struct menu *menu)
{
	struct menu *parent;
	ConfigList* list = NULL;
	ConfigItem* item;

	if (configList->menuSkip(menu))
		return;

	switch (configList->mode) {
	case singleMode:
		list = configList;
		parent = menu_get_parent_menu(menu);
		if (!parent)
			return;
		list->setRootMenu(parent);
		break;
<<<<<<< HEAD
	case symbolMode:
		if (menu->flags & MENU_ROOT) {
			configList->setRootMenu(menu);
			configList->clearSelection();
			list = menuList;
		} else {
			list = configList;
			parent = menu_get_parent_menu(menu->parent);
			if (!parent)
				return;
			item = menuList->findConfigItem(parent);
			if (item) {
				menuList->setSelected(item, TRUE);
				menuList->ensureItemVisible(item);
			}
			list->setRootMenu(parent);
=======
	case menuMode:
		if (menu->flags & MENU_ROOT) {
			menuList->setRootMenu(menu);
			configList->clearSelection();
			list = configList;
		} else {
			parent = menu_get_parent_menu(menu->parent);
			if (!parent)
				return;

			/* Select the config view */
			item = configList->findConfigItem(parent);
			if (item) {
				configList->setSelected(item, true);
				configList->scrollToItem(item);
			}

			menuList->setRootMenu(parent);
			menuList->clearSelection();
			list = menuList;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
		break;
	case fullMode:
		list = configList;
		break;
	default:
		break;
	}

	if (list) {
		item = list->findConfigItem(menu);
		if (item) {
<<<<<<< HEAD
			list->setSelected(item, TRUE);
			list->ensureItemVisible(item);
			list->setFocus();
=======
			list->setSelected(item, true);
			list->scrollToItem(item);
			list->setFocus();
			helpText->setInfo(menu);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}
}

void ConfigMainWindow::listFocusChanged(void)
{
	if (menuList->mode == menuMode)
		configList->clearSelection();
}

void ConfigMainWindow::goBack(void)
{
<<<<<<< HEAD
	ConfigItem* item;

	configList->setParentMenu();
	if (configList->rootEntry == &rootmenu)
		backAction->setEnabled(FALSE);
	item = (ConfigItem*)menuList->selectedItem();
	while (item) {
		if (item->menu == configList->rootEntry) {
			menuList->setSelected(item, TRUE);
			break;
		}
		item = (ConfigItem*)item->parent();
	}
=======
	if (configList->rootEntry == &rootmenu)
		return;

	configList->setParentMenu();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void ConfigMainWindow::showSingleView(void)
{
<<<<<<< HEAD
	menuView->hide();
=======
	singleViewAction->setEnabled(false);
	singleViewAction->setChecked(true);
	splitViewAction->setEnabled(true);
	splitViewAction->setChecked(false);
	fullViewAction->setEnabled(true);
	fullViewAction->setChecked(false);

	backAction->setEnabled(true);

	menuList->hide();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	menuList->setRootMenu(0);
	configList->mode = singleMode;
	if (configList->rootEntry == &rootmenu)
		configList->updateListAll();
	else
		configList->setRootMenu(&rootmenu);
<<<<<<< HEAD
	configList->setAllOpen(TRUE);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	configList->setFocus();
}

void ConfigMainWindow::showSplitView(void)
{
<<<<<<< HEAD
	configList->mode = symbolMode;
=======
	singleViewAction->setEnabled(true);
	singleViewAction->setChecked(false);
	splitViewAction->setEnabled(false);
	splitViewAction->setChecked(true);
	fullViewAction->setEnabled(true);
	fullViewAction->setChecked(false);

	backAction->setEnabled(false);

	configList->mode = menuMode;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (configList->rootEntry == &rootmenu)
		configList->updateListAll();
	else
		configList->setRootMenu(&rootmenu);
<<<<<<< HEAD
	configList->setAllOpen(TRUE);
	configApp->processEvents();
	menuList->mode = menuMode;
	menuList->setRootMenu(&rootmenu);
	menuList->setAllOpen(TRUE);
	menuView->show();
=======
	configList->setAllOpen(true);
	configApp->processEvents();
	menuList->mode = symbolMode;
	menuList->setRootMenu(&rootmenu);
	menuList->setAllOpen(true);
	menuList->show();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	menuList->setFocus();
}

void ConfigMainWindow::showFullView(void)
{
<<<<<<< HEAD
	menuView->hide();
=======
	singleViewAction->setEnabled(true);
	singleViewAction->setChecked(false);
	splitViewAction->setEnabled(true);
	splitViewAction->setChecked(false);
	fullViewAction->setEnabled(false);
	fullViewAction->setChecked(true);

	backAction->setEnabled(false);

	menuList->hide();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	menuList->setRootMenu(0);
	configList->mode = fullMode;
	if (configList->rootEntry == &rootmenu)
		configList->updateListAll();
	else
		configList->setRootMenu(&rootmenu);
<<<<<<< HEAD
	configList->setAllOpen(FALSE);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	configList->setFocus();
}

/*
 * ask for saving configuration before quitting
<<<<<<< HEAD
 * TODO ask only when something changed
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
void ConfigMainWindow::closeEvent(QCloseEvent* e)
{
	if (!conf_get_changed()) {
		e->accept();
		return;
	}
<<<<<<< HEAD
	QMessageBox mb("qconf", _("Save configuration?"), QMessageBox::Warning,
			QMessageBox::Yes | QMessageBox::Default, QMessageBox::No, QMessageBox::Cancel | QMessageBox::Escape);
	mb.setButtonText(QMessageBox::Yes, _("&Save Changes"));
	mb.setButtonText(QMessageBox::No, _("&Discard Changes"));
	mb.setButtonText(QMessageBox::Cancel, _("Cancel Exit"));
=======

	QMessageBox mb(QMessageBox::Icon::Warning, "qconf",
		       "Save configuration?");

	QPushButton *yb = mb.addButton(QMessageBox::Yes);
	QPushButton *db = mb.addButton(QMessageBox::No);
	QPushButton *cb = mb.addButton(QMessageBox::Cancel);

	yb->setText("&Save Changes");
	db->setText("&Discard Changes");
	cb->setText("Cancel Exit");

	mb.setDefaultButton(yb);
	mb.setEscapeButton(cb);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	switch (mb.exec()) {
	case QMessageBox::Yes:
		if (saveConfig())
			e->accept();
		else
			e->ignore();
		break;
	case QMessageBox::No:
		e->accept();
		break;
	case QMessageBox::Cancel:
		e->ignore();
		break;
	}
}

void ConfigMainWindow::showIntro(void)
{
<<<<<<< HEAD
	static const QString str = _("Welcome to the qconf graphical configuration tool.\n\n"
		"For each option, a blank box indicates the feature is disabled, a check\n"
		"indicates it is enabled, and a dot indicates that it is to be compiled\n"
		"as a module.  Clicking on the box will cycle through the three states.\n\n"
		"If you do not see an option (e.g., a device driver) that you believe\n"
		"should be present, try turning on Show All Options under the Options menu.\n"
		"Although there is no cross reference yet to help you figure out what other\n"
		"options must be enabled to support the option you are interested in, you can\n"
		"still view the help of a grayed-out option.\n\n"
		"Toggling Show Debug Info under the Options menu will show the dependencies,\n"
		"which you can then match by examining other options.\n\n");
=======
	static const QString str =
		"Welcome to the qconf graphical configuration tool.\n"
		"\n"
		"For bool and tristate options, a blank box indicates the "
		"feature is disabled, a check indicates it is enabled, and a "
		"dot indicates that it is to be compiled as a module. Clicking "
		"on the box will cycle through the three states. For int, hex, "
		"and string options, double-clicking or pressing F2 on the "
		"Value cell will allow you to edit the value.\n"
		"\n"
		"If you do not see an option (e.g., a device driver) that you "
		"believe should be present, try turning on Show All Options "
		"under the Options menu. Enabling Show Debug Info will help you"
		"figure out what other options must be enabled to support the "
		"option you are interested in, and hyperlinks will navigate to "
		"them.\n"
		"\n"
		"Toggling Show Debug Info under the Options menu will show the "
		"dependencies, which you can then match by examining other "
		"options.\n";
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	QMessageBox::information(this, "qconf", str);
}

void ConfigMainWindow::showAbout(void)
{
<<<<<<< HEAD
	static const QString str = _("qconf is Copyright (C) 2002 Roman Zippel <zippel@linux-m68k.org>.\n\n"
		"Bug reports and feature request can also be entered at http://bugzilla.kernel.org/\n");

	QMessageBox::information(this, "qconf", str);
=======
	static const QString str = "qconf is Copyright (C) 2002 Roman Zippel <zippel@linux-m68k.org>.\n"
		"Copyright (C) 2015 Boris Barbulovski <bbarbulovski@gmail.com>.\n"
		"\n"
		"Bug reports and feature request can also be entered at http://bugzilla.kernel.org/\n"
		"\n"
		"Qt Version: ";

	QMessageBox::information(this, "qconf", str + qVersion());
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void ConfigMainWindow::saveSettings(void)
{
<<<<<<< HEAD
	configSettings->writeEntry("/window x", pos().x());
	configSettings->writeEntry("/window y", pos().y());
	configSettings->writeEntry("/window width", size().width());
	configSettings->writeEntry("/window height", size().height());
=======
	configSettings->setValue("/window x", pos().x());
	configSettings->setValue("/window y", pos().y());
	configSettings->setValue("/window width", size().width());
	configSettings->setValue("/window height", size().height());
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	QString entry;
	switch(configList->mode) {
	case singleMode :
		entry = "single";
		break;

	case symbolMode :
		entry = "split";
		break;

	case fullMode :
		entry = "full";
		break;

	default:
		break;
	}
<<<<<<< HEAD
	configSettings->writeEntry("/listMode", entry);
=======
	configSettings->setValue("/listMode", entry);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	configSettings->writeSizes("/split1", split1->sizes());
	configSettings->writeSizes("/split2", split2->sizes());
}

void ConfigMainWindow::conf_changed(void)
{
	if (saveAction)
		saveAction->setEnabled(conf_get_changed());
}

void fixup_rootmenu(struct menu *menu)
{
	struct menu *child;
	static int menu_cnt = 0;

	menu->flags |= MENU_ROOT;
	for (child = menu->list; child; child = child->next) {
		if (child->prompt && child->prompt->type == P_MENU) {
			menu_cnt++;
			fixup_rootmenu(child);
			menu_cnt--;
		} else if (!menu_cnt)
			fixup_rootmenu(child);
	}
}

static const char *progname;

static void usage(void)
{
<<<<<<< HEAD
	printf(_("%s <config>\n"), progname);
=======
	printf("%s [-s] <config>\n", progname);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	exit(0);
}

int main(int ac, char** av)
{
	ConfigMainWindow* v;
	const char *name;

<<<<<<< HEAD
	bindtextdomain(PACKAGE, LOCALEDIR);
	textdomain(PACKAGE);

	progname = av[0];
	configApp = new QApplication(ac, av);
	if (ac > 1 && av[1][0] == '-') {
		switch (av[1][1]) {
=======
	progname = av[0];
	if (ac > 1 && av[1][0] == '-') {
		switch (av[1][1]) {
		case 's':
			conf_set_message_callback(NULL);
			break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		case 'h':
		case '?':
			usage();
		}
		name = av[2];
	} else
		name = av[1];
	if (!name)
		usage();

	conf_parse(name);
	fixup_rootmenu(&rootmenu);
	conf_read(NULL);
	//zconfdump(stdout);

<<<<<<< HEAD
=======
	configApp = new QApplication(ac, av);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	configSettings = new ConfigSettings();
	configSettings->beginGroup("/kconfig/qconf");
	v = new ConfigMainWindow();

	//zconfdump(stdout);
<<<<<<< HEAD
	configApp->setMainWidget(v);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	configApp->connect(configApp, SIGNAL(lastWindowClosed()), SLOT(quit()));
	configApp->connect(configApp, SIGNAL(aboutToQuit()), v, SLOT(saveSettings()));
	v->show();
	configApp->exec();

	configSettings->endGroup();
	delete configSettings;
<<<<<<< HEAD
=======
	delete v;
	delete configApp;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}
