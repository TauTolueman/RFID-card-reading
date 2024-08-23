#include <esp_log.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>  // strcpy fonksiyonu için gerekli başlık dosyası
#include "rc522.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "esp_err.h"
#include "driver/gpio.h"
#include <stdbool.h>

char x[] = "n";
char y[] = "n";

extern char Current_Date_Time[25];
char Serial_Number[15];
static rc522_handle_t scanner;
int counter=0;

#define MAX_RFID_RECORDS 300

typedef struct {
    char serial_number_1[15];
    char date_time[25];
    char date_time_1[25];
    char serial_number_2[15];

} rfid_record_t;

typedef struct {
    int count;  // Kaydedilen toplam RFID sayısı
    int start;  // İlk kaydın indeksi
    rfid_record_t records[MAX_RFID_RECORDS];  // RFID kayıtlarının dairesel tamponu
} rfid_storage_t;

static void save_rfid_to_flash(const char* serial_number_1, const char* date_time) {
    nvs_handle_t my_handle;
    esp_err_t err;

    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("NVS open failed\n");
        return;
    }

    rfid_storage_t* rfid_storage = malloc(sizeof(rfid_storage_t));
    if (rfid_storage == NULL) {
        printf("Failed to allocate memory for RFID storage\n");
        nvs_close(my_handle);
        return;
    }

    size_t required_size = sizeof(rfid_storage_t);
    err = nvs_get_blob(my_handle, "rfidStorage", rfid_storage, &required_size);
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        memset(rfid_storage, 0, sizeof(rfid_storage_t));
    } else if (err != ESP_OK) {
        printf("Failed to read rfidStorage from NVS\n");
        free(rfid_storage);
        nvs_close(my_handle);
        return;
    }
    int index = (rfid_storage->start + rfid_storage->count) % MAX_RFID_RECORDS;
    strcpy(rfid_storage->records[index].serial_number_1, serial_number_1);
    strcpy(rfid_storage->records[index].date_time, date_time);

    if (rfid_storage->count < MAX_RFID_RECORDS) {
        rfid_storage->count++;
    } else {
        rfid_storage->start = (rfid_storage->start + 1) % MAX_RFID_RECORDS;
    }
    err = nvs_set_blob(my_handle, "rfidStorage", rfid_storage, sizeof(rfid_storage_t));
    if (err != ESP_OK) {
        printf("Failed to write rfidStorage to NVS\n");
    } else {
        printf("New RFID saved: %s at %s\n", serial_number_1, date_time);
        nvs_commit(my_handle);
    }
    free(rfid_storage);
    nvs_close(my_handle);
}

