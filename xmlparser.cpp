#include "xmlparser.h"

#define __XMLPARSER_DEBUG__

XMLParser::XMLParser(QObject *parent) : QObject(parent)
{

    configPath = dir.currentPath()+"/setting.xml";
    file=new QFile(configPath);

}


XMLParser::XMLParser(QString filepath ,QString filename,QObject *parent) : QObject(parent)
{
    this->filepath = filepath;
    this->filename=filename;

}

QString XMLParser::get_savepath(){

    QDomDocument dom;

    if(file->open(QIODevice::ReadOnly))
    {
        dom.setContent(file);
    }else{
        return NULL;
    }
    QDomNodeList save_path=dom.elementsByTagName("save_path");

    int elementnum = save_path.count();
    QString savepath = save_path.item(0).toElement().text();

    file->close();
    if(NULL!=savepath&&savepath!=""){
        return savepath;
    }else{
        return NULL;
    }

}

bool XMLParser::set_savepath(QString path){

    if(NULL==path||path==""){
        return false;
    }
    if(!file->open(QIODevice::WriteOnly|QIODevice::ReadOnly))
    {
        return false;
    }

    QDomDocument doc;
    QDomText text;
    QDomElement element;
    QDomProcessingInstruction instruction;
    instruction=doc.createProcessingInstruction("xml","version=\'1.0\'");
    doc.appendChild(instruction);
    QDomElement root=doc.createElement("root");
    doc.appendChild(root);
    QDomElement general=doc.createElement("general");
    root.appendChild(general);

    element=doc.createElement("save_path");
    text=doc.createTextNode(path);
    element.appendChild(text);
    general.appendChild(element);

    QTextStream out(file);
    doc.save(out,4);
    file->close();

    return true;
}


QString XMLParser::get_element(QString filepath ,QString filename,QString elementstr){
    if(NULL==filepath||NULL==filename||NULL==elementstr){
        return NULL;
    }
    QString Path = filepath+filename;
    //QString Path = filepath.endsWith("/")?filepath+filename:filepath.mid(0,filepath.length()-1);+filename;
    QFile myfile(Path);
    QDomDocument dom;

    if(myfile.open(QIODevice::ReadOnly))
    {
        QString errorStr;
        int errorLine;
        int errorColumn;

        //QDomDocument doc;
        if (!dom.setContent(&myfile, false, &errorStr, &errorLine, &errorColumn)) {
#ifdef __XMLPARSER_DEBUG__
            qDebug()<<"setcontent error..."<<" errorStr is "<<errorStr<<" errorLine is "<<errorLine<<" errorColumn is "<<errorColumn ;
#endif
            myfile.close();
        }
        myfile.close();

        //dom.setContent(myfile);
    }else{
#ifdef __XMLPARSER_DEBUG__
        qDebug()<<"open xml read error..." ;
#endif
        return NULL;
    }

    QDomElement root = dom.documentElement();
    if (root.tagName() != "root") {
#ifdef __XMLPARSER_DEBUG__
        qDebug()<<"root.tagname != root..." ;
#endif
    }

    QDomNode node = root.firstChild();
    while(!node.isNull())
    {
        if(node.isElement()){
            QDomElement element = node.toElement();
#ifdef __XMLPARSER_DEBUG__
            qDebug()<<"tagName is "<<element.tagName()<<" element.text is "<<element.text();
#endif

            if(element.tagName()==elementstr){
                return (element.text().length()>0&&NULL!=element.text())?element.text():NULL;
            }

        }
        node = node.nextSibling();
    }
    return NULL;



    //    QDomNodeList save_path=dom.elementsByTagName(element);

    //    int elementnum = save_path.count();
    //    QString savepath = save_path.item(0).toElement().text();

    //    myfile->close();
    //    if(NULL!=savepath&&savepath!=""){
    //        return savepath;
    //    }else{
    //        return NULL;
    //    }

}

bool XMLParser::add_element(QString filepath ,QString filename,QString element,QString value){

#ifdef __XMLPARSER_DEBUG__
    qDebug()<<"in add_element";
#endif
    if(NULL==filepath||NULL==filename||NULL==element||NULL==value){
        return NULL;
    }
    //QString Path = filepath.endsWith("/")?filepath+filename:filepath.mid(0,filepath.length()-1);+filename;

    QString Path = filepath+filename;

    QFile myfile(Path);
    if(!myfile.open(QIODevice::WriteOnly|QIODevice::ReadOnly))
    {
        return false;
    }

    QDomDocument doc;
    QString errorStr;
    int errorLine;
    int errorColumn;

    if (!doc.setContent(&myfile, false, &errorStr, &errorLine, &errorColumn)) {
#ifdef __XMLPARSER_DEBUG__
        qDebug()<<"setcontent error..."<<" errorStr is "<<errorStr<<" errorLine is "<<errorLine<<" errorColumn is "<<errorColumn ;
#endif
        myfile.close();
    }
    myfile.close();
    QDomElement root = doc.documentElement();
    if(root.isNull())
    {
        root = doc.createElement("root");
    }


    bool if_exist = false;
    QDomNode node = root.firstChild();
    while(!node.isNull())
    {
        if(node.isElement()){
            QDomElement local_element = node.toElement();
#ifdef __XMLPARSER_DEBUG__
            qDebug()<<"tagName is "<<local_element.tagName()<<" element.text is "<<local_element.text();

#endif
            if(local_element.tagName()==element){
                if_exist = true;
                break;
            }

        }
        node = node.nextSibling();
    }

    if(!if_exist){
        QDomElement element_my = doc.createElement(element);
        QDomText text;
        text =doc.createTextNode(value);
        element_my.appendChild(text);
        root.appendChild(element_my);
    }
#ifdef __XMLPARSER_DEBUG__
    qDebug()<<"element is "<<element;
    qDebug()<<"value is "<<value;
#endif
    if(!myfile.open(QIODevice::WriteOnly)){
#ifdef __XMLPARSER_DEBUG__
        qDebug() << "open for add error!";
#endif
    }
    QTextStream out(&myfile);
    doc.save(out,4);
    myfile.close();

    return true;


}

