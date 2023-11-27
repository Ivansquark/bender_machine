#include "my_udp.h"

Udp* Udp::pThis = nullptr;
udp_pcb* Udp::udpLocalPcb = nullptr;
udp_pcb* Udp::udpBroadcastPcb = nullptr;
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
    udpBroadcastPcb = udp_new();
    // ip_set_option(upcb, SOF_BROADCAST); IP_ADDR_BROADCAST
    if (udpBroadcastPcb != NULL) {
        if (Flash::pThis->IPs[3] == 6) {
            udp_bind(udpBroadcastPcb, IP_ADDR_BROADCAST, EMITTER_PORT_IN1);
            udp_recv(udpBroadcastPcb, receive_BROADCAST_callback, NULL);
        } else {
            udp_bind(udpBroadcastPcb, IP_ADDR_BROADCAST, EMITTER_PORT_IN2);
            udp_recv(udpBroadcastPcb, receive_BROADCAST_callback, NULL);
        }
    }
    udpLocalPcb = udp_new();
    if (udpLocalPcb != NULL) {
        ip_addr_t srcaddr;
        IP4_ADDR(&srcaddr, ip[0], ip[1], ip[2], ip[3]);
        // udp_bind(udpLocalPcb, &srcaddr, EMITTER_PORT_IN);
        //udp_bind(udpLocalPcb, IP_ADDR_ANY, EMITTER_PORT_IN);
        //udp_recv(udpLocalPcb, receive_Local_callback, NULL);
    }
}

void Udp::disconnect() {
    if (udpLocalPcb) { udp_remove(udpLocalPcb); }
    if (udpBroadcastPcb) { udp_remove(udpBroadcastPcb); }
}

void Udp::receive_BROADCAST_callback([[maybe_unused]] void* arg,
                                     [[maybe_unused]] struct udp_pcb* upcb,
                                     struct pbuf* p,
                                     [[maybe_unused]] const ip_addr_t* addr,
                                     [[maybe_unused]] u16_t port) {
    uint16_t pLen = p->len;
    memcpy(Udp::pThis->udp_msg_receive, p->payload, pLen);
    // Net::parseUdpBROADCAST(pLen);
    if (Protocol::getCurrentProtocolInterface() !=
        Protocol::ProtocolInterface::UDP) {
        Protocol::setCurrentInterface(Protocol::ProtocolInterface::UDP);
    }
    Protocol::parseFrame(udp_msg_receive, pLen);
    // p->payload = Udp::pThis->udp_msg_receive;
    // Udp::pThis->udp_msg_receive[0] = '0';
    // p->len = pLen;
    // udp_sendto(udpLocalPcb, p, addr, PC_PORT_IN);
    pbuf_free(p);
}

void Udp::receive_Local_callback(void*,
                                 [[maybe_unused]] struct udp_pcb* upcb,
                                 struct pbuf* p,
                                 [[maybe_unused]] const ip_addr_t* addr,
                                 [[maybe_unused]] u16_t port) {
    uint16_t pLen = p->len;
    // ip_addr_t received_ip = *addr;
    memcpy(udp_msg_receive, p->payload, pLen);
    // Set current interface to UDP
    if (Protocol::getCurrentProtocolInterface() !=
        Protocol::ProtocolInterface::UDP) {
        Protocol::setCurrentInterface(Protocol::ProtocolInterface::UDP);
    }
    Protocol::parseFrame(udp_msg_receive, pLen);
    pbuf_free(p);
}

void Udp::send_BROADCAST(const uint8_t* buff, uint8_t len) {
    struct pbuf* p;
    p = pbuf_alloc(PBUF_RAW, 256, PBUF_REF);
    p->payload = (uint8_t*)buff;
    p->len = p->tot_len = len;
    // send only from local pcb
    udp_sendto(udpLocalPcb, p, IP_ADDR_BROADCAST, EMITTER_PORT_OUT);
    pbuf_free(p);
}
void Udp::sendToControl(const uint8_t* buff, uint8_t len) {
    struct pbuf* p;
    p = pbuf_alloc(PBUF_RAW, 256, PBUF_REF);
    p->payload = (uint8_t*)buff;
    p->len = p->tot_len = len;
    // send only from local pcb
    udp_sendto(udpLocalPcb, p, &control_IP, EMITTER_PORT_OUT);
    pbuf_free(p);
}

void Udp::sendToPC(const uint8_t* buff, uint8_t len) {
    struct pbuf* p;
    p = pbuf_alloc(PBUF_RAW, 256, PBUF_REF);
    p->payload = (uint8_t*)buff;
    p->len = p->tot_len = len;
    // send only from local pcb
    udp_sendto(udpLocalPcb, p, &PC_IP, EMITTER_PORT_OUT);
    pbuf_free(p);
}

void Udp::setNewIP(uint8_t ip0, uint8_t ip1, uint8_t ip2, uint8_t ip3) {
    disconnect();
    ip[0] = ip0, ip[1] = ip1, ip[2] = ip2, ip[3] = ip3;
    /*
    //not working
    if (ip[3] == 6) {
        Eth::pThis->MY_MAC[0] = 0x66;
        Eth::pThis->MY_MAC[1] = 0x66;
        Eth::pThis->MY_MAC[2] = 0x66;
        Eth::pThis->MY_MAC[3] = 0x66;
        Eth::pThis->MY_MAC[4] = 0x66;
        Eth::pThis->MY_MAC[5] = 0x66;
    } else {
        Eth::pThis->MY_MAC[0] = 0x77;
        Eth::pThis->MY_MAC[1] = 0x77;
        Eth::pThis->MY_MAC[2] = 0x77;
        Eth::pThis->MY_MAC[3] = 0x77;
        Eth::pThis->MY_MAC[4] = 0x77;
        Eth::pThis->MY_MAC[5] = 0x77;
    }
    */
    Flash::pThis->writeIPs(ip[0], ip[1], ip[2], ip[3]);
    Eth::pThis->init_lwip(ip0, ip1, ip2, ip3, false);
    my_connect();
}
