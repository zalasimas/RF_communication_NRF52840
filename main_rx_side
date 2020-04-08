
// Niekas neveike, kol i nRF_Drivers folderi neikeliau radio_config.c failo. Dabar viskas veikia kaip ir turetu.
// Sitas yra vienintelis modifikuotas ir veikiantis RF Rx Tx kodas.


#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#include "nrf_drv_systick.h"
#include "nrf_delay.h"
#include "boards.h"

#include "nrf_drv_clock.h"

#include "radio_config.h"


//start for USB CDC 

#include "nrf.h"
#include "nrf_drv_usbd.h"
#include "nrf_drv_clock.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "nrf_drv_power.h"
#include "app_usbd_core.h"
#include "app_usbd.h"
#include "app_usbd_string_desc.h"
#include "app_usbd_cdc_acm.h"
#include "app_usbd_serial_num.h"

#include "app_timer.h"

// USB CDC LEDs

#define LED_USB_RESUME      (BSP_BOARD_LED_0)
#define LED_CDC_ACM_OPEN    (BSP_BOARD_LED_1)
#define LED_CDC_ACM_RX      (BSP_BOARD_LED_2)
#define LED_CDC_ACM_TX      (BSP_BOARD_LED_3)

#define BTN_CDC_DATA_SEND       0
#define BTN_CDC_NOTIFY_SEND     1


//#define BTN_CDC_DATA_KEY_RELEASE        (bsp_event_t)(BSP_EVENT_KEY_LAST + 1)

/**
 * @brief Enable power USB detection
 *
 * Configure if example supports USB port connection
 */
#ifndef USBD_POWER_DETECTION
#define USBD_POWER_DETECTION true
#endif


static void cdc_acm_user_ev_handler(app_usbd_class_inst_t const * p_inst,
                                    app_usbd_cdc_acm_user_event_t event);

#define CDC_ACM_COMM_INTERFACE  0
#define CDC_ACM_COMM_EPIN       NRF_DRV_USBD_EPIN2

#define CDC_ACM_DATA_INTERFACE  1
#define CDC_ACM_DATA_EPIN       NRF_DRV_USBD_EPIN1
#define CDC_ACM_DATA_EPOUT      NRF_DRV_USBD_EPOUT1


/**
 * @brief CDC_ACM class instance
 * */
APP_USBD_CDC_ACM_GLOBAL_DEF(m_app_cdc_acm,
                            cdc_acm_user_ev_handler,
                            CDC_ACM_COMM_INTERFACE,
                            CDC_ACM_DATA_INTERFACE,
                            CDC_ACM_COMM_EPIN,
                            CDC_ACM_DATA_EPIN,
                            CDC_ACM_DATA_EPOUT,
                            APP_USBD_CDC_COMM_PROTOCOL_AT_V250
);


#define READ_SIZE 1

static char m_rx_buffer[READ_SIZE];
static char m_tx_buffer[NRF_DRV_USBD_EPSIZE];
static bool m_send_flag = 0;


static void cdc_acm_user_ev_handler(app_usbd_class_inst_t const * p_inst,
                                    app_usbd_cdc_acm_user_event_t event)
{
    app_usbd_cdc_acm_t const * p_cdc_acm = app_usbd_cdc_acm_class_get(p_inst);

    switch (event)
    {
        case APP_USBD_CDC_ACM_USER_EVT_PORT_OPEN:
        {
            bsp_board_led_on(LED_CDC_ACM_OPEN);

            /*Setup first transfer*/
            ret_code_t ret = app_usbd_cdc_acm_read(&m_app_cdc_acm,
                                                   m_rx_buffer,
                                                   READ_SIZE);
            UNUSED_VARIABLE(ret);
            break;
        }
        case APP_USBD_CDC_ACM_USER_EVT_PORT_CLOSE:
            bsp_board_led_off(LED_CDC_ACM_OPEN);
            break;
        case APP_USBD_CDC_ACM_USER_EVT_TX_DONE:
            bsp_board_led_invert(LED_CDC_ACM_TX);
            break;
        case APP_USBD_CDC_ACM_USER_EVT_RX_DONE:
        {
            ret_code_t ret;
//            NRF_LOG_INFO("Bytes waiting: %d", app_usbd_cdc_acm_bytes_stored(p_cdc_acm));
            app_usbd_cdc_acm_bytes_stored(p_cdc_acm);
            do
            {
                /*Get amount of data transfered*/
                size_t size = app_usbd_cdc_acm_rx_size(p_cdc_acm);
//                NRF_LOG_INFO("RX: size: %lu char: %c", size, m_rx_buffer[0]);

                /* Fetch data until internal buffer is empty */
                ret = app_usbd_cdc_acm_read(&m_app_cdc_acm,
                                            m_rx_buffer,
                                            READ_SIZE);
            } while (ret == NRF_SUCCESS);

            memcpy(m_tx_buffer,m_rx_buffer,READ_SIZE);
            app_usbd_cdc_acm_write(&m_app_cdc_acm, m_tx_buffer, READ_SIZE); //

            bsp_board_led_invert(LED_CDC_ACM_RX);
            break;
        }
        default:
            break;
    }
}

static void usbd_user_ev_handler(app_usbd_event_type_t event)
{
    switch (event)
    {
        case APP_USBD_EVT_DRV_SUSPEND:
            bsp_board_led_off(LED_USB_RESUME);
            break;
        case APP_USBD_EVT_DRV_RESUME:
            bsp_board_led_on(LED_USB_RESUME);
            break;
        case APP_USBD_EVT_STARTED:
            break;
        case APP_USBD_EVT_STOPPED:
            app_usbd_disable();
            bsp_board_leds_off();
            break;
        case APP_USBD_EVT_POWER_DETECTED:
//            NRF_LOG_INFO("USB power detected");

            if (!nrf_drv_usbd_is_enabled())
            {
                app_usbd_enable();
            }
            break;
        case APP_USBD_EVT_POWER_REMOVED:
//            NRF_LOG_INFO("USB power removed");
            app_usbd_stop();
            break;
        case APP_USBD_EVT_POWER_READY:
//            NRF_LOG_INFO("USB ready");
            app_usbd_start();
            break;
        default:
            break;
    }
}
/// end for USB CDC