static void rc522_handler(void* arg, esp_event_base_t base, int32_t event_id, void* event_data) // Main scan code. This function run when new card scanned.
{    
    rc522_event_data_t* data = (rc522_event_data_t*) event_data;

    switch(event_id) {
        case RC522_EVENT_TAG_SCANNED: {
            rc522_tag_t* tag = (rc522_tag_t*) data->ptr;
            int serial_number = tag->serial_number;  // Seri numarasını sakla
            sprintf(Serial_Number, "%d", serial_number);
		
				if(counter == 0 ){
					save_rfid_to_flash(Serial_Number,Current_Date_Time);
					gpio_set_direction(GPIO_NUM_3, GPIO_MODE_OUTPUT);  //This is green led. If it blinks, that means you scan a card.
				    gpio_set_level(GPIO_NUM_3,1);
				    vTaskDelay(500 / portTICK_PERIOD_MS);
				    gpio_set_level(GPIO_NUM_3, 0);
					gpio_set_direction(GPIO_NUM_15, GPIO_MODE_OUTPUT);  //This is buzzer.
				    gpio_set_level(GPIO_NUM_15,1);
				    vTaskDelay(500 / portTICK_PERIOD_MS);
				    gpio_set_level(GPIO_NUM_15, 0);	
				}
				counter++;

			    nvs_handle_t my_handle;
			    esp_err_t err;
			
			    err = nvs_open("storage", NVS_READONLY, &my_handle);
			    if (err != ESP_OK) {
			        printf("NVS open failed\n");
			        return;
			    }
			    rfid_storage_t rfid_storage;
			    size_t required_size = sizeof(rfid_storage_t);
			    err = nvs_get_blob(my_handle, "rfidStorage", &rfid_storage, &required_size);
			    if (err == ESP_ERR_NVS_NOT_FOUND) {
			        printf("No RFID records found.\n");
			        nvs_close(my_handle);
			        return;
			    } else if (err != ESP_OK) {
			        printf("Failed to read rfidStorage from NVS\n");
			        nvs_close(my_handle);
			        return;
			    }
				if(counter != 1){
			    printf("Total RFID records: %d\n", rfid_storage.count);
			    }
				x[0] = 'n';
				y[0] = 'n';
				
			    for (int i = 0; i < rfid_storage.count; i++) {
			        int index = (rfid_storage.start + i) % MAX_RFID_RECORDS;
					
			        int pointer=atoi(rfid_storage.records[index].serial_number_1);
			
			        if(pointer  == serial_number){      //At this if status code check the newly scanned serial number either it is same as saved cards or not.
	
						if(Current_Date_Time[23] > rfid_storage.records[index].date_time[23]){ 
			                //save_rfid_to_flash(Serial_Number , date_time_1);
			               printf("RFID: %s, Date & Time: %s\n", rfid_storage.records[index].serial_number_1, rfid_storage.records[index].date_time);
			               
			               y[0] = 'y';

			            }
			            
			                else if((Current_Date_Time[8]*10 + Current_Date_Time[9]) > (rfid_storage.records[index].date_time[8]*10 + rfid_storage.records[index].date_time[9])){
			                    //save_rfid_to_flash(Serial_Number , date_time_1);
			                    printf("RFID: %s, Date & Time: %s\n", rfid_storage.records[index].serial_number_1, rfid_storage.records[index].date_time);
			                    y[0] = 'y';

			                }
			                
			                    else if(((Current_Date_Time[11]*36000 + Current_Date_Time[12]*3600 + Current_Date_Time[14]*600 + Current_Date_Time[15]*60 + Current_Date_Time[17]*10 + Current_Date_Time[18]) - 
			                    (rfid_storage.records[index].date_time[11]*36000 + rfid_storage.records[index].date_time[12]*3600 + rfid_storage.records[index].date_time[14]*600 + 
			                    rfid_storage.records[index].date_time[15]*60 + rfid_storage.records[index].date_time[17]*10 + rfid_storage.records[index].date_time[18])) > 300 ){

			                        printf("RFID: %s, Date & Time: %s\n", rfid_storage.records[index].serial_number_1, rfid_storage.records[index].date_time);
			                        y[0] = 'y';

			                    }
			                    
			                        else if(counter != 1 && x[0] == 'n'){
											
											printf("RFID: %s, Date & Time: %s\n", rfid_storage.records[index].serial_number_1, rfid_storage.records[index].date_time);
			                                gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);  //This is red led. If it blinks that means you scan same card in five minutes. 
			                                                                                   //You should not scan same card in five minutes.
			                                gpio_set_level(GPIO_NUM_2,1);
			                                vTaskDelay(500 / portTICK_PERIOD_MS);
			                                gpio_set_level(GPIO_NUM_2, 0);
			                                printf("You scan same card in five minutes. Wait for new scanning....\n");

											x[0]= 'y';
											
											gpio_set_direction(GPIO_NUM_15, GPIO_MODE_OUTPUT);  //This is buzzer.
										    gpio_set_level(GPIO_NUM_15,1);
										    vTaskDelay(250 / portTICK_PERIOD_MS);
										    gpio_set_level(GPIO_NUM_15, 0);
										    vTaskDelay(100 / portTICK_PERIOD_MS);
										    gpio_set_level(GPIO_NUM_15,1);
										    vTaskDelay(250 / portTICK_PERIOD_MS);
										    gpio_set_level(GPIO_NUM_15, 0);
			                                
			                        }
					}
					else {
						printf("RFID: %s, Date & Time: %s\n", rfid_storage.records[index].serial_number_1, rfid_storage.records[index].date_time);
								if(i == rfid_storage.count - 1 && x[0] == 'n' && y[0] == 'n'){
										save_rfid_to_flash(Serial_Number,Current_Date_Time);
										gpio_set_direction(GPIO_NUM_3, GPIO_MODE_OUTPUT);  //This is green led. If it blinks, that means you scan a card.
									    gpio_set_level(GPIO_NUM_3,1);
									    vTaskDelay(500 / portTICK_PERIOD_MS);
									    gpio_set_level(GPIO_NUM_3, 0);
										gpio_set_direction(GPIO_NUM_15, GPIO_MODE_OUTPUT);  //This is buzzer.
									    gpio_set_level(GPIO_NUM_15,1);
									    vTaskDelay(500 / portTICK_PERIOD_MS);
									    gpio_set_level(GPIO_NUM_15, 0);	
									    

								}
										 if(y[0] == 'y' && i == rfid_storage.count - 1){
										save_rfid_to_flash(Serial_Number,Current_Date_Time);
										gpio_set_direction(GPIO_NUM_3, GPIO_MODE_OUTPUT);  //This is green led. If it blinks, that means you scan a card.
									    gpio_set_level(GPIO_NUM_3,1);
									    vTaskDelay(300 / portTICK_PERIOD_MS);
									    gpio_set_level(GPIO_NUM_3, 0);
										gpio_set_direction(GPIO_NUM_15, GPIO_MODE_OUTPUT);  //This is buzzer.
									    gpio_set_level(GPIO_NUM_15,1);
									    vTaskDelay(600 / portTICK_PERIOD_MS);
									    gpio_set_level(GPIO_NUM_15, 0);	
										}
					}  
			    }

			    nvs_close(my_handle);


			int erasing_num = 977716643;                   //Özel kartı okuttuğumuzda çalışan NVS yi temizleyen kod
			if (serial_number == erasing_num)
			{
			counter = 0;	
			static const char *TAG = "NVS_ERASE";
			    esp_err_t err;		
			    // Initialize the NVS
			    err = nvs_flash_init();
			    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
			        ESP_ERROR_CHECK(nvs_flash_erase());
			        err = nvs_flash_init();
			    }
			    ESP_ERROR_CHECK(err);		
			    // Erase the NVS
			    err = nvs_flash_erase();
			    if (err == ESP_OK) {
			        ESP_LOGI(TAG, "NVS erased successfully");
			    } else {
			        ESP_LOGE(TAG, "Failed to erase NVS: %s", esp_err_to_name(err));
			    }	
			    // Re-initialize the NVS
			    err = nvs_flash_init();
			    ESP_ERROR_CHECK(err);
			}
            break;
        }
    }
}

void rfid_func(void)
{
    rc522_config_t config = {
        .spi.host = SPI2_HOST,
        .spi.miso_gpio = 19,
        .spi.mosi_gpio = 23,
        .spi.sck_gpio = 18,
        .spi.sda_gpio = 5,
    };
    rc522_create(&config, &scanner);
    rc522_register_events(scanner, RC522_EVENT_ANY, rc522_handler, NULL);
    rc522_start(scanner);
}