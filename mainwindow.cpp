#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    XMLParser *xmlparser = new XMLParser("D:/","test.xml");

    QFile *file;
    //QString  filename = "D:/test.xml";
    if(!file->exists("D:/test.xml"))  {
        xmlparser->create_xml("D:/","test.xml");

    }
    bool ret = xmlparser->add_element("D:/","test.xml","password","12345");

    QString password = xmlparser->get_element("D:/","test.xml","password");

    qDebug()<<"password is "<<password;

    bool ret01 = xmlparser->update_element("D:/","test.xml","password","342345");


    bool ret02 = xmlparser->delete_element("D:/","test.xml","password","1212342345");
    qDebug()<<"ret02 is "<<ret02;

    bool ret03 = xmlparser->delete_element("D:/","test.xml","password","342345");
    qDebug()<<"ret03 is "<<ret03;

//    QString path = xmlparser->get_savepath();

//    if(NULL==path||path==""){
//        QString fileName = QFileDialog::getExistingDirectory(this, tr("请选择保存目录"),
//                                                          "",
//                                                       QFileDialog::ShowDirsOnly
//                                                          | QFileDialog::DontResolveSymlinks);
//        qDebug()<<"fileName is "<<fileName;
//        bool ret = xmlparser->set_savepath(fileName);
//        qDebug()<<"ret is "<<ret;
//    }else{
//        qDebug()<<"path is "<<path;
//    }


}

MainWindow::~MainWindow()
{

    delete ui;
}
