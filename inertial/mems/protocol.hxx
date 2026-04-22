#ifndef SIMPLE_PROTOCOL_HXX
#define SIMPLE_PROTOCOL_HXX

#include <cstddef>
#include <cstdint>
#include <stdbool.h>

class SimpleProtocol {
public:
  // Тип callback: void callback(uint8_t type, const uint8_t* data, size_t len,
  // void* userdata)
  using Callback = void (*)(uint16_t type, const uint8_t *data, size_t len,
                            void *userdata);

  explicit SimpleProtocol(const char *devicePath);
  ~SimpleProtocol();

  bool open();
  void close();
  bool isOpen() const { return m_isOpen; }

  bool send(uint16_t type, const uint8_t *data, size_t len);
  void setCallback(Callback cb, void *userdata);
  void runLoop(); // бесконечный цикл

private:
  bool readPacket(uint16_t &type, uint8_t *data, size_t &len, size_t maxLen);
  bool read_pref();
  bool writePacket(uint16_t type, const uint8_t *data, size_t len);

  int m_fd;
  const char *m_device;
  bool m_isOpen;
  Callback m_callback;
  void *m_userdata;
};

#endif