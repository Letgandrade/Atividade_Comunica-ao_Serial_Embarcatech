#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "hardware/uart.h"
#include "hardware/pio.h"
#include "inc/ssd1306.h"
#include "inc/font.h"
#include "ws2812.pio.h"

// Definições de pinos e configurações do display
#define BUTTON_A 5
#define BUTTON_B 6
#define LED_R 13
#define LED_G 11
#define LED_B 12
#define I2C_SDA 14
#define I2C_SCL 15
#define I2C_PORT i2c1
#define endereco 0x3C

// Definições da matriz 5x5 de LEDs WS2812
#define MATRIX_PIN 7

// Definições da UART
#define UART_ID uart0
#define BAUD_RATE 9600

// Variáveis para controlar o estado dos LEDs
bool led_a_state = false;
bool led_b_state = false;

// Inicializa o display SSD1306
ssd1306_t ssd;

// Inicializa a matriz WS2812
void setup_ws2812() {
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, MATRIX_PIN, 800000, false);
}

void display_number_on_matrix(int number) {
    // Definindo a representação binária de 0-9 em uma matriz 5x5
    uint8_t matrix_symbols[10][5] = {
        {0b11111, 0b10001, 0b10001, 0b10001, 0b11111},  // 0
        {0b11111, 0b00100, 0b00100, 0b01100, 0b00100},  // 1
        {0b11111, 0b10000, 0b11111, 0b00001, 0b11111},  // 2
        {0b11111, 0b00001, 0b11111, 0b00001, 0b11111},  // 3
        {0b10000, 0b00001, 0b11111, 0b10001, 0b10001},  // 4
        {0b11111, 0b00001, 0b11111, 0b10000, 0b11111},  // 5
        {0b11111, 0b10001, 0b11111, 0b10000, 0b11111},  // 6
        {0b00001, 0b01000, 0b00100, 0b00010, 0b11111},  // 7
        {0b11111, 0b10001, 0b11111, 0b10001, 0b11111},  // 8
        {0b11111, 0b00001, 0b11111, 0b10001, 0b11111},  // 9
    };

    PIO pio = pio0;
    int sm = 0;

    // Envia os dados para a matriz de LEDs
    for (int i = 0; i < 5; i++) {
        uint8_t row = matrix_symbols[number][i];
        for (int j = 0; j < 5; j++) {
            bool led_state = (row & (1 << (4 - j))) != 0;
            pio_sm_put_blocking(pio, sm, led_state ? 0x00FF00 : 0x000000);
        }
    }
}

void gpio_callback(uint gpio, uint32_t events) {
    if (gpio == BUTTON_A) {
        led_a_state = !led_a_state;
        gpio_put(LED_G, led_a_state);
        printf("Botão A pressionado! LED verde: %s\n", led_a_state ? "LIGADO" : "DESLIGADO");
        
        ssd1306_fill(&ssd, false);
        ssd1306_draw_string(&ssd, "LED VERDE:", 8, 10);
        ssd1306_draw_string(&ssd, led_a_state ? "LIGADO" : "DESLIGADO", 8, 30);
        ssd1306_send_data(&ssd);
    } else if (gpio == BUTTON_B) {
        led_b_state = !led_b_state;
        gpio_put(LED_B, led_b_state);
        printf("Botão B pressionado! LED azul: %s\n", led_b_state ? "LIGADO" : "DESLIGADO");
        
        ssd1306_fill(&ssd, false);
        ssd1306_draw_string(&ssd, "LED AZUL:", 8, 10);
        ssd1306_draw_string(&ssd, led_b_state ? "LIGADO" : "DESLIGADO", 8, 30);
        ssd1306_send_data(&ssd);
    }
}

void handle_uart_input() {
    int input_char = getchar_timeout_us(0); 

    // Se a leitura não retornou erro (-1), significa que há um caractere disponível
    if (input_char != PICO_ERROR_TIMEOUT) {
        printf("Caractere recebido: %c\n", input_char);
        
        // Atualiza o display OLED com o caractere recebido
        ssd1306_fill(&ssd, false);
        ssd1306_draw_string(&ssd, "CARACTERE:", 8, 10);
        char str[2] = {input_char, '\0'};
        ssd1306_draw_string(&ssd, str, 8, 30);
        ssd1306_send_data(&ssd);

        // Exibe o número na matriz de LEDs se for um dígito
        if (input_char >= '0' && input_char <= '9') {
            display_number_on_matrix(input_char - '0');
        }
    }
}

void setup_uart() {
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(0, GPIO_FUNC_UART);
    gpio_set_function(1, GPIO_FUNC_UART);
}

void setup() {
    stdio_init_all();
    
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);

    gpio_init(LED_R);
    gpio_init(LED_G);
    gpio_init(LED_B);
    gpio_set_dir(LED_R, GPIO_OUT);
    gpio_set_dir(LED_G, GPIO_OUT);
    gpio_set_dir(LED_B, GPIO_OUT);
    
    //setup_uart();
    setup_ws2812();
    
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);
    
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_RISE, true, &gpio_callback);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_RISE, true, &gpio_callback);
}

int main() {
    setup();
    while (1) {
        if (stdio_usb_connected()) {
            handle_uart_input();
        }
        tight_loop_contents();
    }
    return 0;
} 