void XMLParser::create_xml(QString filepath ,QString filename){
#ifdef __XMLPARSER_DEBUG__
    qDebug()<<"in create_xml";
#endif
    if(NULL==filepath||NULL==filename){
        return;
    }
    QString Path = filepath+filename;
    //QString Path = filepath.endsWith("/")?filepath+filename:filepath.mid(0,filepath.length()-1);+filename;
    QFile myfile(Path);
    myfile.open(QIODevice::ReadWrite);

    QDomDocument doc;
    QDomProcessingInstruction instruction;
    instruction = doc.createProcessingInstruction("xml","version=\'1.0\'");
    doc.appendChild(instruction);
    QDomElement root = doc.createElement("root");

    doc.appendChild(root);
    QDomText text = doc.createTextNode("");
    root.appendChild(text);
    QTextStream out(&myfile);
    doc.save(out,4);
    myfile.close();
    return;
}

bool XMLParser::update_element(QString filepath ,QString filename,QString element,QString value){
    bool if_sucess = false;
    if(NULL==filepath||NULL==filename||NULL==element||NULL==value){
        return false;
    }

    QString Path = filepath+filename;

    QFile myfile(Path);
    QDomDocument doc;
    QString errorStr;
    int errorLine;
    int errorColumn;

    if (!doc.setContent(&myfile, false, &errorStr, &errorLine, &errorColumn)) {
#ifdef __XMLPARSER_DEBUG__
        qDebug()<<"setcontent error..."<<" errorStr is "<<errorStr<<" errorLine is "<<errorLine<<" errorColumn is "<<errorColumn ;
#endif
        myfile.close();
    }

    myfile.close();

    QDomElement root = doc.documentElement();

    QDomNodeList lists = doc.elementsByTagName(element);

    //新建节点
    QDomElement newNodeTag = doc.createElement(element);
    QDomText newNodeText = doc.createTextNode(value);
    newNodeTag.appendChild(newNodeText);

    int num = 0;
    for(num = 0;num<lists.size();num++){
        QDomElement ele = lists.at(num).toElement();
#ifdef __XMLPARSER_DEBUG__
        qDebug() << "element text is "<<ele.text();

#endif
        //把老的节点替换成新建节点
        root.replaceChild(newNodeTag,ele);
        if_sucess = true;
#ifdef __XMLPARSER_DEBUG__
        qDebug() << "modify ok ! value is "<<value;
#endif
    }

    if(!myfile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
#ifdef __XMLPARSER_DEBUG__
        qDebug() << "open for remove error!";
#endif
    }
    QTextStream out(&myfile);
    doc.save(out,4);
    myfile.close();
    if(if_sucess){
        return if_sucess;
    }else{
        return if_sucess;
    }
    //    return true;

}

bool XMLParser::delete_element(QString filepath ,QString filename,QString element,QString value){
    if(NULL==filepath||NULL==filename||NULL==element||NULL==value){
        return false;
    }


    QString Path = filepath+filename;
    bool if_sucess = false;
    QFile myfile(Path);
    QDomDocument doc;
    QString errorStr;
    int errorLine;
    int errorColumn;

    if (!doc.setContent(&myfile, false, &errorStr, &errorLine, &errorColumn)) {
#ifdef __XMLPARSER_DEBUG__
        qDebug()<<"setcontent error..."<<" errorStr is "<<errorStr<<" errorLine is "<<errorLine<<" errorColumn is "<<errorColumn ;
#endif
        myfile.close();
    }

    myfile.close();

    QDomElement root = doc.documentElement();

    QDomNodeList lists = doc.elementsByTagName(element);

    //新建节点
    QDomElement newNodeTag = doc.createElement(element);
    QDomText newNodeText = doc.createTextNode(value);
    newNodeTag.appendChild(newNodeText);

    int num = 0;
    for(num = 0;num<lists.size();num++){
        QDomElement ele = lists.at(num).toElement();
#ifdef __XMLPARSER_DEBUG__
        qDebug() << "element text is "<<ele.text();
#endif

        //删除节点
        if(ele.text()==value){
            root.removeChild(ele);
            if_sucess = true;
        }
#ifdef __XMLPARSER_DEBUG__
        qDebug() << "modify ok ! value is "<<value;
#endif
    }

    if(!myfile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
#ifdef __XMLPARSER_DEBUG__
        qDebug() << "open for remove error!";
#endif
    }
    QTextStream out(&myfile);
    doc.save(out,4);
    myfile.close();
    if(if_sucess){
        return if_sucess;
    }else{
        return if_sucess;
    }

}


XMLParser::~XMLParser(){
    if(NULL!=file){
        file->close();
        file=NULL;
    }
}
