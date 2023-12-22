#include "my_udp.h"

Udp* Udp::pThis = nullptr;
udp_pcb* Udp::udpLocalPcb = nullptr;
uint8_t Udp::udp_msg_send[255] = {0};
uint8_t Udp::udp_msg_receive[255] = {0};
uint8_t Udp::ip[4] = {0};

Udp::Udp([[maybe_unused]] uint8_t ip0, [[maybe_unused]] uint8_t ip1,
         [[maybe_unused]] uint8_t ip2, [[maybe_unused]] uint8_t ip3) {
    ip[0] = Eth::pThis->IP_ADDRESS[0];
    ip[1] = Eth::pThis->IP_ADDRESS[1];
    ip[2] = Eth::pThis->IP_ADDRESS[2];
    ip[3] = Eth::pThis->IP_ADDRESS[3];
    pThis = this;
    init();
}

void Udp::init() { my_connect(); }

void Udp::my_connect(void) {
    // ip_set_option(upcb, SOF_BROADCAST); IP_ADDR_BROADCAST
    udpLocalPcb = udp_new();
    if (udpLocalPcb != NULL) {
        ip_addr_t srcaddr;
        IP4_ADDR(&srcaddr, ip[0], ip[1], ip[2], ip[3]);
        udp_bind(udpLocalPcb, &srcaddr, PORT_IN);
        udp_recv(udpLocalPcb, receive_Local_callback, NULL);
    }
}

void Udp::disconnect() {
    if (udpLocalPcb) {
        udp_remove(udpLocalPcb);
    }
}

void Udp::receive_Local_callback(void*, [[maybe_unused]] struct udp_pcb* upcb,
                                 struct pbuf* p,
                                 [[maybe_unused]] const ip_addr_t* addr,
                                 [[maybe_unused]] u16_t port) {
    uint16_t pLen = p->len;
    // ip_addr_t received_ip = *addr;
    memcpy(udp_msg_receive, p->payload, pLen);
    // Set current interface to UDP

    Udp::pThis->receiveCommand.currentCommand = Protocol::ERROR;
    if (udp_msg_receive[1] == Protocol::Commands::SEND_SET_SETTINGS) {
        Udp::pThis->receiveCommandSet =
            Protocol::parseFrameSet(udp_msg_receive, pLen);
        Udp::pThis->IsDataSetReceived = true;
    } else {
        Udp::pThis->receiveCommand =
            Protocol::parseFrame(udp_msg_receive, pLen);
        if (Udp::pThis->receiveCommand.currentCommand != Protocol::ERROR) {
            Udp::pThis->IsDataReceived = true;
        }
    }
    pbuf_free(p);
}

void Udp::sendToPC(const uint8_t* buff, uint8_t len) {
    struct pbuf* p;
    p = pbuf_alloc(PBUF_RAW, 256, PBUF_REF);
    p->payload = (uint8_t*)buff;
    p->len = p->tot_len = len;
    // send only from local pcb
    udp_sendto(udpLocalPcb, p, &PC_IP, PC_PORT_IN);
    pbuf_free(p);
}
void Udp::sendReply() {
    uint8_t arr[6] = {Protocol::FROM, Protocol::REPLY_CONTROLLER, 0, 0, 0, 0};
    sendToPC(arr, sizeof(arr));
}
void Udp::sendData(Protocol::Reply reply) {
    uint8_t arr[6] = {
        Protocol::FROM,
        (uint8_t)reply.currentReply,
        (uint8_t)(reply.val >> 24),
        (uint8_t)(reply.val >> 16),
        (uint8_t)(reply.val >> 8),
        (uint8_t)(reply.val),
    };
    sendToPC(arr, sizeof(arr));
}
void Udp::sendDataSet(Protocol::ReplySet replySet) {
    uint8_t arr[] = {
        Protocol::FROM,
        (uint8_t)replySet.currentReply,
        (uint8_t)((*(uint32_t*)&replySet.coefY) >> 24),
        (uint8_t)((*(uint32_t*)&replySet.coefY) >> 16),
        (uint8_t)((*(uint32_t*)&replySet.coefY) >> 8),
        (uint8_t)((*(uint32_t*)&replySet.coefY)),
        (uint8_t)((*(uint32_t*)&replySet.coefX) >> 24),
        (uint8_t)((*(uint32_t*)&replySet.coefX) >> 16),
        (uint8_t)((*(uint32_t*)&replySet.coefX) >> 8),
        (uint8_t)((*(uint32_t*)&replySet.coefX)),
        (uint8_t)(replySet.deviationY >> 24),
        (uint8_t)(replySet.deviationY >> 16),
        (uint8_t)(replySet.deviationY >> 8),
        (uint8_t)(replySet.deviationY),
        (uint8_t)(replySet.deviationX >> 24),
        (uint8_t)(replySet.deviationX >> 16),
        (uint8_t)(replySet.deviationX >> 8),
        (uint8_t)(replySet.deviationX),
    };
    sendToPC(arr, sizeof(arr));
}
