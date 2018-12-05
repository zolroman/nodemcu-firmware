
#include "lualib.h"
#include "lauxlib.h"
#include "platform.h"
#include "module.h"
#include "c_types.h"
#include "c_string.h"

#define PULLUP PLATFORM_GPIO_PULLUP
#define FLOAT PLATFORM_GPIO_FLOAT
#define OUTPUT PLATFORM_GPIO_OUTPUT
#define INPUT PLATFORM_GPIO_INPUT
#define INTERRUPT PLATFORM_GPIO_INT
#define HIGH PLATFORM_GPIO_HIGH
#define LOW PLATFORM_GPIO_LOW

static int zcd_dimmer_start( lua_State* L )
{
    unsigned zcd_pin;
    unsigned control_pin;
    
    zcd_pin = (unsigned) luaL_checkinteger( L, 1 );
    control_pin = (unsigned) luaL_checkinteger( L, 2 );
    
    platform_gpio_mode(zcd_pin, INTERRUPT);
    platform_gpio_mode(control_pin, OUTPUT);


    platform_gpio_register_intr_hook(zcd_pin, zcd_interrupt);
    platform_gpio_intr_init

    return 1;
}


static uint32_t  ICACHE_RAM_ATTR zcd_interrupt(uint32_t ret_gpio_status) 
{
    
    return ret_gpio_status;
}
