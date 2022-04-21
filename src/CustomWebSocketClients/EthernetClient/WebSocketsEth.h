#ifndef WEBSOCKETSETH_H_
#define WEBSOCKETSETH_H_

#include <Arduino.h>

#include "WebSockets.h"

#include <functional>

#include "WebSocketsVersion.h"


#include "Ethernet.h"
#include <SPI.h>

#endif

class WSclient_t_Eth : public WSclient_t {
  public:
    void init(uint8_t num,
        uint32_t pingInterval,
        uint32_t pongTimeout,
        uint8_t disconnectTimeoutCount) {
        this->num                    = num;
        this->pingInterval           = pingInterval;
        this->pongTimeout            = pongTimeout;
        this->disconnectTimeoutCount = disconnectTimeoutCount;
    }

  public:
    uint8_t num = 0;    ///< connection number

    WSclientsStatus_t status = WSC_NOT_CONNECTED;

    EthernetClient * tcp = nullptr;

    bool isSocketIO = false;    ///< client for socket.io server

#if defined(HAS_SSL)
    bool isSSL = false;    ///< run in ssl mode
    WEBSOCKETS_NETWORK_SSL_CLASS * ssl;
#endif

    String cUrl;           ///< http url
    uint16_t cCode = 0;    ///< http code

    bool cIsClient    = false;    ///< will be used for masking
    bool cIsUpgrade   = false;    ///< Connection == Upgrade
    bool cIsWebsocket = false;    ///< Upgrade == websocket

    String cSessionId;        ///< client Set-Cookie (session id)
    String cKey;              ///< client Sec-WebSocket-Key
    String cAccept;           ///< client Sec-WebSocket-Accept
    String cProtocol;         ///< client Sec-WebSocket-Protocol
    String cExtensions;       ///< client Sec-WebSocket-Extensions
    uint16_t cVersion = 0;    ///< client Sec-WebSocket-Version

    uint8_t cWsRXsize = 0;                            ///< State of the RX
    uint8_t cWsHeader[WEBSOCKETS_MAX_HEADER_SIZE];    ///< RX WS Message buffer
    WSMessageHeader_t cWsHeaderDecode;

    String base64Authorization;    ///< Base64 encoded Auth request
    String plainAuthorization;     ///< Base64 encoded Auth request

    String extraHeaders;

    bool cHttpHeadersValid = false;    ///< non-websocket http header validity indicator
    size_t cMandatoryHeadersCount;     ///< non-websocket mandatory http headers present count

    bool pongReceived              = false;
    uint32_t pingInterval          = 0;    // how often ping will be sent, 0 means "heartbeat is not active"
    uint32_t lastPing              = 0;    // millis when last pong has been received
    uint32_t pongTimeout           = 0;    // interval in millis after which pong is considered to timeout
    uint8_t disconnectTimeoutCount = 0;    // after how many subsequent pong timeouts discconnect will happen, 0 means "do not disconnect"
    uint8_t pongTimeoutCount       = 0;    // current pong timeout count

#if(WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266_ASYNC)
    String cHttpLine;    ///< HTTP header lines
#endif

  public:
    uint8_t getNum() {
        return this->num;
    }    ///< connection number

    void setNum(uint8_t num) {
        this->num = num;
    }    ///< connection number

    WSclientsStatus_t getStatus() {
        return this->status;
    }

    void setStatus(WSclientsStatus_t status) {
        this->status = status;
    }

    EthernetClient * getTcp() {
        return this->tcp;
    }

    void setTcp(EthernetClient * tcp) {
        this->tcp = tcp;
    }

    bool getIsSocketIO() {
        return this->isSocketIO;
    }    ///< client for socket.io server

    void setIsSocketIO(bool socketIO) {
        this->isSocketIO = socketIO;
    }    ///< client for socket.io server

    String getCUrl() {
        return this->cUrl;
    }    ///< http url
    void setCUrl(String cUrl) {
        this->cUrl = cUrl;
    }    ///< http url

    uint16_t getCCode() {
        return this->cCode;
    }    ///< http code
    void setCCode(uint16_t cCode) {
        this->cCode = cCode;
    }    ///< http code

    bool getCIsClient() {
        return this->cIsClient;
    }    ///< will be used for masking

    void setCIsClient(bool cIsClient) {
        this->cIsClient = cIsClient;
    }    ///< will be used for masking

    bool getCIsUpgrade() {
        return this->cIsUpgrade;
    }    ///< Connection == Upgrade
    void setCIsUpgrade(bool cIsUpgrade) {
        this->cIsUpgrade = cIsUpgrade;
    }    ///< Connection == Upgrade

