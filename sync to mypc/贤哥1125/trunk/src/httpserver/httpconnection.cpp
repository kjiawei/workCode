
#include "httprequest.h"
#include "httpresponse.h"
#include "httpconnection.h"

#include <QTcpSocket>
#include <QHostAddress>

HttpConnection::HttpConnection(QTcpSocket *socket, QObject *parent)
    : QObject(parent)
    , m_socket(socket)
    , m_parser(0)
{
    qDebug() << "新连接：" << socket->peerAddress() << socket->peerPort();

    m_parser = (http_parser*)malloc(sizeof(http_parser));
    http_parser_init(m_parser, HTTP_REQUEST);

    m_parserSettings.on_message_begin = MessageBegin;
    m_parserSettings.on_path = Path;
    m_parserSettings.on_query_string = 0;
    //m_parserSettings.on_query_string = QueryString;
    m_parserSettings.on_url = Url;
    m_parserSettings.on_fragment = Fragment;
    m_parserSettings.on_header_field = HeaderField;
    m_parserSettings.on_header_value = HeaderValue;
    m_parserSettings.on_headers_complete = HeadersComplete;
    m_parserSettings.on_body = Body;
    m_parserSettings.on_message_complete = MessageComplete;

    m_parser->data = this;

    connect(socket, SIGNAL(readyRead()), this, SLOT(parseRequest()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(deleteLater()));
}

HttpConnection::~HttpConnection()
{
    delete m_socket;
    m_socket = 0;

    free(m_parser);
    m_parser = 0;
}

void HttpConnection::parseRequest()
{
    Q_ASSERT(m_parser);

    while(m_socket->bytesAvailable())
    {
        QByteArray arr = m_socket->read(80*1024);

        if( arr.size() < 0 ) {
            // TODO
        }
        else {
            int nparsed = http_parser_execute(m_parser, &m_parserSettings, arr.data(), arr.size());
            if( nparsed != arr.size() ) {
            }
        }
    }
}

void HttpConnection::write(const QByteArray &data)
{
    m_socket->write(data);
}

void HttpConnection::flush()
{
    m_socket->flush();
}

void HttpConnection::responseDone()
{
    HttpResponse *response = qobject_cast<HttpResponse*>(QObject::sender());
    if( response->m_last )
    {
        m_socket->disconnectFromHost();
    }
}

/********************
 * 静态 回调函数 *
 *******************/

int HttpConnection::MessageBegin(http_parser *parser)
{
    HttpConnection *theConnection = (HttpConnection *)parser->data;
    theConnection->m_currentHeaders.clear();
    theConnection->m_request = new HttpRequest(theConnection);
    return 0;
}

int HttpConnection::HeadersComplete(http_parser *parser)
{
    HttpConnection *theConnection = (HttpConnection *)parser->data;
    Q_ASSERT(theConnection->m_request);

    //设置命令
    QString method = QString::fromAscii(http_method_str((enum http_method) parser->method));
    theConnection->m_request->setMethod(method);

    //设置http版本
    theConnection->m_request->setVersion(QString("%1.%2").arg(parser->http_major).arg(parser->http_minor));

    // Insert last remaining header
    theConnection->m_currentHeaders[theConnection->m_currentHeaderField.toLower()] = theConnection->m_currentHeaderValue;

    //设置头
    if( theConnection->m_currentHeaders.contains("host") ) {
        QUrl url = QUrl(theConnection->m_request->url());
        url.setAuthority(theConnection->m_currentHeaders["host"]);
        theConnection->m_request->setUrl(url);
    }
    else {
        // TODO: 中断 with 400
    }
    theConnection->m_request->setHeaders(theConnection->m_currentHeaders);

   //设置客户信息
    theConnection->m_request->m_remoteAddress = theConnection->m_socket->peerAddress().toString();
    theConnection->m_request->m_remotePort = theConnection->m_socket->peerPort();

    HttpResponse *response = new HttpResponse(theConnection);
    if( parser->http_major < 1 || parser->http_minor < 1 )
        response->m_keepAlive = false;

    connect(response, SIGNAL(done()), theConnection, SLOT(responseDone()));

    //头设置好了，提交请求信号
    emit theConnection->newRequest(theConnection->m_request, response);
    return 0;
}

int HttpConnection::MessageComplete(http_parser *parser)
{
    // TODO: do cleanup and prepare for next request
    HttpConnection *theConnection = (HttpConnection *)parser->data;
    Q_ASSERT(theConnection->m_request);

    emit theConnection->m_request->end();
    return 0;
}

int HttpConnection::Path(http_parser *parser, const char *at, size_t length)
{
    HttpConnection *theConnection = (HttpConnection *)parser->data;
    Q_ASSERT(theConnection->m_request);
    QString path = QString::fromAscii(at, length);

    QUrl url = theConnection->m_request->url();
    url.setPath(path);
    theConnection->m_request->setUrl(url);
    return 0;
}

int HttpConnection::QueryString(http_parser *parser, const char *at, size_t length)
{
    HttpConnection *theConnection = (HttpConnection *)parser->data;
    Q_ASSERT(theConnection->m_request);

    Q_ASSERT(false);
    return 0;
}

int HttpConnection::Url(http_parser *parser, const char *at, size_t length)
{
//    qDebug() << "URL CALL" << QString::fromAscii(at, length);
//    HttpConnection *theConnection = (HttpConnection *)parser->data;
//    theConnection->m_request->m_url->setPath(QString::fromAscii(at, length));

    return 0;
}

int HttpConnection::Fragment(http_parser *parser, const char *at, size_t length)
{
    // TODO: 实现必要的操作
    Q_ASSERT(false);
    return 0;
}

int HttpConnection::HeaderField(http_parser *parser, const char *at, size_t length)
{
    HttpConnection *theConnection = (HttpConnection *)parser->data;
    Q_ASSERT(theConnection->m_request);

    // 插入我们刚才已经解析过的头
    // 插入到map中
    if( !theConnection->m_currentHeaderField.isEmpty() && !theConnection->m_currentHeaderValue.isEmpty() )
    {
        // header 的名字必须全部小写
        theConnection->m_currentHeaders[theConnection->m_currentHeaderField.toLower()] = theConnection->m_currentHeaderValue;
		//清空下次备用
        theConnection->m_currentHeaderField = QString();
        theConnection->m_currentHeaderValue = QString();
    }

    QString fieldSuffix = QString::fromAscii(at, length);
    theConnection->m_currentHeaderField += fieldSuffix;
    return 0;
}

int HttpConnection::HeaderValue(http_parser *parser, const char *at, size_t length)
{
    HttpConnection *theConnection = (HttpConnection *)parser->data;
    Q_ASSERT(theConnection->m_request);

    QString valueSuffix = QString::fromAscii(at, length);
    theConnection->m_currentHeaderValue += valueSuffix;
    return 0;
}

int HttpConnection::Body(http_parser *parser, const char *at, size_t length)
{
    HttpConnection *theConnection = (HttpConnection *)parser->data;
    Q_ASSERT(theConnection->m_request);

    emit theConnection->m_request->data(QByteArray(at, length));
    return 0;
}
