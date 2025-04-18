#include "CustomItemWidget.h"

CustomItemWidget::CustomItemWidget(int styleIndex, QWidget* parent, const QString& text, const QString& text2, const QString& text3, const QString& text4)
    : QWidget(parent)
    , ui_ItemWidgetMeshBoundaries1(nullptr)
    , ui_ItemWidgetMeshBoundaries2(nullptr)
    , ui_ItemWidgetMeshRegions1(nullptr)
    , ui_ItemWidgetMeshRegions2(nullptr)
	, ui_ItemWidgetMeshZones(nullptr)
	, ui_ItemWidgetBoundaryConditions(nullptr)
    , lineEdit(nullptr)
    , editing(false)
	, previousTypeIndex(0)
	, previousOptionIndex(0)
{
	initializeUI(styleIndex, text, text2, text3, text4);
}

CustomItemWidget::~CustomItemWidget()
{
    delete ui_ItemWidgetMeshBoundaries1;
    delete ui_ItemWidgetMeshBoundaries2;
    delete ui_ItemWidgetMeshRegions1;
    delete ui_ItemWidgetMeshRegions2;
	delete ui_ItemWidgetMeshZones;
	delete ui_ItemWidgetBoundaryConditions;
}

void CustomItemWidget::initializeUI(int styleIndex, const QString& text, const QString& text2, const QString& text3, const QString& text4)
{
    if (styleIndex == 0) {
        ui_ItemWidgetMeshBoundaries1 = new Ui::CustomItemWidgetMeshBoundaries1Class();
        ui_ItemWidgetMeshBoundaries1->setupUi(this);
        ui_ItemWidgetMeshBoundaries1->label->setText(text);

        // 初始化 text
        this->text1 = text;
        this->text2 = text2;

        // 设置 QComboBox 的自定义视图
        QListView* combo1View1 = new QListView(ui_ItemWidgetMeshBoundaries1->comboBox);
        combo1View1->setMinimumWidth(150);
        ui_ItemWidgetMeshBoundaries1->comboBox->setView(combo1View1);

        QListView* combo1View2 = new QListView(ui_ItemWidgetMeshBoundaries1->comboBox_2);
        combo1View2->setMinimumWidth(150);
        ui_ItemWidgetMeshBoundaries1->comboBox_2->setView(combo1View2);

        // 连接类型切换信号槽
        connect(ui_ItemWidgetMeshBoundaries1->comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int currentIndex) {
            emit typeChanged(this, previousTypeIndex); 
            previousTypeIndex = currentIndex; 
            });

		// 连接选项选择信号槽
		connect(ui_ItemWidgetMeshBoundaries1->comboBox_2, QOverload<int>::of(&QComboBox::activated), this, [=](int currentIndex) {
			emit optionChanged(this, previousOptionIndex);
			previousOptionIndex = currentIndex;
			});

        // 安装事件过滤器
        ui_ItemWidgetMeshBoundaries1->label->installEventFilter(this);
    }
    else if (styleIndex == 1) {
        ui_ItemWidgetMeshBoundaries2 = new Ui::CustomItemWidgetMeshBoundaries2Class();
        ui_ItemWidgetMeshBoundaries2->setupUi(this);
        ui_ItemWidgetMeshBoundaries2->label->setText(text);
		ui_ItemWidgetMeshBoundaries2->label_2->setText(text2);

		//初始化 text
		this->text1 = text;
		this->text2 = text2;
		this->text3 = text3;
		this->text4 = text4;

        // 安装事件过滤器
        ui_ItemWidgetMeshBoundaries2->label->installEventFilter(this);
        ui_ItemWidgetMeshBoundaries2->label_2->installEventFilter(this);
    }
    else if (styleIndex == 2) {
        ui_ItemWidgetMeshRegions1 = new Ui::CustomItemWidgetMeshRegions1Class();
        ui_ItemWidgetMeshRegions1->setupUi(this);
        ui_ItemWidgetMeshRegions1->label->setText(text);

        // 初始化 text
        this->text1 = text;

        // 设置 QComboBox 的自定义视图
        QListView* combo1View1 = new QListView(ui_ItemWidgetMeshRegions1->comboBox);
        combo1View1->setMinimumWidth(150);
        ui_ItemWidgetMeshRegions1->comboBox->setView(combo1View1);

        // 连接选项选择信号槽
        connect(ui_ItemWidgetMeshRegions1->comboBox, QOverload<int>::of(&QComboBox::activated), this, [=](int currentIndex) {  
           emit optionChanged(this, previousOptionIndex);  
           previousOptionIndex = currentIndex;  
        });

        // 安装事件过滤器
        ui_ItemWidgetMeshRegions1->label->installEventFilter(this);
    }
    else if (styleIndex == 3) {
        ui_ItemWidgetMeshRegions2 = new Ui::CustomItemWidgetMeshRegions2Class();
        ui_ItemWidgetMeshRegions2->setupUi(this);
        ui_ItemWidgetMeshRegions2->label->setText(text);

        // 初始化 text
        this->text1 = text;

        // 设置 QComboBox 的自定义视图
        QListView* combo1View1 = new QListView(ui_ItemWidgetMeshRegions2->comboBox);
        combo1View1->setMinimumWidth(150);
        ui_ItemWidgetMeshRegions2->comboBox->setView(combo1View1);

        QListView* combo1View2 = new QListView(ui_ItemWidgetMeshRegions2->comboBox_2);
        combo1View2->setMinimumWidth(150);
        ui_ItemWidgetMeshRegions2->comboBox_2->setView(combo1View2);

        // 连接类型切换信号槽
        connect(ui_ItemWidgetMeshRegions2->comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int currentIndex) {
            emit typeChanged(this, previousTypeIndex);
            previousTypeIndex = currentIndex;
            });

        // 连接选项选择信号槽
        connect(ui_ItemWidgetMeshRegions2->comboBox_2, QOverload<int>::of(&QComboBox::activated), this, [=](int currentIndex) {
            emit optionChanged(this, previousOptionIndex);
            previousOptionIndex = currentIndex;
            });

        // 安装事件过滤器
        ui_ItemWidgetMeshRegions2->label->installEventFilter(this);
    }
    else if (styleIndex == 4) {
		ui_ItemWidgetMeshZones = new Ui::CustomItemWidgetMeshZonesClass();
		ui_ItemWidgetMeshZones->setupUi(this);
		ui_ItemWidgetMeshZones->label->setText(text);

        // 初始化 text
        this->text1 = text;

		// 设置 QComboBox 的自定义视图
		QListView* combo1View1 = new QListView(ui_ItemWidgetMeshZones->comboBox);
		combo1View1->setMinimumWidth(150);
		ui_ItemWidgetMeshZones->comboBox->setView(combo1View1);

        // 连接选项选择信号槽
        connect(ui_ItemWidgetMeshZones->comboBox, QOverload<int>::of(&QComboBox::activated), this, [=](int currentIndex) {
            emit optionChanged(this, previousOptionIndex);
            previousOptionIndex = currentIndex;
            });

		// 安装事件过滤器
		ui_ItemWidgetMeshZones->label->installEventFilter(this);
    }
    else if (styleIndex == 5) {
		ui_ItemWidgetBoundaryConditions = new Ui::CustomItemWidgetBoundaryConditionsClass();
		ui_ItemWidgetBoundaryConditions->setupUi(this);
		ui_ItemWidgetBoundaryConditions->label->setText(text);

		// 初始化 text
		this->text1 = text;
        this->text3 = text3;
        if (text2 == "patch")
        {
            QPixmap pixmap("../res/patch.png");
            ui_ItemWidgetBoundaryConditions->label_2->setPixmap(pixmap);

			ui_ItemWidgetBoundaryConditions->comboBox->clear();
            QPixmap pressureInletIcon("../res/PressureInlet.png");
            ui_ItemWidgetBoundaryConditions->comboBox->addItem(QIcon(pressureInletIcon), "Pressure Inlet");
			QPixmap velocityInletIcon("../res/VelocityInlet.png");
			ui_ItemWidgetBoundaryConditions->comboBox->addItem(QIcon(velocityInletIcon), "Velocity Inlet");
			QPixmap massFlowInletIcon("../res/MassFlowInlet.png");
			ui_ItemWidgetBoundaryConditions->comboBox->addItem(QIcon(massFlowInletIcon), "Mass Flow Inlet");
			QPixmap pressureOutletIcon("../res/PressureOutlet.png");
			ui_ItemWidgetBoundaryConditions->comboBox->addItem(QIcon(pressureOutletIcon), "Pressure Outlet");
			QPixmap outflowIcon("../res/Outflow.png");
			ui_ItemWidgetBoundaryConditions->comboBox->addItem(QIcon(outflowIcon), "Outflow");
			QPixmap freeStreamIcon("../res/FreeStream.png");
			ui_ItemWidgetBoundaryConditions->comboBox->addItem(QIcon(freeStreamIcon), "Free Stream");
			QPixmap customIcon("../res/Custom.png");
			ui_ItemWidgetBoundaryConditions->comboBox->addItem(QIcon(customIcon), "Custom");
		}
        else if (text2 == "wall")
        {
			QPixmap pixmap("../res/wall.png");
			ui_ItemWidgetBoundaryConditions->label_2->setPixmap(pixmap);

			ui_ItemWidgetBoundaryConditions->comboBox->clear();
			QPixmap wallIcon("../res/Wall.png");
			ui_ItemWidgetBoundaryConditions->comboBox->addItem(QIcon(wallIcon), "Wall");
            QPixmap customIcon("../res/Custom.png");
            ui_ItemWidgetBoundaryConditions->comboBox->addItem(QIcon(customIcon), "Custom");
        }
		else if (text2 == "symmetry")
		{
            QPixmap pixmap("../res/symmetry.png");
            ui_ItemWidgetBoundaryConditions->label_2->setPixmap(pixmap);
            ui_ItemWidgetBoundaryConditions->comboBox->clear();
            QPixmap symmetryIcon("../res/Symmetry.png");
            ui_ItemWidgetBoundaryConditions->comboBox->addItem(QIcon(symmetryIcon), "Symmetry");
        }
		else if (text2 == "empty")
		{
			QPixmap pixmap("../res/empty.png");
			ui_ItemWidgetBoundaryConditions->label_2->setPixmap(pixmap);
			ui_ItemWidgetBoundaryConditions->comboBox->clear();
			QPixmap emptyIcon("../res/Empty.png");
			ui_ItemWidgetBoundaryConditions->comboBox->addItem(QIcon(emptyIcon), "Empty");
		}
		else if (text2 == "Wedge")
		{
			QPixmap pixmap("../res/Wedge.png");
			ui_ItemWidgetBoundaryConditions->label_2->setPixmap(pixmap);
			ui_ItemWidgetBoundaryConditions->comboBox->clear();
			QPixmap wedgeIcon("../res/Wedge.png");
			ui_ItemWidgetBoundaryConditions->comboBox->addItem(QIcon(wedgeIcon), "Wedge");
		}
        else if (text2 == "Overset")
        {
			QPixmap pixmap("../res/Overset.png");
			ui_ItemWidgetBoundaryConditions->label_2->setPixmap(pixmap);
			ui_ItemWidgetBoundaryConditions->comboBox->clear();
			QPixmap oversetIcon("../res/Overset.png");
			ui_ItemWidgetBoundaryConditions->comboBox->addItem(QIcon(oversetIcon), "Overset");
        }
    }
    else {
        return;
    }
}

