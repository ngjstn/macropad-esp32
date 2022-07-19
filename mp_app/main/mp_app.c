#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 
#include <sys/time.h> 
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "ssd1306.h"
#include "font8x8_basic.h"

#define driver "SSD1306"

struct tm *get_time(void)
{
    struct timeval tv; 
    time_t t; 
    // struct tm *info; 
    gettimeofday(&tv, NULL); 
    t = tv.tv_sec; 
    return localtime(&t); 
}


void lcd_startup(void)
{
    SSD1306_t lcd;
	int center, top, bottom;
	char lineChar[20];

#if CONFIG_I2C_INTERFACE
	ESP_LOGI(driver, "INTERFACE is i2c");
	ESP_LOGI(driver, "CONFIG_SDA_GPIO=%d",CONFIG_SDA_GPIO);
	ESP_LOGI(driver, "CONFIG_SCL_GPIO=%d",CONFIG_SCL_GPIO);
	ESP_LOGI(driver, "CONFIG_RESET_GPIO=%d",CONFIG_RESET_GPIO);
	i2c_master_init(&lcd, CONFIG_SDA_GPIO, CONFIG_SCL_GPIO, CONFIG_RESET_GPIO);
#endif

#if CONFIG_FLIP
	lcd._flip = true;
	ESP_LOGW(driver, "Flip upside down");
#endif
    
#if CONFIG_SSD1306_128x32
	ESP_LOGI(driver, "Panel is 128x32");
	ssd1306_init(&lcd, 128, 32);
#endif 

	ssd1306_clear_screen(&lcd, false);
    ssd1306_contrast(&lcd, 0xff);
    ssd1306_display_text(&lcd, 0, "dies", 4, false);
    ssd1306_display_text(&lcd, 1, "  from", 6, false);
    ssd1306_display_text(&lcd, 2, "    cringe", 10, false);
    
    ssd1306_hardware_scroll(&lcd, SCROLL_RIGHT);
	vTaskDelay(5000 / portTICK_PERIOD_MS);
    ssd1306_hardware_scroll(&lcd, SCROLL_LEFT);
    vTaskDelay(5000 / portTICK_PERIOD_MS);

    ssd1306_hardware_scroll(&lcd, SCROLL_STOP); 

	top = 1;
	center = 1;
	bottom = 4;

    struct tm *time_info = get_time();
    printf("%s", asctime(time_info));
    // char buffer[64] = asctime(time_info);
    // printf(buffer);
    // char buffer[64]; 
    // strftime(buffer, sizeof(buffer), "%A, %B, %d \n", time_info);
    // printf(buffer);
    // ESP_LOGI(driver, "TIME: %d", buffer);

	ssd1306_display_text(&lcd, 0, "SSD1306 128x32", 14, false);
	ssd1306_display_text(&lcd, 1, "Hello World!!", 13, false);
	ssd1306_display_text(&lcd, 2, "SSD1306 128x32", 14, true);
	ssd1306_display_text(&lcd, 3, "Hello World!!", 13, true);
	vTaskDelay(3000 / portTICK_PERIOD_MS);
}


void app_main(void)
{
    lcd_startup();
	esp_restart();
}