    bool getCIsWebsocket() {
        return this->cIsWebsocket;
    }    ///< Upgrade == websocket
    void setCIsWebsocket(bool cIsWebsocket) {
        this->cIsWebsocket = cIsWebsocket;
    }    ///< Upgrade == websocket

    String getCSessionId() {
        return this->cSessionId;
    }
    void setCSessionId(String cSessionId) {
        this->cSessionId = cSessionId;
    }

    String & getCKey() {
        return this->cKey;
    }
    void setCKey(String cKey) {
        this->cKey = cKey;
    }

    String getCAccept() {
        return this->cAccept;
    }
    void setCAccept(String cAccept) {
        this->cAccept = cAccept;
    }

    String getCProtocol() {
        return this->cProtocol;
    }
    void setCProtocol(String cProtocol) {
        this->cProtocol = cProtocol;
    }

    String getCExtensions() {
        return this->cExtensions;
    }
    void setCExtensions(String cExtensions) {
        this->cExtensions = cExtensions;
    }

    uint16_t getCVersion() {
        return this->cVersion;
    }
    void setCVersion(uint16_t cVersion) {
        this->cVersion = cVersion;
    }

    uint8_t getCWsRXsize() {
        return this->cWsRXsize;
    }
    void setCWsRXsize(uint8_t cWsRXsize) {
        this->cWsRXsize = cWsRXsize;
    }

    uint8_t * getCWsHeader() {
        return this->cWsHeader;
    }

    // uint8_t cWsHeader[WEBSOCKETS_MAX_HEADER_SIZE];
    void setCWsHeader(uint8_t * cWsHeader) {
        // this->cWsHeader = cWsHeader;
        for(int i = 0; i < WEBSOCKETS_MAX_HEADER_SIZE; i++) {
            this->cWsHeader[i] = cWsHeader[i];
        }
    }

    WSMessageHeader_t * getCWsHeaderDecode() {
        return &(this->cWsHeaderDecode);
    }
    void setCWsHeaderDecode(WSMessageHeader_t cWsHeaderDecode) {
        this->cWsHeaderDecode = cWsHeaderDecode;
    }

    String getBase64Authorization() {
        return this->base64Authorization;
    }
    void setBase64Authorization(String base64Authorization) {
        this->base64Authorization = base64Authorization;
    }

    String getPlainAuthorization() {
        return this->plainAuthorization;
    }
    void setPlainAuthorization(String plainAuthorization) {
        this->plainAuthorization = plainAuthorization;
    }

    String getExtraHeaders() {
        return this->extraHeaders;
    }
    void setExtraHeaders(String extraHeaders) {
        this->extraHeaders = extraHeaders;
    }

    bool getCHttpHeadersValid() {
        return this->cHttpHeadersValid;
    }
    void setCHttpHeadersValid(bool cHttpHeadersValid) {
        this->cHttpHeadersValid = cHttpHeadersValid;
    }

    size_t getCMandatoryHeadersCount() {
        return this->cMandatoryHeadersCount;
    }
    void setCMandatoryHeadersCount(size_t cMandatoryHeadersCount) {
        this->cMandatoryHeadersCount = cMandatoryHeadersCount;
    }

    bool getPongReceived() {
        return this->pongReceived;
    }
    void setPongReceived(bool pongReceived) {
        this->pongReceived = pongReceived;
    }

    uint32_t getPingInterval() {
        return this->pingInterval;
    }
    void setPingInterval(uint32_t pingInterval) {
        this->pingInterval = pingInterval;
    }

    uint32_t getLastPing() {
        return this->lastPing;
    }
    void setLastPing(uint32_t lastPing) {
        this->lastPing = lastPing;
    }

    uint32_t getPongTimeout() {
        return this->lastPing;
    }
    void setPongTimeout(uint32_t pongTimeout) {
        this->pongTimeout = pongTimeout;
    }

    uint8_t getDisconnectTimeoutCount() {
        return this->disconnectTimeoutCount;
    }
    void setDisconnectTimeoutCount(uint8_t disconnectTimeoutCount) {
        this->disconnectTimeoutCount = disconnectTimeoutCount;
    }
    uint8_t getPongTimeoutCount() {
        return this->pongTimeoutCount;
    }
    void setPongTimeoutCount(uint8_t pongTimeoutCount) {
        this->pongTimeoutCount = pongTimeoutCount;
    }

#if(WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266_ASYNC)
    String getCHttpLine() {
        return this->cHttpLine;
    }
    void setCHttpLine(String cHttpLine) {
        this->cHttpLine = cHttpLine;
    }
#endif
};

#ifndef UNUSED
#define UNUSED(var) (void)(var)
#endif
//#endif /* WEBSOCKETS_H_ */
