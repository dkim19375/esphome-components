#pragma once

#include "esphome/components/cover/cover.h"
#include "esphome/core/component.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace kaidi_desk {

enum KaidiDeskOperation {
  RAISING,
  LOWERING,
  IDLE,
};

const char *kaidi_desk_operation_to_str(KaidiDeskOperation op);

class KaidiDesk : public cover::Cover, public Component {
 public:
  void setup() override;
  void loop() override;
  void dump_config() override;

  cover::CoverTraits get_traits() override;

  void request_operation(KaidiDeskOperation operation);
  KaidiDeskOperation current_operation();

  void set_up_pin(GPIOPin *pin) { this->up_pin_ = pin; }
  void set_down_pin(GPIOPin *pin) { this->down_pin_ = pin; }
  void set_duration_until_reset(uint32_t duration) { this->duration_until_reset_ = duration; }

 protected:
  void control(const cover::CoverCall &call) override;

  GPIOPin *up_pin_{nullptr};
  GPIOPin *down_pin_{nullptr};
  uint32_t duration_until_reset_{0};
  uint32_t last_move_time_{0};
  KaidiDeskOperation current_operation_{IDLE};
};

}  // namespace kaidi_desk
}  // namespace esphome