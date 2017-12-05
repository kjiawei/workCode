#include "httpserver.h"
#include "httprequest.h"
#include "httpresponse.h"
#include "httpconnection.h"
#include "common_tr_string.h"
#include "global_var.h"

#include <QTcpServer>

QHash<int, QString> STATUS_CODES;

HttpServer::HttpServer(QObject *parent)
    : QObject(parent)
    , m_tcpServer(0)
{

#define STATUS_CODE(num, reason) STATUS_CODES.insert(num, reason);

  STATUS_CODE(100, "Continue")
  STATUS_CODE(101, "Switching Protocols")
  STATUS_CODE(102, "Processing")                 // RFC 2518) obsoleted by RFC 4918
  STATUS_CODE(200, "OK")
  STATUS_CODE(201, "Created")
  STATUS_CODE(202, "Accepted")
  STATUS_CODE(203, "Non-Authoritative Information")
  STATUS_CODE(204, "No Content")
  STATUS_CODE(205, "Reset Content")
  STATUS_CODE(206, "Partial Content")
  STATUS_CODE(207, "Multi-Status")               // RFC 4918
  STATUS_CODE(300, "Multiple Choices")
  STATUS_CODE(301, "Moved Permanently")
  STATUS_CODE(302, "Moved Temporarily")
  STATUS_CODE(303, "See Other")
  STATUS_CODE(304, "Not Modified")
  STATUS_CODE(305, "Use Proxy")
  STATUS_CODE(307, "Temporary Redirect")
  STATUS_CODE(400, "Bad Request")
  STATUS_CODE(401, "Unauthorized")
  STATUS_CODE(402, "Payment Required")
  STATUS_CODE(403, "Forbidden")
  STATUS_CODE(404, "Not Found")
  STATUS_CODE(405, "Method Not Allowed")
  STATUS_CODE(406, "Not Acceptable")
  STATUS_CODE(407, "Proxy Authentication Required")
  STATUS_CODE(408, "Request Time-out")
  STATUS_CODE(409, "Conflict")
  STATUS_CODE(410, "Gone")
  STATUS_CODE(411, "Length Required")
  STATUS_CODE(412, "Precondition Failed")
  STATUS_CODE(413, "Request Entity Too Large")
  STATUS_CODE(414, "Request-URI Too Large")
  STATUS_CODE(415, "Unsupported Media Type")
  STATUS_CODE(416, "Requested Range Not Satisfiable")
  STATUS_CODE(417, "Expectation Failed")
  STATUS_CODE(418, "I\"m a teapot")              // RFC 2324
  STATUS_CODE(422, "Unprocessable Entity")       // RFC 4918
  STATUS_CODE(423, "Locked")                     // RFC 4918
  STATUS_CODE(424, "Failed Dependency")          // RFC 4918
  STATUS_CODE(425, "Unordered Collection")       // RFC 4918
  STATUS_CODE(426, "Upgrade Required")           // RFC 2817
  STATUS_CODE(500, "Internal Server Error")
  STATUS_CODE(501, "Not Implemented")
  STATUS_CODE(502, "Bad Gateway")
  STATUS_CODE(503, "Service Unavailable")
  STATUS_CODE(504, "Gateway Time-out")
  STATUS_CODE(505, "HTTP Version not supported")
  STATUS_CODE(506, "Variant Also Negotiates")    // RFC 2295
  STATUS_CODE(507, "Insufficient Storage")       // RFC 4918
  STATUS_CODE(509, "Bandwidth Limit Exceeded")
  STATUS_CODE(510, "Not Extended")                // RFC 2774
}

HttpServer::~HttpServer()
{

}

void HttpServer::newConnection()
{
    Q_ASSERT(m_tcpServer);
    while(m_tcpServer->hasPendingConnections()) {
        HttpConnection *connection = new HttpConnection(m_tcpServer->nextPendingConnection(), this);
       // connect(connection, SIGNAL(newRequest(HttpRequest*, HttpResponse*)),
             //   this, SIGNAL(newRequest(HttpRequest*, HttpResponse*)));
		 connect(connection, SIGNAL(newRequest(HttpRequest*, HttpResponse*)),
		   this, SLOT(onRequest(HttpRequest*, HttpResponse*)));
    }
}

bool HttpServer::listen(const QHostAddress &address, quint16 port)
{
    m_tcpServer = new QTcpServer;

    connect(m_tcpServer, SIGNAL(newConnection()), this, SLOT(newConnection()));
    return m_tcpServer->listen(address, port);
}

bool HttpServer::listen(quint16 port)
{
    return listen(QHostAddress::Any, port);
}


