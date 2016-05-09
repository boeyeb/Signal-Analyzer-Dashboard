#include "DAQServer.hpp"
#include <sstream>
#include <cstdlib>
#include <random>

using namespace std;
using Poco::Timespan;
using Poco::TimeoutException;
using Poco::trim;

#define PKT_RECEIVE_TIMEOUT 5000000
#define TX_PKT_SIZE 16
#define TX_PKT_SIZE2 64

void WebSocketRequestHandler::handleSNRRequest(HTTPServerRequest& request, HTTPServerResponse& response)
{
    Application& app = Application::instance();

    try
    {
        WebSocket ws(request, response);

        char rxPkt[TX_PKT_SIZE];
        char txPkt[TX_PKT_SIZE*2];
        int flags;
        int n = ws.receiveFrame(rxPkt, sizeof(rxPkt), flags);
        string pktStr = rxPkt;
        app.logger().information(Poco::format("%s flags=0x%x", pktStr, unsigned(flags)));
        int tick = 1;
        do
        {
            std::random_device rd;
            std::mt19937 eng(rd());
            std::uniform_real_distribution<> unif(0.0, 40.0);
            memset(txPkt,0,TX_PKT_SIZE*2);
            sprintf(txPkt,"t:%d,1:%.2f,2:%.2f", tick++, unif(eng), unif(eng));
            ws.sendFrame(txPkt, sizeof(txPkt), flags);
            usleep(1000000);
        }
        while (n > 0 || (flags & WebSocket::FRAME_OP_BITMASK) != WebSocket::FRAME_OP_CLOSE);
        app.logger().information("SNRSocket closed.");
    }

    catch (WebSocketException& exc)
    {
        app.logger().log(exc);
        switch (exc.code())
        {
            case WebSocket::WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION:
                response.set("Sec-WebSocket-Version", WebSocket::WEBSOCKET_VERSION);
            case WebSocket::WS_ERR_NO_HANDSHAKE:
            case WebSocket::WS_ERR_HANDSHAKE_NO_VERSION:
            case WebSocket::WS_ERR_HANDSHAKE_NO_KEY:
                response.setStatusAndReason(HTTPResponse::HTTP_BAD_REQUEST);
                response.setContentLength(0);
                response.send();
                break;
        }
    }
}

void WebSocketRequestHandler::handleBERRequest(HTTPServerRequest& request, HTTPServerResponse& response)
{
    Application& app = Application::instance();
    try
    {
        WebSocket ws(request, response);

        char rxPkt[TX_PKT_SIZE];
        char txPkt[TX_PKT_SIZE*2];
        int flags;
        int n = ws.receiveFrame(rxPkt, sizeof(rxPkt), flags);
        string pktStr = rxPkt;
        app.logger().information(Poco::format("%s flags=0x%x", pktStr, unsigned(flags)));
        do
        {
            std::random_device rd;
            std::mt19937 eng(rd());
            std::uniform_real_distribution<> unif(0, 1);

            double ber0 = unif(eng)/10000;
            double ber1 = unif(eng)/10000;

            std::stringstream ber;
            ber<<std::scientific;
            //ber<<std::setprecision(6);
            ber << ber0 << "," << ber1;

            memset(txPkt,0,TX_PKT_SIZE*2);
            sprintf(txPkt,"%s", ber.str().c_str());
            ws.sendFrame(txPkt, sizeof(txPkt), flags);
            usleep(1000000);
        }
        while (n > 0 || (flags & WebSocket::FRAME_OP_BITMASK) != WebSocket::FRAME_OP_CLOSE);
        app.logger().information("BERSocket closed.");
    }

    catch (WebSocketException& exc)
    {
        app.logger().log(exc);
        switch (exc.code())
        {
            case WebSocket::WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION:
                response.set("Sec-WebSocket-Version", WebSocket::WEBSOCKET_VERSION);
            case WebSocket::WS_ERR_NO_HANDSHAKE:
            case WebSocket::WS_ERR_HANDSHAKE_NO_VERSION:
            case WebSocket::WS_ERR_HANDSHAKE_NO_KEY:
                response.setStatusAndReason(HTTPResponse::HTTP_BAD_REQUEST);
                response.setContentLength(0);
                response.send();
                break;
        }
    }
}

