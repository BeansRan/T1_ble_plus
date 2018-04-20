#ifndef EXTEND_SERVICE_INCLUDE
#define EXTEND_SERVICE_INCLUDE

#include <stdint.h>
#include <stdbool.h>

#include "pstorage_platform.h"
#include "ble.h"

#define DEV_NAME_SIZE                   16

typedef struct 
{
    bool        updt;
    uint8_t     length;
    uint8_t     data[DEV_NAME_SIZE];
}dev_name_t;

extern pstorage_handle_t                m_dev_adv_name_id;

void device_name_manager_init(void);
void ble_adv_name_manager_on_ble_evt(ble_evt_t *p_ble_evt) ;
#endif
