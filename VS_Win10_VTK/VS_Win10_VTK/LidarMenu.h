#pragma once

#include <QObject>
#include <QMenuBar>
#include <QtWidgets>
#include "LidarMaster.h"
#include <QPointer>
#include <QToolBar>
#include <QAction>
#include "LidarNewPro.h"
#include "PtRenderWidget.h"
#include <qcolordialog.h>
#include <qcolor.h>
class LidarMaster;
class LidarNewPro;
class PtCloudRender;
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
	//��Ⱦ�Ի���
	QPointer< PtRenderWidget> m_PtrCloudRender;
	
	// �ļ�ģ��
	void menu_File();
	// ��ͼģ��
	void menu_ShowWin();
	// ��������
	void menu_PtCloudSetup();

signals:
	void sendLidarColor(QColor& color);

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

	//��ʾ��Ⱦ�Ի���
	void showRenderDialog();
	// ��ʾ������ɫ��
	void showColorDialog();



};
