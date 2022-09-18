#pragma once

#include <QObject>
#include <QMenuBar>
#include <QtWidgets>
#include "LidarMaster.h"
#include <QPointer>
#include <QToolBar>
#include <QAction>
#include "LidarNewPro.h"
class LidarMaster;
class LidarNewPro;
class LidarMenu  : public QMenuBar
{
	Q_OBJECT

public:
	LidarMenu(QWidget*parent, LidarMaster *lasMaster);
	~LidarMenu();

	// �Ƿ���ʾ��������
	QPointer<QAction> m_isShowProInfoAct;

	// �Ƿ���ʾ������Ϣ����
	QPointer<QAction> m_isShowLasInfoAct;

	// �Ƿ���ʾ������Ϣ����
	QPointer<QAction> m_isShowOtherInfoAct;

private:
	//�����ָ��
	LidarMaster *m_PtrLidarMaster;
	// ������
	QPointer<QToolBar> m_PtrToolBar;
	//�½�����
	QPointer<LidarNewPro>m_PtrNewPro;
	
	
	// �ļ�ģ��
	void menu_File();
	// ��ͼģ��
	void menu_ShowWin();

	

private slots:
	//�½�����
	void File_new();
	//�½�����
	void Pro_Open();

	// �Ƿ���ʾ��������
	void showProInfo();
	// �Ƿ���ʾ������Ϣ����
	void showLasInfo();
	// �Ƿ���ʾ������Ϣ����
	void showOtherInfo();
};