//static uint32_t                   packet;           //   < Packet to transmit. 
char packet;

uint32_t read_packet()
{
    uint32_t result = 0;

    NRF_RADIO->EVENTS_READY = 0U;
    NRF_RADIO->TASKS_RXEN = 1U;     // Enable radio and wait for ready
    while (NRF_RADIO->EVENTS_READY == 0U);
    NRF_RADIO->EVENTS_END = 0U;
    NRF_RADIO->TASKS_START = 1U;  // Start listening and wait for address received event
    while (NRF_RADIO->EVENTS_END == 0U) ; // Wait for end of packet or buttons state changed

    if (NRF_RADIO->CRCSTATUS == 1U) { result = packet; }

    NRF_RADIO->EVENTS_DISABLED = 0U;
    NRF_RADIO->TASKS_DISABLE = 1U;     // Disable radio
    while (NRF_RADIO->EVENTS_DISABLED == 0U) ; 
    return result;
}

int main(void)
{

ret_code_t ret;
  static const app_usbd_config_t usbd_config = {
      .ev_state_proc = usbd_user_ev_handler
  };

  bsp_board_init(BSP_INIT_LEDS);


/*
 Gali buti viena is priezasciu kodel neveikia,
 nes USB CDC naudoja vienoki clock skaiciu, o radjas - kitoki
 t.y., USB CDC buvo nustatytas low frequency rezonatorius,
 o radijas naudoja ir HIGH (isorinis) ir LOW (isorinis)  frequency
 rezonatorius. Kol veikia LOW freq rezonatorius, USB nera aptinkamas,
 taciau, kai ijungti tiek high tiek low, ismetama windows lentele,
 kad USB irenginys yra sugandintas. 

 ===UPDATE===

Win ismesdavo pranesima apie sugadinta USB del to, nes buvo deklaruoti
int32_t received = read_packet(); ir LED_Control(received-1); kodo
apacioje. USB veikia tiek su HIGH tiek su LOW freq rezonatoriais. 
Problema turetu buti read_packet() f-joje.
*/
  ///start clock set mandatory for USB CDC

  nrf_drv_clock_init(); 
  nrf_drv_clock_hfclk_request(NULL); // for HF 32MHz external X-tal
  while(!nrf_drv_clock_hfclk_is_running()) ; // Just waiting
  nrf_drv_clock_lfclk_request(NULL); // for LF 32.768kHz external X-tal
  while(!nrf_drv_clock_lfclk_is_running()) ; 
  /// end clock set

 /// end clock set
  app_usbd_serial_num_generate();
  ret = app_usbd_init(&usbd_config);
  APP_ERROR_CHECK(ret);

  app_usbd_class_inst_t const * class_cdc_acm = app_usbd_cdc_acm_class_inst_get(&m_app_cdc_acm);
  ret = app_usbd_class_append(class_cdc_acm);
  APP_ERROR_CHECK(ret);

  if (USBD_POWER_DETECTION) {
      ret = app_usbd_power_events_enable();
      APP_ERROR_CHECK(ret);
  }
  else {
      app_usbd_enable();
      app_usbd_start();
  }


 radio_configure();
  NRF_RADIO->PACKETPTR = (uint32_t)&packet;


  while(true)
  {

char                       paketas_pakeistas[600];   // KEISTI CIA!!!!!
uint32_t result = 0;
//char result;
//char packet[10];
NRF_RADIO->EVENTS_READY = 0U;
NRF_RADIO->TASKS_RXEN = 1U;     // Enable radio and wait for ready
while (NRF_RADIO->EVENTS_READY == 0U);
NRF_RADIO->EVENTS_END = 0U;
NRF_RADIO->TASKS_START = 1U;  // Start listening and wait for address received event

/*
TIES SITA VIETA KODAS LUZTA (PRADEDAMAS NEBEMATYTI USB)
============
while (NRF_RADIO->EVENTS_END == 0U) ; // Wait for end of packet or buttons state changed
============
*/

static int  frame_counter;

while (NRF_RADIO->EVENTS_END == 0U) { // Wait for end of packet or buttons state changed
app_usbd_event_queue_process();
}

//if (NRF_RADIO->CRCSTATUS == 1U) { result = packet; }
if (NRF_RADIO->CRCSTATUS == 1U) { result = packet; }
NRF_RADIO->EVENTS_DISABLED = 0U;
NRF_RADIO->TASKS_DISABLE = 1U;     // Disable radio
while (NRF_RADIO->EVENTS_DISABLED == 0U) ; 

      //  size_t size = sprintf(m_tx_buffer, "Gautas paketas:  \r\n",packet, frame_counter);
      // size_t size = sprintf(m_tx_buffer,(unsigned int)result, frame_counter);
        //ret = app_usbd_cdc_acm_write(&m_app_cdc_acm, m_tx_buffer, size);

      sprintf(paketas_pakeistas,"%lu", result);

        size_t size = sprintf(m_tx_buffer,paketas_pakeistas, frame_counter);
        ret = app_usbd_cdc_acm_write(&m_app_cdc_acm, m_tx_buffer, size);

        //app_usbd_event_queue_process();
        nrf_delay_ms(10);
       // return result;
  }
}



