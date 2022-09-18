#include "LidarMaster.h"
#include <iostream>
#include <string>

LidarMaster::LidarMaster(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	//��ʼ�������ļ���
	m_PtrProTree = new QTreeWidget(this);
	//�����ļ���ͷΪ��
	m_PtrProTree->setHeaderHidden(true);

	m_PtrLasInfoTree = new QTreeWidget(this);


	connect(m_PtrProTree, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(treeItemClickedSlot(QTreeWidgetItem*, int)));
	// ��������������
	MainFramAttri();
	// ��ʼ���˵���ָ��
	m_PtrMenu = new LidarMenu(this, this);
	this->setMenuBar(m_PtrMenu);
	
	m_PtrQVtkWindow = new QVTKWindow(100, this, this);
	m_dockMain->setWidget(m_PtrQVtkWindow);

}

LidarMaster::~LidarMaster()
{

}


void LidarMaster::MainFramAttri()
{
	// �������
	setWindowTitle(QStringLiteral("LidarMaster"));
	// ���ͼ��
	setWindowIcon(QIcon(":/LidarMaster/img/logo.png"));
	// ���ô�С
	resize(QGuiApplication::primaryScreen()->availableSize() * 16 / 9);

	// ɾ�����봰��
	QWidget* centralWidget = takeCentralWidget();
	if (centralWidget)
		delete centralWidget;
	//����Ƕ��dock
	setDockNestingEnabled(true);

	// ����״̬��
	QStatusBar* stBar = statusBar();
	setStatusBar(stBar);
	//�����ʾ��Ϣ���Ҳ�
	QLabel* lab_cprt = new QLabel(QStringLiteral("@��Ȩ����1114809057@qq.com"), this);
	lab_cprt->setStyleSheet("color:#ffffff;margin-right:10px;");
	stBar->addPermanentWidget(lab_cprt);
	stBar->setStyleSheet("background:#000000;");

	// ������ʾ����
	m_dockMain = new QDockWidget(this);
	m_dockMain->setWindowTitle(QStringLiteral("����"));
	// ����Ϊ���ƶ��ɸ����������ɹر�
	m_dockMain->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
	// ���ƶ���Χ������
	m_dockMain->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);  

	// ������Ϣ
	m_dockProInfo = new QDockWidget(this);
	m_dockProInfo->setWindowTitle(QStringLiteral("����"));
	m_dockProInfo->setFeatures(QDockWidget::DockWidgetClosable);

	// ������Ϣ
	m_dockLasInfo = new QDockWidget(this);
	m_dockLasInfo->setWindowTitle(QStringLiteral("����"));
	m_dockLasInfo->setFeatures(QDockWidget::AllDockWidgetFeatures);

	// ������Ϣ
	m_dockOtherInfo = new QDockWidget(this);
	m_dockOtherInfo->setWindowTitle(QStringLiteral("����"));
	m_dockOtherInfo->setFeatures(QDockWidget::AllDockWidgetFeatures);

	// �����Ӵ����С
	m_dockProInfo->setFixedWidth(200);
	m_dockLasInfo->setFixedWidth(200);
	m_dockOtherInfo->setFixedWidth(200);
	m_dockLasInfo->setMaximumHeight(400);

	// ���ַ���
	setCentralWidget(m_dockMain);
	addDockWidget(Qt::LeftDockWidgetArea, m_dockProInfo);
	addDockWidget(Qt::RightDockWidgetArea, m_dockLasInfo);
	addDockWidget(Qt::RightDockWidgetArea, m_dockOtherInfo);

	// ���ַ���
	splitDockWidget(m_dockLasInfo, m_dockOtherInfo, Qt::Vertical);

	connect(m_dockLasInfo, SIGNAL(visibilityChanged(bool)), this, SLOT(isLasInfo()));
	connect(m_dockOtherInfo, SIGNAL(visibilityChanged(bool)), this, SLOT(isOtherInfo()));
	connect(m_dockProInfo, SIGNAL(visibilityChanged(bool)), this, SLOT(isProInfo()));
	m_dockProInfo->setWidget(m_PtrProTree);
	setLasInfoDock();

}
void LidarMaster::setLasInfoDock()
{
	QStringList strList;
	strList << QStringLiteral("����") << QStringLiteral("ֵ");
	m_PtrLasInfoTree->setHeaderLabels(strList);

	QTreeWidgetItem* parentItem1 = new QTreeWidgetItem(QStringList() << QStringLiteral("������Ϣ"));
	m_PtrLasInfoTree->addTopLevelItem(parentItem1);
	QTreeWidgetItem* item11 = new QTreeWidgetItem();
	item11->setText(0, QStringLiteral("���Ƹ���"));
	item11->setText(1, QStringLiteral(""));

	QTreeWidgetItem* item12 = new QTreeWidgetItem();
	item12->setText(0, QStringLiteral("���Ƹ�ʽ"));
	item12->setText(1, QStringLiteral(""));
	QTreeWidgetItem* item13 = new QTreeWidgetItem();
	item13->setText(0, QStringLiteral("��������"));
	item13->setText(1, QStringLiteral(""));
	parentItem1->addChild(item11);
	parentItem1->addChild(item12);
	parentItem1->addChild(item13);

	m_dockLasInfo->setWidget(m_PtrLasInfoTree);
}
void LidarMaster::isProInfo()
{
	if (m_dockProInfo->isHidden())
	{
		m_PtrMenu->m_isShowProInfoAct->setIcon(QIcon(":/LidarMaster/img/hide.png"));
	}
	else
	{
		m_PtrMenu->m_isShowProInfoAct->setIcon(QIcon(":/LidarMaster/img/show.png"));
	}
}

void LidarMaster::isOtherInfo()
{
	if (m_dockOtherInfo->isHidden())
	{
		m_PtrMenu->m_isShowOtherInfoAct->setIcon(QIcon(":/LidarMaster/img/hide.png"));
	}
	else
	{
		m_PtrMenu->m_isShowOtherInfoAct->setIcon(QIcon(":/LidarMaster/img/show.png"));
	}
}

void LidarMaster::treeItemClickedSlot(QTreeWidgetItem* item, int col)
{
	qDebug() << "item:" << item->text(0);
	if (item->parent())
	{
		if (item->checkState(0)==Qt::Checked)
		{
			QString lidarFile = item->data(0, Qt::UserRole + 1).toString();
			m_PtrQVtkWindow->showLidarData(lidarFile);
		}
		else
		{
			QString tmp = "";
			m_PtrQVtkWindow->showLidarData(tmp);
		}

	}
}

void LidarMaster::isLasInfo()
{
	if (m_dockLasInfo->isHidden())
	{
		m_PtrMenu->m_isShowLasInfoAct->setIcon(QIcon(":/LidarMaster/img/hide.png"));
	}
	else
	{
		m_PtrMenu->m_isShowLasInfoAct->setIcon(QIcon(":/LidarMaster/img/show.png"));
	}
}
