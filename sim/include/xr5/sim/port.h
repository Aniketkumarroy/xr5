#ifndef PORT_H_
#define PORT_H_

#include "xr5/utils/types.h"
#include <algorithm>
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
  static constexpr Id kInvalidId = int16_t(-1);

  /** DISCUSS: should we steal the resource of the caller(name_ =
   * std::move(name))
   */
  Port(const std::string &name, Id id) : name_(name), id_(id) {
    ++num_of_ports_minus_one_;
  }
  Port() = delete;
  virtual ~Port() { unbindAll(); };

  /**
   * @brief binds the port with its sink
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

  void unbindAll();

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
   * @brief send a packet to every sink in \c sinks_ by calling \c sink->receive
   *
   * @param[in] packet packet to send
   */
  inline void send(const Packet *packet) {
    /** DISCUSS: can we parallelize this loop
     */
    for (const auto sink : sinks_)
      sink->receive(packet);
  }

  const std::string &getName() { return name_; }

  const Id getId() { return id_; }

  static Id getNewId() { return num_of_ports_minus_one_; }

private:
  /** DISCUSS: although it will be a good practice to use std::unordered_set
   * over vector to avoid duplication, fast removal and find operation, we stick
   * to vector because this find, removal, addition will only be done in
   * initialization or stop step of the system. we will bear this small startup
   * or stop cost to get, in runtime, benefit of better cache locality of
   * vectors in looping through them
   */
  std::vector<Port *> sinks_;
  /** DISCUSS: producer ports don't wait for callbacks from their source,
   * instead its the duty of a producer port to broadcast packet to every of its
   * sinks, so just maintaining a vector of sinks is enough. but still we are
   * maintaining a vector of sources too because, if a sink goes out of scope,
   * or want's to disconnect, it must erase its presense in each of its sources'
   * sinks vector so that the any source will not broadcast packet to this sink
   */
  std::vector<Port *> sources_;
  std::string name_;
  Id id_ = -1;
  static Id num_of_ports_minus_one_;
};

} // namespace sim
} // namespace xr5

#endif