#include "roomwindow.h"
#include "ui_roomwindow.h"

RoomWindow::RoomWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RoomWindow)
{
    ui->setupUi(this);
    ui->txtInput->setFocus();
}

RoomWindow::~RoomWindow()
{
    delete ui;
}

void RoomWindow::on_btnSend_clicked()
{
    emit enteredText(ui->txtInput->text());
    //ui->txtChat->append("Me: " + ui->txtInput->text()); - Your input
    ui->txtInput->clear();
    ui->txtInput->setFocus();
}

void RoomWindow::receivedPM(QString from, QString text)
{
    QString f = from.count()>1? from + " : " : "";
    qDebug() << text;
    int pos;
    if ((pos = text.indexOf(":")) != -1) {
        QString begin, lessText;
        bool flag = false;
        while (0 < text.count(":")) {
            if ((pos = text.indexOf(":")) != -1) {
                //qDebug() << pos;
                begin = text.mid(0, pos);
                lessText = text.mid(pos + 2, text.length() - pos - 2);
                if (!flag) {
                    ui->txtChat->append(f + begin);
                    flag = true;
                }
                else {
                    ui->txtChat->moveCursor(QTextCursor::End);
                    ui->txtChat->insertPlainText(begin);
                }
                switch(text.at(pos + 1).toLatin1()) {
                case ')': {
                    addSmile("C://Users/Dan/Desktop/Lan-Messenger-master/Lan-Messenger/resources/smile.jpg");
                    break;
                }
                case '(': {
                    addSmile("C://Users/Dan/Desktop/Lan-Messenger-master/Lan-Messenger/resources/kovalchuk.jpg");
                    break;
                }
                //case '3':
                case '*': {
                    addSmile("C://Users/Dan/Desktop/Lan-Messenger-master/Lan-Messenger/resources/lovelick.jpg");
                    break;
                }
                default: {
                    ui->txtChat->moveCursor(QTextCursor::End);
                    QString str = QString(":") + text.at(pos + 1);
                    ui->txtChat->insertPlainText(str);
                    break;
                }
                }

                //ui->txtChat->moveCursor(QTextCursor::End);
                //ui->txtChat->insertPlainText(lessText);
                text = lessText;
                qDebug() << text;
            } else {
                ui->txtChat->append(f + text);
            }
        }
        ui->txtChat->moveCursor(QTextCursor::End);
        ui->txtChat->insertPlainText(lessText);
    }
    else {
        ui->txtChat->append(f + text);
    }
}

void RoomWindow::addSmile(QString path)
{
    QUrl Uri ( QString ( "file://%1" ).arg ( path ) );
    QImage image = QImageReader (path).read();
    QTextDocument * textDocument = ui->txtChat->document();
    textDocument->addResource( QTextDocument::ImageResource, Uri, QVariant ( image ) );
    QTextCursor cursor = ui->txtChat->textCursor();
    QTextImageFormat imageFormat;
    imageFormat.setWidth( image.width() / 8);
    imageFormat.setHeight( image.height() / 8);
    imageFormat.setName( Uri.toString() );
    cursor.insertImage(imageFormat);
}

void RoomWindow::on_txtInput_returnPressed()
{
    on_btnSend_clicked();
}

void RoomWindow::closeEvent(QCloseEvent *event)
{
    emit closedWindow();
    event->accept();
    this->deleteLater();
}

void RoomWindow::updateList(QList<QString> list)
{
    ui->listUsers->clear();
    for(int i=0; i<list.count(); i++)
        ui->listUsers->addItem(list[i]);
}


void RoomWindow::on_listUsers_doubleClicked(const QModelIndex &index)
{
    QString userid = ui->listUsers->currentItem()->text();
    emit startPM(userid);
}
