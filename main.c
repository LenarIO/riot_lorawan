/*
 * Copyright (C) 2017 Inria
 *               2017 Inria Chile
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     tests
 *
 * @file
 * @brief       Semtech LoRaMAC test application
 *
 * @author      Alexandre Abadie <alexandre.abadie@inria.fr>
 * @author      Jose Alamos <jose.alamos@inria.cl>
 */

#include <string.h>

#include "msg.h"
#include "shell.h"
#include "fmt.h"
#include "xtimer.h"

#include "net/loramac.h"
#include "semtech_loramac.h"
#include "isl29125.h"

/* we will use Cayenne LPP for displaying our data */
#include "cayenne_lpp.h"

#define BUF_SIZE    64
#define LORA_PORT   1

/* Use a unique value for DEV_EUI in TTN */
#define DEV_EUI         "0000000000000000"

/* APP_EUI and AP_KEY are provided by the network server (TTN)*/
#define APP_EUI         "0000000000000000"
#define APP_KEY         "00000000000000000000000000000000"

/* SF7BW125 */
#define LORAWAN_DATARATE 5

/* we use "Dynamic Sensor Payload for our data */
#define CAYENNE_LPP_CHANNEL 1

/* we must respect the duty cycle limitations */
#define SLEEP_TIME 10

static cayenne_lpp_t lpp;
semtech_loramac_t g_loramac;

void setup_light_sensor(isl29125_t *dev) {
    if (isl29125_init(dev, TEST_ISL29125_I2C, TEST_ISL29125_IRQ_PIN,
                ISL29125_MODE_RGB, ISL29125_RANGE_10K,
                ISL29125_RESOLUTION_16) == 0) {
        puts("Sensor Initialized\n");
    }
    else {
        puts("Sensor Failed");
        return;
    }

    puts("Resetting mode to RGB and reading continuously");
    isl29125_set_mode(dev, ISL29125_MODE_RGB);
}

void setup_lora(semtech_loramac_t *loramac) {
    uint8_t buf[BUF_SIZE];

    /* init LoRaMAC */
    semtech_loramac_init(loramac);

    /* load required keys into LoRaMAC */
    fmt_hex_bytes(buf, DEV_EUI);
    semtech_loramac_set_deveui(loramac, buf);

    fmt_hex_bytes(buf, APP_EUI);
    semtech_loramac_set_appeui(loramac, buf);

    fmt_hex_bytes(buf, APP_KEY);
    semtech_loramac_set_appkey(loramac, buf);

    /* Try to join by Over The Air Activation */
    if (semtech_loramac_join(loramac, LORAMAC_JOIN_OTAA) != SEMTECH_LORAMAC_JOIN_SUCCEEDED) {
		puts("Join failed");
	}
    else {
        puts("Join Success");
    }
}

uint16_t get_grayscale(isl29125_t *dev) {
    isl29125_rgb_t data;
    memset(&data, 0x00, sizeof(data));

    isl29125_read_rgb_lux(dev, &data);

    uint16_t value;

    /* Luma */
    value = 0.299*data.red + 0.587*data.green + 0.114*data.blue; 
    printf("Grayscale value: %d\n", value);
    return value;
}

void send_lora_data(semtech_loramac_t *loramac, uint16_t data) {

    printf("Sending:");

    /* reset our cayenne buffer */
    cayenne_lpp_reset(&lpp);

    /* add luminosity to cayanne payload */
    cayenne_lpp_add_luminosity(&lpp, CAYENNE_LPP_CHANNEL, data);
    /* add other values as needed. Eg:
     *
     * cayenne_lpp_add_temperature(...);
     * cayenne_lpp_add_digital_output(...);
     *
     * These values will be encoded in a Cayenne LPP packet */

    /* set unconfirmed packets and port */
    semtech_loramac_set_tx_mode(loramac, LORAMAC_TX_UNCNF);
    semtech_loramac_set_tx_port(loramac, LORA_PORT);

    /* set datarate */
    semtech_loramac_set_dr(loramac, LORAWAN_DATARATE);


    /* try to send data */
    switch (semtech_loramac_send(loramac, lpp.buffer, lpp.cursor)) {
        case SEMTECH_LORAMAC_NOT_JOINED:
            puts("Failed: not joined");
            return;

        case SEMTECH_LORAMAC_BUSY:
            puts("Failed: mac is busy");
            return;
    }

    /* check if something was received */
    switch (semtech_loramac_recv(loramac)) {
        case SEMTECH_LORAMAC_DATA_RECEIVED:
            loramac->rx_data.payload[loramac->rx_data.payload_len] = 0;
            printf("Data received: %s, port: %d\n",
                   (char *)loramac->rx_data.payload, loramac->rx_data.port);
            break;

        case SEMTECH_LORAMAC_TX_CNF_FAILED:
            puts("Confirmable TX failed");
            break;

        case SEMTECH_LORAMAC_TX_DONE:
            puts("TX complete, no data received");
            break;
    }
}

int main(void)
{
    /* This sensor is not yet supported by SAUL :( 
     * Feel free to collaborate to RIOT and make this possible! */
    isl29125_t dev;

    puts("Initializing");

    setup_lora(&g_loramac);
    setup_light_sensor(&dev);
    uint16_t grayscale;

	while (1)
	{
        LED1_TOGGLE;
        grayscale = get_grayscale(&dev);
        send_lora_data(&g_loramac, grayscale);
		xtimer_sleep(SLEEP_TIME);
	}
}
