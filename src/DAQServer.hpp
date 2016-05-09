#ifndef DAQWebSocketServer_hpp
#define DAQWebSocketServer_hpp
#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/WebSocket.h"
#include "Poco/Net/NetException.h"
#include "Poco/File.h"
#include "Poco/URI.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/Format.h"
#include "Poco/Timespan.h"
#include "Poco/UTF8String.h"

#include <iostream>
#include <stdio.h>
#include <vector>
#include <unordered_map>
#include <string>
#include <memory>

using Poco::Net::ServerSocket;
using Poco::Net::WebSocket;
using Poco::Net::WebSocketException;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPServerResponse;
using Poco::Net::HTTPServerParams;
using Poco::Timestamp;
using Poco::ThreadPool;
using Poco::Util::ServerApplication;
using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;
using Poco::URI;
using Poco::Path;
using Poco::icompare;
using Poco::UTF8;
using namespace std;


class PageRequestHandler: public HTTPRequestHandler
{
public:
    string mimeType(const Path& path);
    void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response);
   };

class WebSocketRequestHandler: public HTTPRequestHandler
{
public:
    void handleRequest              (HTTPServerRequest& request, HTTPServerResponse& response);
    void handleSNRRequest           (HTTPServerRequest& request, HTTPServerResponse& response);
    void handleBERRequest           (HTTPServerRequest& request, HTTPServerResponse& response);
    void handleRSSIRequest          (HTTPServerRequest& request, HTTPServerResponse& response);
    void handleConstellationRequest (HTTPServerRequest& request, HTTPServerResponse& response);
    void handleCAQRequest           (HTTPServerRequest& request, HTTPServerResponse& response);

};

class RequestHandlerFactory: public HTTPRequestHandlerFactory
{
public:
    HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request);
};

class WebSocketServer: public Poco::Util::ServerApplication
{
public:
    WebSocketServer(){}
    ~WebSocketServer(){}

protected:
    void initialize(Application& self);
    void uninitialize();
    void defineOptions(OptionSet& options);
    void handleOption(const std::string& name, const std::string& value);
    int  main(const std::vector<std::string>& args);

};

#define URL_GET_SNR                 "/snr"
#define URL_GET_RSSI                "/rssi"
#define URL_GET_CONSTELLATION       "/sd"
#define URL_GET_BER                 "/ber"
#define URL_GET_3D_PLOT_DATA        "/3dplot"

#endif /* DAQWebSocketServer_hpp */
