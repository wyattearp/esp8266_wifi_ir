#ifndef CONFIG_ITEM_H
#define CONFIG_ITEM_H

#include <stdint.h>

struct config_data {
    uint32_t multicast_addr;
    uint16_t multicast_port;
};

#endif