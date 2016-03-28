#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <QObject>
#include <QTextStream>
#include <QDebug>
#include <QDir>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNodeList>


class XMLParser : public QObject
{
    Q_OBJECT
public:
    explicit XMLParser(QObject *parent = 0);
    explicit XMLParser(QString filepath ,QString filename, QObject *parent = 0);
    ~XMLParser();
    QString get_savepath();
    bool set_savepath(QString path);


    void create_xml(QString filepath ,QString filename);
    QString get_element(QString filepath ,QString filename,QString element);
    bool add_element(QString filepath ,QString filename,QString element,QString value);
    bool update_element(QString filepath ,QString filename,QString element,QString value);
    bool delete_element(QString filepath ,QString filename,QString element,QString value);


signals:

public slots:

private:
    QDir dir;
    QString configPath;
    QFile *file;
    QString filepath;
    QString filename;

};

#endif // XMLPARSER_H
