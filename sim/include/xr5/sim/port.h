#ifndef PORT_H_
#define PORT_H_

#include "xr5/utils/types.h"
#include <memory>
#include <string>
#include <vector>

namespace xr5 {
namespace sim {

struct Packet {

  enum class Type : uint8_t {
    WORD,
    ADDRESS,
    COHERENCE, // invalidations, snoops, probes
    CONTROL,   // barriers, TLB shootdowns, interrupts
    DRAM_CMD,  // ACT, PRE, REF
    DRAM_DATA  // burst read/write
  };

  Type type;

  // ---- Type-specific data ----
  union Data {
    xr5::types::Address addr;
    xr5::types::Word word;

    struct {
      int srcId;
      int dstId;
      int state; // e.g., MESI state
    } coherence;

    struct {
      int bank;
      int row;
      int col;
      bool isRead; // DRAM read/write command
    } dram;

    struct {
      int barrierType; // full fence, store fence, etc.
    } control;

  } data;
};

class Port {

public:
  using Id = int16_t;
  using Ptr = std::unique_ptr<Port>;
  const static Id InvalidId = int16_t(-1);

  /** DISCUSS: should we steal the resource of the caller(name_ =
   * std::move(name))
   */
  Port(const std::string &name, Id id) : name_(name), id_(id) {}
  Port() = delete;
  virtual ~Port() { unbindAllSource(); };

  /**
   * @brief binds the port with its peer
   *
   * NOTE: we are using raw pointers and not smart pointers because Ports life
   * will be managed by their creators and we don't want to have a ownership
   * responsibility of some other ports here. here @p sink memory will be
   * managed by its class and we don't want to take or influence it and get into
   * a reference cycle
   *
   * @param[in] sink raw pointer of Port
   */
  void bindSink(Port *sink);
  void unbindSink(Port *sink);

  void bindSource(Port *source);
  void unbindSource(Port *source);
  void unbindAllSource();

  /**
   * @brief receives the pointer to the data
   *
   * NOTE: we are using raw pointers and not smart pointers because packets life
   * will be managed by their creators and we don't want to have a ownership
   * responsibility of some other packets here. here @p packet memory will be
   * managed by its users and we don't want to take or influence it and get into
   * a reference cycle. we just want to copy relevent fields of the packet into
   * our context. we have to copy data from packet because after sending data to
   * us by the packet, the packet can be reused by its owner for other purposes
   * before even we can utilize the data.
   *
   * DISCUSS: can we have a better idea to de-virtualize \c receive but leave
   * the data handling after receive to user
   *
   * @param[in] packet raw pointer to the packet
   */
  virtual void receive(const Packet *packet) = 0;
  /**
   * @brief send a packet to its peer by calling \c peer_->receive
   *
   * @param[in] packet packet to send
   */
  void send(const Packet *packet);

  const std::string &getName() { return name_; }

private:
  std::vector<Port *> sinks_;
  std::vector<Port *> sources_;
  std::string name_;
  Id id_ = -1;
};

} // namespace sim
} // namespace xr5

#endif