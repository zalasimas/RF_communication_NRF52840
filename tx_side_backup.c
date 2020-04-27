// integruotas paprastas NRF tickeris, kuris LED jungineja pagal nustatyta intervala.

#define TEST_STRING "Labas"

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include "boards.h"
#include "radio_config.h" // for radio
#include "nrf_drv_clock.h"
#include "nrf_delay.h"
#include "nrf.h"
#include "nrf_drv_timer.h"
#include "bsp.h"

//uint32_t                   packet = 1; 
//static char       packet[5] = TEST_STRING;


/*
Galima deklaruoti siunciama paketa taip, ir Rx puseje viskas yra gaunama. Taciau turi buti nurodyti elementai buferije.
*/
//static uint8_t packet_buf[5] = {'L','a'};

// low power taimeris. High sukonfiguruotas Rx puseje.
const nrf_drv_timer_t TIMER_LED = NRF_DRV_TIMER_INSTANCE(0);

static uint8_t packet_buf[PACKET_PAYLOAD_MAXSIZE];

//static uint8_t packet_buf[PACKET_PAYLOAD_MAXSIZE]; 


/**
 * @brief Handler for timer events.
 */
void timer_led_event_handler(nrf_timer_event_t event_type, void* p_context)
{
    static uint32_t i;
    uint32_t led_to_invert = ((i++) % LEDS_NUMBER);

    switch (event_type)
    {
        case NRF_TIMER_EVENT_COMPARE0:
          //bsp_board_led_invert(led_to_invert);
          bsp_board_led_invert(0); // zalias LED
            break;

        default:
            //Do nothing.
            break;
    }
}


void send_packet()
{
    // send the packet:
    NRF_RADIO->EVENTS_READY = 0U;
    NRF_RADIO->TASKS_TXEN   = 1;
    while (NRF_RADIO->EVENTS_READY == 0U) ; 
    NRF_RADIO->EVENTS_END  = 0U;
    NRF_RADIO->TASKS_START = 1U;
    while (NRF_RADIO->EVENTS_END == 0U) ;
    NRF_RADIO->EVENTS_DISABLED = 0U;
    NRF_RADIO->TASKS_DISABLE = 1U;     // Disable radio
    while (NRF_RADIO->EVENTS_DISABLED == 0U) ; 
}

void LED_Control(uint8_t led_idx)
{
  switch(led_idx)
  {
    case 0: 
    bsp_board_led_on(0);
    bsp_board_led_off(1);
    bsp_board_led_off(2);
    bsp_board_led_off(3);
    break; 
    case 1: 
    bsp_board_led_off(0);
    bsp_board_led_on(1);
    bsp_board_led_off(2);
    bsp_board_led_off(3);
    break;
    case 2: 
    bsp_board_led_off(0);
    bsp_board_led_off(1);
    bsp_board_led_on(2);
    bsp_board_led_off(3);
    break;
    case 3: 
    bsp_board_led_off(0);
    bsp_board_led_off(1);
    bsp_board_led_off(2);
    bsp_board_led_on(3);
    break;
  }
}
int main(void)
{
  bsp_board_init(BSP_INIT_LEDS);
  nrf_drv_clock_init();
  nrf_drv_clock_hfclk_request(NULL); // for HF 32MHz external X-tal
  while(!nrf_drv_clock_hfclk_is_running()) ; // Just waiting
  nrf_drv_clock_lfclk_request(NULL); // for LF 32.768kHz external X-tal
  while(!nrf_drv_clock_lfclk_is_running()) ; // Just waiting
  radio_configure();
   // NRF_RADIO->PACKETPTR = (uint32_t)&packet;   // Set payload pointer
  //NRF_RADIO->PACKETPTR = (uint32_t)&packet_buf[0]; // PACKETPTR = koks_nors_buferis
  NRF_RADIO->PACKETPTR = (uint32_t)packet_buf; // PACKETPTR = koks_nors_buferis
  //NRF_RADIO->EVENTS_PAYLOAD = 4;


    uint32_t time_ms = 500; //Time(in miliseconds) between consecutive compare events.
    uint32_t time_ticks;
    uint32_t err_code = NRF_SUCCESS;

    //Configure TIMER_LED for generating simple light effect - leds on board will invert his state one after the other.
    nrf_drv_timer_config_t timer_cfg = NRF_DRV_TIMER_DEFAULT_CONFIG;
    err_code = nrf_drv_timer_init(&TIMER_LED, &timer_cfg, timer_led_event_handler);
    APP_ERROR_CHECK(err_code);

    time_ticks = nrf_drv_timer_ms_to_ticks(&TIMER_LED, time_ms);


    nrf_drv_timer_extended_compare(
       &TIMER_LED, NRF_TIMER_CC_CHANNEL0, time_ticks, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);

    nrf_drv_timer_enable(&TIMER_LED);

  while(true)
  {

//__WFI();

  // Set the length byte (first byte of the payload buffer)
   packet_buf[0] = 7;

   // Read Data to send
   packet_buf[1] = 'L';
   packet_buf[2] = 'a';
   packet_buf[3] = 'b';
   packet_buf[4] = 'a';
   packet_buf[5] = 's';
   packet_buf[6] = ' ';
   packet_buf[7] = '1';
   packet_buf[8] = '2';
   packet_buf[9] = '3';

  NRF_RADIO->EVENTS_READY = 0U;


 // Enable radio and wait for ready
  NRF_RADIO->TASKS_TXEN = 1;

 while (NRF_RADIO->EVENTS_READY == 0U)
    {
    }

    // Start transmission and wait for end of packet.
   NRF_RADIO->TASKS_START = 1U;

   NRF_RADIO->EVENTS_END = 0U;
    
    while(NRF_RADIO->EVENTS_END == 0U) // Wait for end
    {
    }

   NRF_RADIO->EVENTS_DISABLED = 0U;    

   // Disable radio
    NRF_RADIO->TASKS_DISABLE = 1U;

    while(NRF_RADIO->EVENTS_DISABLED == 0U)
    {
    }


    nrf_delay_ms(50);
  }
}
