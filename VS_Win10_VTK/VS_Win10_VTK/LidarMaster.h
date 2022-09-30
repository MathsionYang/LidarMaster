#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_LidarMaster.h"
#include "QVTKOpenGLNativeWidget.h"
#include "vtkGenericOpenGLRenderWindow.h"
#include <qdebug.h>
#include <QDockWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include "LidarMenu.h"
#include "QVTKWindow.h"
#include <qsharedpointer.h>
class LidarMenu;
class QVTKWindow;
class LidarMaster : public QMainWindow
{
    Q_OBJECT

public:
    LidarMaster(QWidget *parent = nullptr);
    ~LidarMaster();

     // �˵�
     QPointer<LidarMenu> m_PtrMenu;
     // ��ʾLas������
     QPointer<QDockWidget> m_dockMain;
     // ��ʾ������Ϣ������
     QPointer<QDockWidget> m_dockProInfo;
     // ��ʾLas��Ϣ������
     QPointer<QDockWidget> m_dockLasInfo;
     // ��ʾ������Ϣ������
     QPointer<QDockWidget> m_dockOtherInfo;

     // �����ļ���
     QPointer<QTreeWidget> m_PtrProTree;

     // ������Ϣ��
     QPointer<QTreeWidget> m_PtrLasInfoTree;

     QPointer< QVTKWindow> m_PtrQVtkWindow;

     QPointer<QVTKOpenGLNativeWidget> m_LidarWidget;
     pcl::visualization::PCLVisualizer::Ptr viewer;
     vtkSmartPointer<vtkRenderer>renderer2;
     vtkSmartPointer<vtkGenericOpenGLRenderWindow>renderWindow2;
     pcl::PointCloud<pcl::PointXYZ>::Ptr m_Cloud;
private:
    Ui::LidarMasterClass ui;
    // ��������������
    void MainFramAttri();

    // ���õ�����Ϣ����
    void setLasInfoDock();
    QSharedPointer<std::thread> m_MainThread;
    void showLidarData(QString& lidarFile);

    void showPtCloudHeightInfo();
signals:
    void sendRenderAxis(QString& strAxis);
    void sendColorInfo(QColor& color);
private slots:
    // �رյ�����Ϣ�ź�
    void isLasInfo();

    // �رչ�����Ϣ�ź�
    void isProInfo();

    // �ر�������Ϣ�ź�
    void isOtherInfo();

    // �������ڵ�����
    void treeItemClickedSlot(QTreeWidgetItem* item, int col);

  

    // 	������Ⱦ�Ի���ۺ���


    //ˢ�µ�����ͼ
    void updateWindSlot();


    void recvRenderCoords(QString& strAxis);

    void recColorInfoSlot(QColor& color);

};
