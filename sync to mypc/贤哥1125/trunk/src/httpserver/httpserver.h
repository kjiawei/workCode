
#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#define HTTPSERVER_VERSION_MAJOR 0
#define HTTPSERVER_VERSION_MINOR 1
#define HTTPSERVER_VERSION_PATCH 0

#include <QObject>
#include <QHostAddress>

class QTcpServer;
class HttpRequest;
class HttpResponse;

//请求和响应头的表
typedef QHash<QString, QString> HeaderHash;

//状态代码
extern QHash<int, QString> STATUS_CODES;


class HttpServer : public QObject
{
	Q_OBJECT
public:
	HttpServer(QObject *parent = 0);
	virtual ~HttpServer();
	bool listen(const QHostAddress &address = QHostAddress::Any, quint16 port=0);
	bool listen(quint16 port);

signals:
	//void newRequest(QHttpRequest *request, QHttpResponse *response);

private slots:
	void newConnection();
	void onRequest(HttpRequest*, HttpResponse*);

private:
	QTcpServer *m_tcpServer;
};

#endif