bool CustomItemWidget::eventFilter(QObject* obj, QEvent* event)
{
    if (ui_ItemWidgetMeshBoundaries1 && obj == ui_ItemWidgetMeshBoundaries1->label) {
        if (event->type() == QEvent::Enter) {
            ui_ItemWidgetMeshBoundaries1->label->setText(text2);
            return true;
        }
        else if (event->type() == QEvent::Leave) {
            ui_ItemWidgetMeshBoundaries1->label->setText(text1);
            return true;
        }
        else if (event->type() == QEvent::MouseButtonDblClick) {
            beginEdit(ui_ItemWidgetMeshBoundaries1->label);
            return true;
        }
    }
    if (ui_ItemWidgetMeshBoundaries2 && obj == ui_ItemWidgetMeshBoundaries2->label) {
        if (event->type() == QEvent::Enter) {
			ui_ItemWidgetMeshBoundaries2->label->setText(text3);          
            return true;
        }
        else if (event->type() == QEvent::Leave) {
			ui_ItemWidgetMeshBoundaries2->label->setText(text1);           
            return true;
        }
        else if (event->type() == QEvent::MouseButtonDblClick) {
			beginEdit(ui_ItemWidgetMeshBoundaries2->label);        
            return true;
        }
    }
	if (ui_ItemWidgetMeshBoundaries2 && obj == ui_ItemWidgetMeshBoundaries2->label_2) {
		if (event->type() == QEvent::Enter) {
			ui_ItemWidgetMeshBoundaries2->label_2->setText(text4);
			return true;
		}
		else if (event->type() == QEvent::Leave) {
			ui_ItemWidgetMeshBoundaries2->label_2->setText(text2);
			return true;
		}
		else if (event->type() == QEvent::MouseButtonDblClick) {
			beginEdit(ui_ItemWidgetMeshBoundaries2->label_2);
			return true;
		}
	}
    if (ui_ItemWidgetMeshRegions1 && obj == ui_ItemWidgetMeshRegions1->label && event->type() == QEvent::MouseButtonDblClick) {
        return true;
    }
    if (ui_ItemWidgetMeshRegions2 && obj == ui_ItemWidgetMeshRegions2->label && event->type() == QEvent::MouseButtonDblClick) {
        beginEdit(ui_ItemWidgetMeshRegions2->label);
        return true;
    }
	if (ui_ItemWidgetMeshZones && obj == ui_ItemWidgetMeshZones->label && event->type() == QEvent::MouseButtonDblClick) {
		beginEdit(ui_ItemWidgetMeshZones->label);
		return true;
	}
	if (ui_ItemWidgetBoundaryConditions && obj == ui_ItemWidgetBoundaryConditions->label && event->type() == QEvent::MouseButtonDblClick) {
		beginEdit(ui_ItemWidgetBoundaryConditions->label);
		return true;
	}

    if (editing && event->type() == QEvent::MouseButtonPress) {
        if (obj != lineEdit) {
            finishEdit();
            return true;
        }
    }

    if (editing && event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
            finishEdit();
            return true;
        }
    }

    return QWidget::eventFilter(obj, event);
}

