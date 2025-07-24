#include "audio_data.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include "arm_math.h"
#include "arm_const_structs.h"
#include "gpio.h"
#include "i2c.h"

#define FFT_SIZE 128 // audio_data 256 elemanlı, real+imag için 128 kullanılabilir

float32_t input_f32[FFT_SIZE * 2];  // [real0, imag0, real1, imag1, ...]
float32_t fft_magnitude[FFT_SIZE];

void convert_audio_to_float(const uint8_t *audio_data, float32_t *output, uint16_t length) {
    for (int i = length - 1; i >= 0; i--) {
        output[2 * i] = ((float32_t)audio_data[i] - 128.0f) / 128.0f; // real
        output[2 * i + 1] = 0.0f; // imag
    }
}

void calculate_fft_magnitude(const float32_t *fft_complex, float32_t *magnitude, uint16_t fft_size) {
    for (int i = 0; i < fft_size; i++) {
        float32_t real = fft_complex[2 * i];
        float32_t imag = fft_complex[2 * i + 1];
        magnitude[i] = sqrtf(real * real + imag * imag);
    }
}

void display_fft_on_oled(const float32_t *magnitude, uint16_t fft_size) {
    ssd1306_Fill(Black);

    ssd1306_SetCursor(0, 0);
    ssd1306_WriteString("FFT", Font_6x8, White);  // başlık

    for (int i = 0; i < fft_size && i < SSD1306_WIDTH; i++) {
        uint8_t bar_height = (uint8_t)(magnitude[i] * 100); // daha büyük ölçekleme
        if (bar_height > SSD1306_HEIGHT - 10) bar_height = SSD1306_HEIGHT - 10;

        for (uint8_t y = 0; y < bar_height; y++) {
            ssd1306_DrawPixel(i, SSD1306_HEIGHT - 1 - y, White);
        }
    }

    ssd1306_UpdateScreen();
}

void process_audio_and_display_fft(void) {
    convert_audio_to_float(audio_data, input_f32, FFT_SIZE);
    arm_cfft_f32(&arm_cfft_sR_f32_len128, input_f32, 0, 1);
    calculate_fft_magnitude(input_f32, fft_magnitude, FFT_SIZE);
    display_fft_on_oled(fft_magnitude, FFT_SIZE);
}

void SystemClock_Config(void);
void Error_Handler(void);

int main(void) {
    HAL_Init();
    SystemClock_Config();

    MX_GPIO_Init();
    MX_I2C1_Init();

    ssd1306_Init();
    ssd1306_SetContrast(0xFF);
    ssd1306_Fill(Black);
    ssd1306_UpdateScreen();

    while (1) {
        process_audio_and_display_fft();
        HAL_Delay(500);
    }
}

/* Clock ayarları */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
        Error_Handler();
    }
}

/* Hata fonksiyonu */
void Error_Handler(void) {
    __disable_irq();
    while (1) {}
}