void WebSocketRequestHandler::handleRSSIRequest(HTTPServerRequest& request, HTTPServerResponse& response)
{
    Application& app = Application::instance();

    try
    {
        WebSocket ws(request, response);

        char rxPkt[TX_PKT_SIZE];
        char txPkt[TX_PKT_SIZE*2];
        int flags;
        int n = ws.receiveFrame(rxPkt, sizeof(rxPkt), flags);
        string pktStr = rxPkt;
        app.logger().information(Poco::format("%s flags=0x%x", pktStr, unsigned(flags)));
        do
        {
            int rssi[2] = {-1, -1};
            std::random_device rd;
            std::mt19937 eng(rd());
            std::uniform_int_distribution<int> unii(0, 50);

            rssi[0] = unii(eng);
            rssi[1] = unii(eng);
            std::stringstream ss;
            ss << rssi[0] << "," << rssi[1];

            memset(txPkt,0,TX_PKT_SIZE*2);
            sprintf(txPkt,"%s", ss.str().c_str());
            ws.sendFrame(txPkt, sizeof(txPkt), flags);
            usleep(1000000);
        }
        while (n > 0 || (flags & WebSocket::FRAME_OP_BITMASK) != WebSocket::FRAME_OP_CLOSE);
        app.logger().information("RSSISocket closed.");
    }

    catch (WebSocketException& exc)
    {
        app.logger().log(exc);
        switch (exc.code())
        {
            case WebSocket::WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION:
                response.set("Sec-WebSocket-Version", WebSocket::WEBSOCKET_VERSION);
            case WebSocket::WS_ERR_NO_HANDSHAKE:
            case WebSocket::WS_ERR_HANDSHAKE_NO_VERSION:
            case WebSocket::WS_ERR_HANDSHAKE_NO_KEY:
                response.setStatusAndReason(HTTPResponse::HTTP_BAD_REQUEST);
                response.setContentLength(0);
                response.send();
                break;
        }
    }
}

void WebSocketRequestHandler::handleConstellationRequest(HTTPServerRequest& request, HTTPServerResponse& response)
{
    Application& app = Application::instance();
    try
    {
        WebSocket ws(request, response);
        char rxPkt[TX_PKT_SIZE];
        char txPkt[TX_PKT_SIZE2];
        int flags;
        int n = ws.receiveFrame(rxPkt, sizeof(rxPkt), flags);
        string pktStr = rxPkt;
        app.logger().information(Poco::format("%s flags=0x%x", pktStr, unsigned(flags)));
        Timespan sdtimeout(PKT_RECEIVE_TIMEOUT);
        ws.setReceiveTimeout(sdtimeout);
        int iQam = 0, nQam = 0;

        do
        {
            const int nstream = 2;
            int qam0[9] = {4,8,16,32,64,128,256,512,1024};
            int qam1[9] = {4,8,16,32,64,128,256,512,1024};
            const int nsample = 2000;
            nQam = iQam/10;
            std::random_device rd;
            std::mt19937 eng(rd());
            std::uniform_int_distribution<int> unii(-2048, 2048);

            memset(txPkt,0,TX_PKT_SIZE2);
            sprintf(txPkt,"HDR=%d,%d,%d,%d,%d,%d,", nstream, qam0[nQam], qam1[nQam], 0, 0, nsample);
            n = ws.sendFrame(txPkt, sizeof(txPkt), flags);

            if(n > 0 || (flags & WebSocket::FRAME_OP_BITMASK) != WebSocket::FRAME_OP_CLOSE)
            {
                char txBuffer[1024*TX_PKT_SIZE2];
                memset(txBuffer, 0 , 1024*TX_PKT_SIZE2);
                int l = sprintf(txBuffer,"s1=");
                for(int i= 0; i< nsample; i++)
                {
                    l += sprintf(txBuffer+l,"%d,%d,", unii(eng),unii(eng));
                }
                n = ws.sendFrame(txBuffer, l, flags);
                app.logger().information(Poco::format("/sd: sent=%d, flags=0x%x", n, unsigned(flags)));
            }
            else {
                break;
            }

            if(n > 0 || (flags & WebSocket::FRAME_OP_BITMASK) != WebSocket::FRAME_OP_CLOSE)
            {
                char txBuffer[1024*TX_PKT_SIZE2];
                memset(txBuffer, 0 , 1024*TX_PKT_SIZE2);
                int l = sprintf(txBuffer,"s2=");
                for(int i= 0; i< nsample; i++)
                {
                    l += sprintf(txBuffer+l,"%d,%d,", unii(eng),unii(eng));
                }
                n = ws.sendFrame(txBuffer, l, flags);
                app.logger().information(Poco::format("/sd: sent=%d, flags=0x%x", n, unsigned(flags)));
            }
            else {
                break;
            }

            try{
                ws.receiveFrame(rxPkt, sizeof(rxPkt), flags);
            }
            catch (TimeoutException& texc){
                app.logger().log(texc);
                break;
            }
            iQam+=2;
            if(iQam == 90){
                iQam = 0;
            }
        }
        while (n > 0 || (flags & WebSocket::FRAME_OP_BITMASK) != WebSocket::FRAME_OP_CLOSE);
        app.logger().information("ConstellationSocket closed.");
    }

    catch (WebSocketException& exc)
    {
        app.logger().log(exc);
        switch (exc.code())
        {
            case WebSocket::WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION:
                response.set("Sec-WebSocket-Version", WebSocket::WEBSOCKET_VERSION);
                // fallthrough
            case WebSocket::WS_ERR_NO_HANDSHAKE:
            case WebSocket::WS_ERR_HANDSHAKE_NO_VERSION:
            case WebSocket::WS_ERR_HANDSHAKE_NO_KEY:
                response.setStatusAndReason(HTTPResponse::HTTP_BAD_REQUEST);
                response.setContentLength(0);
                response.send();
                break;
        }
    }

}

