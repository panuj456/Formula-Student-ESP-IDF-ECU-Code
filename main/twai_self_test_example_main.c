#include "driver/twai.h"
#include "esp_log.h"

#define RX_GPIO_NUM 16
#define TX_GPIO_NUM 15

#define CAN_SPEED 1000000  // 1Mbps


void receive_can_message() {
    twai_message_t received_msg;

    // Wait for a CAN message to be received
    esp_err_t ret = twai_receive(&received_msg, portMAX_DELAY);
    if (ret == ESP_OK) {
        ESP_LOGI("TWAI", "Message received: ID=%03X", (unsigned int)received_msg.identifier);
        ESP_LOGI("TWAI", "Data: ");
        
        // Print each byte in the message
        for (int i = 0; i < received_msg.data_length_code; i++) {
            ESP_LOGI("TWAI", "Byte %d: %02X", i, received_msg.data[i]);
        }

        // Process your data as needed (for example, interpret the 8 data bytes)
    } else {
        ESP_LOGE("TWAI", "Failed to receive message!");
    }
}


void app_main(void) {
    // Configure the TWAI driver for 1Mbps and 11-bit IDs
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(TX_GPIO_NUM, RX_GPIO_NUM, TWAI_MODE_NORMAL);

    // Configure timing for 1MBit speed
    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_1MBITS();

    // Set filter to 11-bit CAN IDs
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();  // Accept all IDs for now

    // Install the TWAI driver
    ESP_ERROR_CHECK(twai_driver_install(&g_config, &t_config, &f_config));

    // Start the TWAI driver
    ESP_ERROR_CHECK(twai_start());

    ESP_LOGI("TWAI", "CAN driver installed and started.");

    while (1)
    {
        receive_can_message();
    }
    
}
