#include "main.h"


// Niekas neveike, kol i nRF_Drivers folderi neikeliau radio_config.c failo. Dabar viskas veikia kaip ir turetu.
// Sitas yra vienintelis modifikuotas ir veikiantis RF Rx Tx kodas.

//static void cdc_acm_user_ev_handler(app_usbd_class_inst_t const * p_inst, app_usbd_cdc_acm_user_event_t event);


/**
 * @brief CDC_ACM class instance
 * */
 /*
APP_USBD_CDC_ACM_GLOBAL_DEF(m_app_cdc_acm,
                            cdc_acm_user_ev_handler,
                            CDC_ACM_COMM_INTERFACE,
                            CDC_ACM_DATA_INTERFACE,
                            CDC_ACM_COMM_EPIN,
                            CDC_ACM_DATA_EPIN,
                            CDC_ACM_DATA_EPOUT,
                            APP_USBD_CDC_COMM_PROTOCOL_AT_V250
);
*/

//static char m_rx_buffer[READ_SIZE];
//static char m_tx_buffer[NRF_DRV_USBD_EPSIZE];
//static bool m_send_flag = 0;

static uint8_t volatile                  packet_buf[PACKET_PAYLOAD_MAXSIZE];        ///< Received packet buffer

/*********************** MAIN ***************************/
int main(void)
{

bsp_board_init(BSP_INIT_LEDS);
io_init();
dbg_init();
uart_init();

radio_configure();
//NRF_RADIO->PACKETPTR = (uint32_t)&packet;

// enable the use DWT
//CoreDebug->DEMCR |= 0x01000000; // Core debugas. Ijungiam DWT,

// Resetinam counteri
//DWT->CYCCNT = 0;

// Enablinam counteri
//DWT -> CTRL |= 0x1; 

//Ciklu skaicius saugomas kintamajame
//count = DWT->CYCCNT;


/*
ret_code_t ret;
  static const app_usbd_config_t usbd_config = {
      .ev_state_proc = usbd_user_ev_handler
  };
*/

 // bsp_board_init(BSP_INIT_LEDS);

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
  while(!nrf_drv_clock_lfclk_is_running()); 
  /// end clock set

 /// end clock set
 /*
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

*/

/// for radio receiver
// radio_configure();
 // NRF_RADIO->PACKETPTR = (uint32_t)&packet;

  volatile uint32_t count = 0;
  int x=0;
  int y=0;

// enable the use DWT
CoreDebug->DEMCR |= 0x01000000;

// Reset cycle counter
DWT->CYCCNT = 0;

// enable cycle counter
DWT->CTRL |= 0x1; 


  uart_transmit_char( '*' ); // sita reik pasiust nes kazkas nesuveikia
 // DBG("*** START ****\r\n");

  while(true) {

  //NRF_G

  //  static int            frame_counter;
    //static int            frame_counter_terminalui;

  //  uint32_t             received = read_packet();

//    char    rezultatas[40];  // KEISTI CIA!!!!!


    /*static uint8_t m_tx_buffer[NRF_DRV_USBD_EPSIZE * 16];
    sprintf(m_tx_buffer,"%c", received);
    app_usbd_cdc_acm_write(&m_app_cdc_acm, m_tx_buffer, sizeof(m_tx_buffer));


    //  sprintf(komandos_i_terminala,"%u", komandos_i_terminala_integer);

    //size_t size = sprintf(m_tx_buffer,rezultatas, frame_counter);
       // ret = app_usbd_cdc_acm_write(&m_app_cdc_acm, m_tx_buffer, size);


    if (ret == NRF_SUCCESS) {
      ++frame_counter;
    }
*/
//uint8_t packet_buf[2]; // nurodom toki pati buferi kaip ir Tx puseje.
//*packet_buf = read_packet(); // received data from RADIO. *koks_nors_buferis = read_packet();


/*
 Kaip pvz, Tx puseje paketas galetu buti nustatytas char duomenu tipu.

 Rx puseje paketo duomenis butu isvedami panaudojant "%s".

*/

//char gautas_paketas;
//char received[9] = "Labadiena";
//unsigned char gautas_paketas = received;

//sprintf((char)gautas_paketas, "%c", (unsigned int)received);

// Set payload pointer
    NRF_RADIO->PACKETPTR = (uint32_t)packet_buf;

    NRF_RADIO->EVENTS_READY = 0U;

    // Enable radio and wait for ready
    NRF_RADIO->TASKS_RXEN = 1U;

    while(NRF_RADIO->EVENTS_READY == 0U)
    {
    }
    NRF_RADIO->EVENTS_END = 0U;
    // Start listening and wait for address received event
    NRF_RADIO->TASKS_START = 1U;

    // Wait for end of packet
    while(NRF_RADIO->EVENTS_END == 0U)
    {
    }

    NRF_RADIO->EVENTS_DISABLED = 0U;

    // Disable radio
    NRF_RADIO->TASKS_DISABLE = 1U;

    while(NRF_RADIO->EVENTS_DISABLED == 0U)
    {
    }

    y++;
    if (y < 10) {
        LED_G_ON;
    }
    else {
        LED_G_OFF;
        if (y > 26) {
            y=0;
           // DBG("Valio %d\r\n", x++);
        //    DBG("%c %c %c %c\n\r", packet_buf[0], packet_buf[1], packet_buf[2], packet_buf[3]);
        // DBG("%c", packet_buf[0]);
        DBG("%s\n\r", packet_buf);

          // number of cycles stored in count variable
        count = DWT->CYCCNT;
        DBG("%u\n\r", count);
          // uart_transmit_char(received);
        }
    }
    dbg_handler();

  }// WHILE END
    }// MAIN END



/* init inputs-outputs */
void io_init( void ) {
}


/*static void cdc_acm_user_ev_handler(app_usbd_class_inst_t const * p_inst,
                                    app_usbd_cdc_acm_user_event_t event)
{
    app_usbd_cdc_acm_t const * p_cdc_acm = app_usbd_cdc_acm_class_get(p_inst);

    switch (event)
    {
        case APP_USBD_CDC_ACM_USER_EVT_PORT_OPEN:
        {
            bsp_board_led_on(LED_CDC_ACM_OPEN);

 
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
                size_t size = app_usbd_cdc_acm_rx_size(p_cdc_acm);
//                NRF_LOG_INFO("RX: size: %lu char: %c", size, m_rx_buffer[0]);

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

*/
/*
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
*/
/*
uint32_t read_packet( void )
{
  uint32_t result = 0;

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
/*
    while (NRF_RADIO->EVENTS_END == 0U) { // Wait for end of packet or buttons state changed
    //app_usbd_event_queue_process();
    }

    if (NRF_RADIO->CRCSTATUS == 1U) { result = packet; }

    NRF_RADIO->EVENTS_DISABLED = 0U;
    NRF_RADIO->TASKS_DISABLE = 1U;     // Disable radio
    while (NRF_RADIO->EVENTS_DISABLED == 0U) ; 
    return result;
}
*/

/*

kolkas sito nereikia kad siustu i terminala

        size_t komandos_siunciamos_terminale = sprintf(m_rx_buffer,komandos_i_terminala, frame_counter_terminalui);
        ret = app_usbd_cdc_acm_read(&m_app_cdc_acm, m_rx_buffer, size);
       
if(m_tx_buffer || m_rx_buffer >= 0)
{ 
bsp_board_led_on(3);
}

//else{
//bsp_board_led_off(3);
//}
*/
        //app_usbd_event_queue_process();
//        nrf_delay_ms(10);
       // return result;