void WebSocketRequestHandler::handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
{
    Application& app = Application::instance();

    URI uri(request.getURI());
    string uri_path(uri.getPath());
    uri_path = Path::expand(uri_path);

    app.logger().information(uri_path);

    if(Poco::icompare(uri_path, URL_GET_SNR) == 0)
        handleSNRRequest(request,response);
    if(Poco::icompare(uri_path, URL_GET_BER) == 0)
        handleBERRequest(request,response);
    if(Poco::icompare(uri_path, URL_GET_RSSI) == 0)
         handleRSSIRequest(request,response);
    if(Poco::icompare(uri_path, URL_GET_CONSTELLATION) == 0)
        handleConstellationRequest(request,response);
}

string  PageRequestHandler::mimeType(const Path& path)
{
    string ext(UTF8::toLower(path.getExtension())), mime;

    if ((ext == "htm") || (ext == "html") || (ext == "xhtm") || (ext == "xhtml") || ext.empty()) {
        mime = "text/html";
    } else if (ext == "css") {
        mime = "text/css";
    } else if (ext == "js") {
        mime = "text/javascript";
    } else if (ext == "png") {
        mime = "image/png";
    } else if (ext == "gif") {
        mime = "image/gif";
    } else if ((ext == "jpg") || (ext == "jpeg")) {
        mime = "image/jpeg";
    } else if ((ext == "tif") || (ext == "tiff")) {
        mime = "image/tiff";
    } else if (ext == "json") {
        mime = "application/json";
    } else if (ext == "xml") {
        mime = "application/xml";
    } else if (ext == "ico") {
        mime = "image/vnd.microsoft.icon";
    } else {
        mime = "application/octet-stream";
    }

    return mime;
}

void PageRequestHandler::handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
{
    URI uri(request.getURI());
    string uri_path(uri.getPath());
    uri_path = Path::expand(uri_path);
    Application& app = Application::instance();
    string mime = mimeType(uri_path);
    string webroot("../../FrontDeck");

    if(Poco::icompare(mime,"text/html") == 0 && Poco::icompare(uri_path,"/") == 0)
        uri_path = webroot + "/index.html";
    else
        uri_path = webroot + uri_path;

    response.sendFile(uri_path, mime);
    app.logger().information(Poco::format("path=%s, mime=%s", uri_path, mime));
    return;
}

HTTPRequestHandler*  RequestHandlerFactory::createRequestHandler(const HTTPServerRequest& request)
{
    if(request.find("Upgrade") != request.end() && Poco::icompare(request["Upgrade"], "websocket") == 0)
        return new WebSocketRequestHandler();
    else
        return new PageRequestHandler;
}


void WebSocketServer::initialize(Application& self)
{
    loadConfiguration();
    ServerApplication::initialize(self);
}

void WebSocketServer::uninitialize()
{
    ServerApplication::uninitialize();
}

void WebSocketServer::defineOptions(OptionSet& options)
{
    ServerApplication::defineOptions(options);

    options.addOption(
                      Option("help", "h", "display help information on command line arguments")
                      .required(false)
                      .repeatable(false));
}

void  WebSocketServer::handleOption(const string& name, const string& value)
{
    ServerApplication::handleOption(name, value);
}

int WebSocketServer::main(const vector<string>& args)
{
    unsigned short port = (unsigned short) config().getInt("WebSocketServer.port", 9980);
    ServerSocket svs(port);
    HTTPServerParams::Ptr params = new HTTPServerParams;
    params->setMaxQueued(100);
    params->setMaxThreads(100);
    params->setKeepAlive(true);

    HTTPServer srv(new RequestHandlerFactory, svs, params);

    srv.start();
    waitForTerminationRequest();
    srv.stop();
    return Application::EXIT_OK;
}