void HttpServer::onRequest(HttpRequest* req, HttpResponse* resp)
{
        PacketUF packetUF;
        PacketSubBloodPump SubBPump;
        QTime timeLeave;
        QDateTime timeobj;
        QString strHeaderStatus;
        PacketPDOHeparinPump heparinPDO;
        PacketFluid Fluid;
        FormulaSet formulaSet;
        unsigned short int  Oxygen=0;

        dataRead(PACKET_NAME_UF, &packetUF);
        dataRead(PACKET_PDO_SUBBLOODPUMP, &SubBPump);
        dataRead(PACKET_PDO_HEPARINPUMP,&heparinPDO);
        dataRead(PACKET_NAME_DIALYSATE,&Fluid);
        dataRead(PACKET_FORMULA_TARGET, &formulaSet);
        timeobj.setTime_t(G_SYS_CURTIME);

        switch(workstate.tipState.current)
        {
        case START_INIT: 
            strHeaderStatus="INITSTART";
            break;

        case TREAT_READY:  
            strHeaderStatus="PREPARESTATE";
            break;

        case TREAT_RUNNING: 
            strHeaderStatus="TREATMENTSTATE";
            break;

        case TREAT_FINISH: 
            strHeaderStatus="FINISHTREATMENTSTATE";
            break;

        case CLEANING:   
            strHeaderStatus="CLEANINGSTATE";
            break;

        case DEBUG:
            strHeaderStatus="DEBUGSTATE";
            break;

        case DRAIN:
            strHeaderStatus="DRAINSTATE";
            break;
        }

        if(dataManage->dataCan.monitorBloodState->N5_Oxygen&0x0080)
        {
            Oxygen=0;
        }
        else
        {
            Oxygen=dataManage->dataCan.monitorBloodState->N5_Oxygen&0x007f;
        }

	
//	QString reply  = tr("<html><head><title>Rokh Server Test</title></head><body><h1>hello!</h1></body></html>");
    QString reply=tr("<!DOCTYPE html>   \
                     <html>  \
                     <head>  \
                     <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" /> \
                     <meta http-equiv=\"refresh\" content=\"10\"> \
                     <title>JH-5058 </title> \
                     </head>  \
                     <body> <table width=\"100%\" border=\"1\"><tr> ")+ \
                     "<td>BP Rate="+QString::number(G_SET_BPSPEED)+"ml/min"+\
                     "</td><td>Dialysate Temp="+QString::number(dataManage->dataCan.masterCtrlOutlet->N3_out_temp) +"C"+\
                     "</td><td>UF Goal="+QString::number(packetUF.ufVolAim)+"ml"+\
                      "</td><td>SUB Rate="+QString::number(SubBPump.N10_set_speed)+"ml/min"+\
                      "</td></tr><tr> <td>PA="+ QString::number(dataManage->dataCan.monitorPress->N5_AP-700)+"mmHg"+\
                      "</td><td>PV="+QString::number(dataManage->dataCan.monitorPress->N5_VP-700)+"mmHg"+\
                      "</td><td>TMP="+QString::number(dataManage->dataCan.monitorPress->N5_TMP-700) +"mmHg"+\
                      "</td><td>COND="+QString::number(dataManage->dataCan.masterCtrlOutlet->N3_out_cond)+"mS/cm"+\
                      "</tr><tr><td>Treatment time="+timeLeave.addSecs(total_secCount).toString(QString("hh:mm:ss"))+\
                      "</td><td>Current date="+timeobj.toString("yyyy-MM-dd")+\
                      "</td><td>Current time="+timeobj.toString("hh:mm:ss")+\
                      "</td><td>Current state="+strHeaderStatus+\
                       "</td></tr> <tr><td>&nbsp;</td>"+\
                       "<td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td>"+\
                       "</tr><tr><td>Remaining time="+timeLeave.addSecs(residue_secCount).toString(QString("hh:mm:ss"))+\
                       "</td><td>UF Rate="+QString::number(G_UF_SETRATE)+"ml/h"+\
                       "</td><td>UF Set Value="+QString::number(G_UF_CURETARGET)+"ml"+\
                       "</td><td>UF Finised="+QString::number(UFPump_Cure_Amount) +"ml"+\
                       "</td></tr><tr><td>Na+="+QString::number(formulaSet.tempBuffer.na)+"mmol"+\
                       "</td><td>Heparin Rate="+QString::number(heparinPDO.speed)+"ml/h"+\
                       "</td><td>Heparin Finised="+QString::number(dataManage->dataCan.heparinData->inputAllValue) +"ml"+\
                       "</td><td>Dialysate Rate="+ QString::number(Fluid.Wspeed)+"ml/min"+\
                       "</td></tr><tr><td>HCO3-="+QString::number(formulaSet.tempBuffer.hco3,'g',2)+"mmol"+\
                       "</td><td>Blood T="+ QString::number(37)+"C"+\
                       "</td><td>Blood O="+QString::number(Oxygen)+"%"+\
                       "</td><td>BPM status="+QString::number(dataManage->dataCan.monitorBPMState->N5_BPM_status)+\
                       "</td></tr><tr><td>Pulse="+QString::number(dataManage->dataCan.monitorBPMMisc->N5_BPM_pulse)+\
                       "<td>SYS="+QString::number(dataManage->dataCan.monitorBPMState->N5_BPM_SYS)+"mmHg"+\
                         "</td><td>DIA="+QString::number(dataManage->dataCan.monitorBPMState->N5_BPM_DIA)+"mmHg"+\
                         "</td><td>Blood Volume=--"+\
                         "</td></tr></table></body></html>";
	resp->setHeader("Content-Type", "text/html");
	resp->setStatus(200);
//	resp->end(QString("Test OK!"));
    resp->end(reply);
	return;

}
