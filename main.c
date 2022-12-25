#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/spi.h"
#include "../usbkb/usbkeyboard.h"
#include "../lcd-lib/LCDdriver.h"
#include "../lcd-lib/graphlib.h"
#include "../lcd-lib/ff.h"
#include "editor.h"
#include "keyinput.h"

unsigned char RAM[RAMSIZE];

//ダミー関数
void init_compiler(void){
	return;
}
int compile_file(unsigned char *p){
	return 0;
}
void pre_run(void){
	return;
}
void run_code(void){
	return;
}
void post_run(void){
	return;
}

void core1_entry(void){
  while(1){
    usbkb_polling();
    sleep_us(100);
  }
}

FATFS FatFs; /* FatFs work area needed for each volume */

int main(void) {
	stdio_init_all();

	// 液晶用ポート設定
	// Enable SPI at 32 MHz and connect to GPIOs
	spi_init(LCD_SPICH, 32000 * 1000);
	gpio_set_function(LCD_SPI_RX, GPIO_FUNC_SPI);
	gpio_set_function(LCD_SPI_TX, GPIO_FUNC_SPI);
	gpio_set_function(LCD_SPI_SCK, GPIO_FUNC_SPI);

	gpio_init(LCD_CS);
	gpio_put(LCD_CS, 1);
	gpio_set_dir(LCD_CS, GPIO_OUT);
	gpio_init(LCD_DC);
	gpio_put(LCD_DC, 1);
	gpio_set_dir(LCD_DC, GPIO_OUT);
	gpio_init(LCD_RESET);
	gpio_put(LCD_RESET, 1);
	gpio_set_dir(LCD_RESET, GPIO_OUT);

	init_textgraph(HORIZONTAL); //液晶初期化、テキスト利用開始
//	init_textgraph(VERTICAL); //液晶初期化、テキスト利用開始

	f_mount(&FatFs, "", 0); /* Give a work area to the default drive */

	gpio_init(PICO_DEFAULT_LED_PIN);  // on board LED
	gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    lockkey=1; // 下位3ビットが<SCRLK><CAPSLK><NUMLK>
    keytype=0; // 0：日本語109キー、1：英語104キー
	if(!usbkb_init()){
		return 1;
	}
    printstr("Init USB OK\n");
	multicore_launch_core1(core1_entry);
	while(!usbkb_mounted()) //USBキーボードの接続待ち
		sleep_ms(16);
    printstr("USB keyboard found\n");
	sleep_ms(500); //0.5秒待ち

	cls();
	EDITORRAM=(unsigned char *)RAM;
    texteditor();

    return 0;
}