void CustomItemWidget::beginEdit(QLabel* label)
{
    if (editing) return;
    editing = true;

    QBoxLayout* boxLayout = nullptr;

    if (ui_ItemWidgetMeshBoundaries1) {
        boxLayout = qobject_cast<QBoxLayout*>(ui_ItemWidgetMeshBoundaries1->horizontalLayout);
    }
    else if (ui_ItemWidgetMeshBoundaries2) {
        boxLayout = qobject_cast<QBoxLayout*>(ui_ItemWidgetMeshBoundaries2->horizontalLayout);
    }
	else if (ui_ItemWidgetMeshRegions1) {
		boxLayout = qobject_cast<QBoxLayout*>(ui_ItemWidgetMeshRegions1->horizontalLayout);
	}
	else if (ui_ItemWidgetMeshRegions2) {
		boxLayout = qobject_cast<QBoxLayout*>(ui_ItemWidgetMeshRegions2->horizontalLayout);
	}
	else if (ui_ItemWidgetMeshZones) {
		boxLayout = qobject_cast<QBoxLayout*>(ui_ItemWidgetMeshZones->horizontalLayout);
	}
	else if (ui_ItemWidgetBoundaryConditions) {
		boxLayout = qobject_cast<QBoxLayout*>(ui_ItemWidgetBoundaryConditions->horizontalLayout);
	}

    if (!label || !boxLayout) return;

    
	QString text = label->text();
	int index = text.indexOf(" in ");
	if (index != -1) text = text.left(index);

    lineEdit = new QLineEdit(text, this);
    lineEdit->setStyleSheet(
        "QLineEdit {"
        "    border: 1px solid rgb(192, 192, 192);" // 默认边框颜色
        "    font-family: 'Arial Rounded MT Bold';"// 字体
        "    font-weight: 250;"                    // 字体粗细
        "    color: rgb(66, 66, 66);"              // 字体颜色
        "}"
        "QLineEdit:focus {"
        "    border: 1px solid rgb(50, 170, 233);" // 焦点时的边框颜色
        "    outline: none;"                       // 移除默认虚线框
        "}"
    );
    lineEdit->setGeometry(label->geometry()); 
    lineEdit->setSizePolicy(label->sizePolicy());
    int labelIndex = boxLayout->indexOf(label);
    boxLayout->removeWidget(label);
    label->hide();
    boxLayout->insertWidget(labelIndex, lineEdit);
    lineEdit->setFocus();

    qApp->installEventFilter(this);
}

