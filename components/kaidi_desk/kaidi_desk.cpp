#include "esphome/core/gpio.h"
#include "esphome/core/log.h"
#include "kaidi_desk.h"


namespace esphome::kaidi_desk {

static const char *TAG = "kaidi_desk";

const char *kaidi_desk_operation_to_str(KaidiDeskOperation op) {
  switch (op) {
    case IDLE:
      return "IDLE";
    case RAISING:
      return "RAISING";
    case LOWERING:
      return "LOWERING";
    default:
      return "UNKNOWN";
  }
}

void KaidiDesk::setup() {
  if (this->up_pin_ != nullptr) {
    this->up_pin_->pin_mode(gpio::Flags::FLAG_OUTPUT);
    this->up_pin_->digital_write(false);
  }
  if (this->down_pin_ != nullptr) {
    this->down_pin_->pin_mode(gpio::Flags::FLAG_OUTPUT);
    this->down_pin_->digital_write(false);
  }
}

void KaidiDesk::request_operation(KaidiDeskOperation operation) {
  if (operation == this->current_operation_) {
    return;
  }
  if ((operation == IDLE || this->current_operation_ == RAISING) && this->up_pin_ != nullptr) {
    this->up_pin_->digital_write(false);
  }
  if ((operation == IDLE || this->current_operation_ == LOWERING) && this->down_pin_ != nullptr) {
    this->down_pin_->digital_write(false);
  }
  if (operation != IDLE) {
    last_move_time_ = millis();
  }
  if (operation == RAISING && this->up_pin_ != nullptr) {
    this->up_pin_->digital_write(true);
    this->current_operation_ = operation;
  }
  if (operation == LOWERING && this->down_pin_ != nullptr) {
    this->down_pin_->digital_write(true);
    this->current_operation_ = operation;
  }
  if (operation == IDLE) {
    this->current_operation_ = operation;
  }
}

KaidiDeskOperation KaidiDesk::current_operation() {
  return this->current_operation_;
}

void KaidiDesk::loop() {
  if (this->current_operation_ != IDLE && last_move_time_ + duration_until_reset_ < millis()) {
    request_operation(IDLE);
  }
}

void KaidiDesk::dump_config() {
    ESP_LOGCONFIG(TAG, "Kaidi desk:");
    LOG_PIN("Up pin: ", this->up_pin_);
    LOG_PIN("Down pin: ", this->down_pin_);
    ESP_LOGCONFIG(TAG, "Duration until reset: %ums", this->duration_until_reset_);
}

cover::CoverTraits KaidiDesk::get_traits() {
  auto traits = cover::CoverTraits();
  traits.set_supports_stop(true);
  traits.set_supports_tilt(false);
  traits.set_supports_toggle(false);
  traits.set_is_assumed_state(false);
  traits.set_supports_position(true);
  return traits;
}

void KaidiDesk::control(const cover::CoverCall &call) {
  if (call.get_stop()) {
    request_operation(IDLE);
    return;
  }
  if (call.get_position() > 0.5f) {
    request_operation(RAISING);
  } else {
    request_operation(LOWERING);
  }
}

} // namespace esphome::kaidi_desk
