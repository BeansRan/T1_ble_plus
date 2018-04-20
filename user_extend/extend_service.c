#include <string.h>

#include "extend_service.h"
#include "pstorage.h"
#include "app_trace.h"
#include "app_error.h"

pstorage_handle_t                m_dev_adv_name_id;            

static void device_name_manager_callback(pstorage_handle_t  * handle,
                               uint8_t              op_code,
                               uint32_t             result,
                               uint8_t            * p_data,
                               uint32_t             data_len)
{
    switch(op_code)
    {      
       case PSTORAGE_UPDATE_OP_CODE:
           if (result == NRF_SUCCESS)
           {
              app_trace_log("dev name update end.\r\n"); 
           }
           break;
       default:
           break;
    }
}

/**@brief Function for the pstorage manager.
 */
void device_name_manager_init(void) 
{
    uint32_t err_code;
    pstorage_module_param_t pstorage_param;
    
    pstorage_param.block_count  = 1;
    pstorage_param.block_size   = 32;
    pstorage_param.cb           = device_name_manager_callback;
    
//    err_code = pstorage_init();
//    APP_ERROR_CHECK(err_code);
    
    err_code = pstorage_register(&pstorage_param, &m_dev_adv_name_id);
    APP_ERROR_CHECK(err_code);
}
/**@snippet [Pstorage Initialization] */

void ble_adv_name_manager_on_ble_evt(ble_evt_t *p_ble_evt)
{
    uint32_t err_code;
    dev_name_t dev_name;
    pstorage_handle_t dest_block_id;
    
     ble_gatts_evt_write_t * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;

    if((p_evt_write->context.char_uuid.uuid == BLE_UUID_GAP_CHARACTERISTIC_DEVICE_NAME) && 
       (p_ble_evt->header.evt_id == BLE_GATTS_EVT_WRITE))
    {
        dev_name.updt = true;
        dev_name.length = p_evt_write->len;
        memcpy(dev_name.data, p_evt_write->data, p_evt_write->len);
        
        err_code = pstorage_block_identifier_get(&m_dev_adv_name_id, 0, &dest_block_id);
        APP_ERROR_CHECK(err_code);
        
        /* Update internal falsh data */
        err_code = pstorage_update(&dest_block_id, dev_name.data, dev_name.length, 0);
        APP_ERROR_CHECK(err_code);
        
        app_trace_log("new adv name:%s\r\n", dev_name.data);
    }
}