void CustomItemWidget::finishEdit()
{
    editing = false;
    QString newText = lineEdit->text();

    QLabel* label = nullptr;
    QBoxLayout* boxLayout = nullptr;

    if (ui_ItemWidgetMeshBoundaries1) {
        label = ui_ItemWidgetMeshBoundaries1->label;
        boxLayout = qobject_cast<QBoxLayout*>(ui_ItemWidgetMeshBoundaries1->horizontalLayout);
		QString previousText = text2;
        text2.replace(text1, newText);
		text1.replace(text1, newText);	
        emit textChanged(this, previousText);
    }
    else if (ui_ItemWidgetMeshBoundaries2) {
        if (ui_ItemWidgetMeshBoundaries2->label->isHidden()) {
            label = ui_ItemWidgetMeshBoundaries2->label;
            QString previousText = text3;
            text3.replace(text1, newText);
			text1.replace(text1, newText);	
            emit textChanged(this, previousText);
		}
        else {
            label = ui_ItemWidgetMeshBoundaries2->label_2;
            QString previousText = text4;
            text4.replace(text2, newText);
			text2.replace(text2, newText);			
            emit textChanged(this, previousText);
        }
        boxLayout = qobject_cast<QBoxLayout*>(ui_ItemWidgetMeshBoundaries2->horizontalLayout);
    }
	else if (ui_ItemWidgetMeshRegions1) {
		label = ui_ItemWidgetMeshRegions1->label;
		boxLayout = qobject_cast<QBoxLayout*>(ui_ItemWidgetMeshRegions1->horizontalLayout);
	}
	else if (ui_ItemWidgetMeshRegions2) {
		label = ui_ItemWidgetMeshRegions2->label;
		boxLayout = qobject_cast<QBoxLayout*>(ui_ItemWidgetMeshRegions2->horizontalLayout);
        QString previousText = text1;
        text1.replace(text1, newText);     
        emit textChanged(this, previousText);
	}
	else if (ui_ItemWidgetMeshZones) {
		label = ui_ItemWidgetMeshZones->label;
		boxLayout = qobject_cast<QBoxLayout*>(ui_ItemWidgetMeshZones->horizontalLayout);
        QString previousText = text1;
        text1.replace(text1, newText);  
        emit textChanged(this, previousText);
	}
	else if (ui_ItemWidgetBoundaryConditions) {
		label = ui_ItemWidgetBoundaryConditions->label;
		boxLayout = qobject_cast<QBoxLayout*>(ui_ItemWidgetBoundaryConditions->horizontalLayout);
		QString previousText = text1;
		text1.replace(text1, newText);
		emit textChanged(this, previousText);
	}

    if (!label || !boxLayout) return;

    int lineEditIndex = boxLayout->indexOf(lineEdit); 
    boxLayout->removeWidget(lineEdit);
    lineEdit->hide();
    label->setText(newText);
    boxLayout->insertWidget(lineEditIndex, label); 
    label->show();

    lineEdit->deleteLater();
    lineEdit = nullptr;

    qApp->removeEventFilter(this);
}
