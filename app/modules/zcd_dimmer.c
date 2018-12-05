
#include "lualib.h"
#include "lauxlib.h"
#include "platform.h"
#include "module.h"
#include "c_types.h"
#include "c_string.h"
#include "gpio.h"
#include "hw_timer.h"

#define PULLUP PLATFORM_GPIO_PULLUP
#define FLOAT PLATFORM_GPIO_FLOAT
#define OUTPUT PLATFORM_GPIO_OUTPUT
#define INPUT PLATFORM_GPIO_INPUT
#define INTERRUPT PLATFORM_GPIO_INT
#define HIGH PLATFORM_GPIO_HIGH
#define LOW PLATFORM_GPIO_LOW

unsigned int zcd_dimmer_p;

static const os_param_t TIMER_OWNER = 0x6770677f;

static void ICACHE_RAM_ATTR zcd_timer(os_param_t p) 
{
    platform_gpio_write(zcd_dimmer_p, HIGH);
}

static uint32_t  ICACHE_RAM_ATTR zcd_interrupt(uint32_t ret_gpio_status) 
{
    platform_gpio_write(zcd_dimmer_p, LOW);

    platform_hw_timer_arm_us(TIMER_OWNER, 750);

    return ret_gpio_status;
}

static int zcd_dimmer_start( lua_State* L )
{
    unsigned int zcd_pin;
    unsigned int control_pin;
    
    zcd_pin = (unsigned) luaL_checkinteger( L, 1 );
    control_pin = (unsigned) luaL_checkinteger( L, 2 );
    
    zcd_dimmer_p = control_pin;

    platform_gpio_mode(zcd_pin, INTERRUPT, PLATFORM_GPIO_FLOAT);
    platform_gpio_mode(control_pin, OUTPUT, PLATFORM_GPIO_FLOAT);

    platform_gpio_register_intr_hook(zcd_pin, zcd_interrupt);
    platform_gpio_intr_init(zcd_pin, 5);

    if (!platform_hw_timer_init(TIMER_OWNER, FRC1_SOURCE, FALSE)) {
      // Failed to init the timer
      luaL_error(L, "Unable to initialize timer");
    }

    platform_hw_timer_set_func(TIMER_OWNER, zcd_timer, 0);

    return 1;
}


const LUA_REG_TYPE zcd_dimmer_map[] =
{
  { LSTRKEY( "start" ), LFUNCVAL( zcd_dimmer_start ) },
  { LNILKEY, LNILVAL } // This map must always end like this
};

NODEMCU_MODULE(ZCD_DIMMER, "zcd_dimmer", zcd_dimmer_map, NULL);