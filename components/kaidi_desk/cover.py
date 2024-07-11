import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import cover
from esphome.const import CONF_ID

kaidi_desk_ns = cg.esphome_ns.namespace('kaidi_desk')
KaidiDesk = kaidi_desk_ns.class_('KaidiDesk', cover.Cover, cg.Component)

CONF_DURATION_UNTIL_RESET = "duration_until_reset"
CONF_UP_PIN = "up_pin"
CONF_DOWN_PIN = "down_pin"

CONFIG_SCHEMA = cover.COVER_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(KaidiDesk),
    cv.Optional(CONF_UP_PIN): pins.gpio_output_pin_schema,
    cv.Optional(CONF_DOWN_PIN): pins.gpio_output_pin_schema,
    cv.Optional(CONF_DURATION_UNTIL_RESET, default="15s"): cv.positive_time_period_milliseconds
}).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await cover.register_cover(var, config)

    if CONF_UP_PIN in config:
        pin = await cg.gpio_pin_expression(config[CONF_UP_PIN])
        cg.add(var.set_up_pin(pin))
    if CONF_DOWN_PIN in config:
        pin = await cg.gpio_pin_expression(config[CONF_DOWN_PIN])
        cg.add(var.set_down_pin(pin))
    cg.add(var.set_duration_until_reset(config[CONF_DURATION_UNTIL_RESET].total_milliseconds))
