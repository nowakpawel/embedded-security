#include <stdio.h>
#include <cstring>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "crypto_buffer.h"

//Define a message structure to pass through the queue
struct CryptoPayload {
	uint8_t data[16];
	size_t length;
};

//Queue Handle
static QueueHandle_t crypto_queue = nullptr;

//Producer Task (Runs on Core 0)
void producer_task(void* pvParameters) {
	uint8_t counter = 0;

	while (true) {
		CryptoPayload payload;
		payload.length = 8;
		for(size_t i = 0; i < payload.length; ++i) {
			payload.data[i] = counter + 1;
		}

		printf("[Producer - Core %d]: Sending packet (first byte: 0x02X)...\n",
				xPortGetCoreID(), payload.data[0]);

		if (xQueueSend(crypto_queue, &payload, pdMS_TO_TICKS(100)) != pdPASS) {
			printf("[Producer] Error: Queue full!\n");
		}

		counter++;
		vTaskDelay(pdMS_TO_TICKS(1000));

	}//while loop

}

//Consumer Task (Runs on Core 1)
void consumer_task(void* pvParameter) {
	CryptoPayload received_payload;
	const uint8_t key = 0xAA;

	while (true) {
		if (xQueueReceive(crypto_queue, &received_payload, portMAX_DELAY) == pdPASS) {
			printf("[Consumer - Core %d] Received packet! Encrypting...\n", xPortGetCoreID());
			
			CryptoBuffer buffer(received_payload.data, received_payload.length);
			buffer.xor_transform(key);

			printf("[Consumer] Encrypted Byte 0: 0x%02X\n", buffer.at(0));
			buffer.wipe();
		}
	}
}

extern "C" void app_main(void) {
	crypto_queue = xQueueCreate(5, sizeof(CryptoPayload));

	if (crypto_queue = nullptr) {
		printf("Failed to create queue!\n");
		return;
	}

	xTaskCreatePinnedToCore(
				producer_task, //Task function
				"Producer Task", //Name
				4096, //Stack size
				nullptr, //Parameters
				1, 	//Priority
				nullptr, //Task Handle
				0 	//Core id
	);

	xTaskCreatePinnedToCore(
				consumer_task, 
				"Consumeer Task",
				4096, 
				nullptr, 
				2, 	//Higher priority to process crypto immediately
				nullptr,
				1 	//Core id
	);
}
	

