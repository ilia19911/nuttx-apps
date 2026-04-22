#include "protocol.hxx"
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <cstdio>
#include <cstring>
#include <errno.h>

static const uint16_t PREF = 0x0123;

static uint8_t crc8(const uint8_t* data, size_t len) {
    uint8_t crc = 0;
    for (size_t i = 0; i < len; ++i) crc += data[i];
    return crc;
}

SimpleProtocol::SimpleProtocol(const char* devicePath)
    : m_fd(-1), m_device(devicePath), m_isOpen(false), m_callback(nullptr), m_userdata(nullptr) {}

SimpleProtocol::~SimpleProtocol() { close(); }

bool SimpleProtocol::open() {
    if (m_isOpen) return true;
    m_fd = ::open(m_device, O_RDWR | O_NOCTTY );
    if (m_fd < 0) {
        perror("open");
        return false;
    }

    struct termios tty;
    if (tcgetattr(m_fd, &tty) != 0) {
        perror("tcgetattr");
        close();
        return false;
    }

    cfsetospeed(&tty, B115200);
    cfsetispeed(&tty, B115200);
    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;
    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_iflag &= ~(INLCR | ICRNL | IGNCR);
    tty.c_oflag &= ~OPOST;
    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 1;

    if (tcsetattr(m_fd, TCSANOW, &tty) != 0) {
        perror("tcsetattr");
        close();
        return false;
    }

    m_isOpen = true;
    return true;
}

void SimpleProtocol::close() {
    if (m_fd >= 0) {
        ::close(m_fd);
        m_fd = -1;
    }
    m_isOpen = false;
}

bool SimpleProtocol::writePacket(uint16_t type, const uint8_t* data, size_t len) {
    if (!m_isOpen || len > 253) return false;

    uint8_t packet[2 + 2 + 2 + len + 4];
    memcpy(packet, &PREF, 2);
    memcpy(packet + 2, &type, 2);
    memcpy(packet + 4, &len, 2);
    memcpy(packet + 6, data, len);
    memcpy(packet + 6 + len, data, 4);//todo поменять на crc
    ssize_t written = ::write(m_fd, packet, sizeof(packet));
    return (written == static_cast<ssize_t>(sizeof(packet)));
}

bool SimpleProtocol::read_pref() {

    uint8_t byte;
    int ret = ::read(m_fd, &byte, 1);

    if (ret <= 0 || byte != (uint8_t)(PREF & 0xFF)) {
        return false;
    }

    ret = ::read(m_fd, &byte, 1);
    if (ret <= 0 || byte != (uint8_t)((PREF >> 8) & 0xFF)) {
        return false;
    }

    return true;
}
bool SimpleProtocol::readPacket(uint16_t& type, uint8_t* data, size_t& len, size_t maxLen) {
    if (!m_isOpen) return false;

    if (!read_pref()) {
        return false;
    }

    uint8_t header[4]; // type (2) + len (2)
    size_t bytesRead = 0;

    // читаем заголовок полностью
    while (bytesRead < sizeof(header)) {
        ssize_t n = ::read(m_fd, header + bytesRead, sizeof(header) - bytesRead);
        if (n <= 0) {
            return false;
        }
        bytesRead += n;
    }

    uint16_t typ;
    uint16_t pktLen;
    memcpy(&typ, header, 2);
    memcpy(&pktLen, header + 2, 2);

    if (pktLen > maxLen) {
        return false;
    }

    // читаем payload
    bytesRead = 0;
    while (bytesRead < pktLen) {
        ssize_t n = ::read(m_fd, data + bytesRead, pktLen - bytesRead);
        if (n <= 0) {
            return false;
        }
        bytesRead += n;
    }

    // читаем CRC (4 байта)
    uint32_t receivedCrc;
    bytesRead = 0;
    while (bytesRead < sizeof(receivedCrc)) {
        ssize_t n = ::read(m_fd, ((uint8_t*)&receivedCrc) + bytesRead, sizeof(receivedCrc) - bytesRead);
        if (n <= 0) {
            return false;
        }
        bytesRead += n;
    }

    // TODO: проверить CRC
    // uint8_t calc = crc8(...);

    type = typ;
    len = pktLen;
    return true;
}

bool SimpleProtocol::send(uint16_t type, const uint8_t* data, size_t len) {
    return writePacket(type, data, len);
}

void SimpleProtocol::setCallback(Callback cb, void* userdata) {
    printf("MEMS App setCallback\n");
    m_callback = cb;
    m_userdata = userdata;
}

void SimpleProtocol::runLoop() {
    printf("MEMS App runLoop\n");
    if (!m_isOpen) return;
    uint16_t type;
    uint8_t data[256];
    size_t len;
    while (m_isOpen) {
        if (readPacket(type, data, len, sizeof(data))) {
            if (m_callback) {
                m_callback(type, data, len, m_userdata);
            }
        }
        else{
           // usleep(1);
        }
    }
}