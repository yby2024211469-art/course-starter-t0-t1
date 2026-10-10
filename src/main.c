/*
 * T1：KEY1 短按切换黄山派板载 RGB 灯。
 *
 * 基于 SiFli-SDK 官方例程（Apache-2.0）：
 *   example/get-started/hello_world/rtt   —— 启动打印
 *   example/rt_device/rgb_led             —— RGB 灯驱动用法（SK6812MINI-HS，PA32，PWMT2 通道1）
 * 板型：sf32lb52-lchspi-ulp（立创·黄山派）。
 *
 * 行为：启动后保持红色，每次 KEY1 按下按红→绿→蓝切换并打印计数。
 * 第一版每 5ms 采样，只检测按下边沿，不做消抖。
 *
 * 新增 .c 文件直接放进 src/ 即可，src/SConscript 会自动收集。
 */
#include "rtthread.h"
#include "bf0_hal.h"
#include "drv_io.h"
#include "stdio.h"
#include "string.h"
#include "drivers/rt_drv_pwm.h"
#include "key.h"

#define APP_NAME        "course_starter"
#define APP_VERSION     "0.2.0-t1"

#define RGBLED_NAME     "rgbled"
#define KEY_POLL_MS     5

struct rgb_color
{
    const char *name;
    uint32_t    value;      /* 0xRRGGBB */
};

static const struct rgb_color color_table[] =
{
    {"red",   0xff0000},
    {"green", 0x00ff00},
    {"blue",  0x0000ff},
};

static struct rt_device *rgbled_device;

static unsigned int next_color(unsigned int index)
{
    return (index + 1) % (sizeof(color_table) / sizeof(color_table[0]));
}

static rt_bool_t color_selftest(void)
{
    unsigned int index = 0;
    index = next_color(index);
    if (index != 1) return RT_FALSE;
    index = next_color(index);
    if (index != 2) return RT_FALSE;
    index = next_color(index);
    return index == 0;
}

static void rgb_led_init(void)
{
#ifdef SF32LB52X
    HAL_PIN_Set(PAD_PA32, GPTIM2_CH1, PIN_NOPULL, 1);   /* pwmt2_cc1 */
    /* RGB 灯供电 */
    HAL_PMU_ConfigPeriLdo(PMU_PERI_LDO3_3V3, true, true);
#endif
    rgbled_device = rt_device_find(RGBLED_NAME);
    if (!rgbled_device)
    {
        rt_kprintf("[%s] ERROR: device '%s' not found, check proj.conf RGB options\n", APP_NAME, RGBLED_NAME);
        RT_ASSERT(0);
    }
}

static void rgb_led_set_color(uint32_t color)
{
    struct rt_rgbled_configuration configuration;
    configuration.color_rgb = color;
    rt_device_control(rgbled_device, PWM_CMD_SET_COLOR, &configuration);
}

int main(void)
{
    unsigned int color_index = 0;
    rt_uint32_t press_count = 0;
    rt_bool_t key_ok;
    rt_bool_t color_ok;

    rt_kprintf("\n[%s] v%s boot ok\n", APP_NAME, APP_VERSION);
    rt_kprintf("Hello world!\n");

    key_ok = key_edge_selftest();
    color_ok = color_selftest();
    rt_kprintf("[t1:selftest] key_edge=%s color_cycle=%s\n",
               key_ok ? "PASS" : "FAIL", color_ok ? "PASS" : "FAIL");
    if (!key_ok || !color_ok)
    {
        rt_kprintf("[t1] ERROR: logic selftest failed\n");
        return -1;
    }

    rgb_led_init();
    key1_init();
    rgb_led_set_color(color_table[color_index].value);
    rt_kprintf("[t1] ready pin=%d active=%d poll_ms=%d debounce=off\n",
               BSP_KEY1_PIN, BSP_KEY1_ACTIVE_HIGH, KEY_POLL_MS);
    rt_kprintf("[t1] count=0 rgb -> %s\n", color_table[color_index].name);

    while (1)
    {
        if (key1_poll())
        {
            color_index = next_color(color_index);
            press_count++;
            rgb_led_set_color(color_table[color_index].value);
            rt_kprintf("[t1] count=%lu rgb -> %s\n",
                       (unsigned long)press_count, color_table[color_index].name);
        }
        rt_thread_mdelay(KEY_POLL_MS);
    }
    return 0;
}
