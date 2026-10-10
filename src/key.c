#include "key.h"
#include <drivers/pin.h>

#ifndef BSP_KEY1_PIN
#error "This training requires BSP_KEY1_PIN from the selected board configuration"
#endif
#ifndef BSP_KEY1_ACTIVE_HIGH
#error "This training requires BSP_KEY1_ACTIVE_HIGH from the board configuration"
#endif

static struct key_edge_state key1_state;

void key_edge_init(struct key_edge_state *state, rt_bool_t pressed)
{
    state->was_pressed = pressed ? RT_TRUE : RT_FALSE;
}

rt_bool_t key_edge_update(struct key_edge_state *state, rt_bool_t pressed)
{
    rt_bool_t current = pressed ? RT_TRUE : RT_FALSE;
    rt_bool_t event = current && !state->was_pressed;
    state->was_pressed = current;
    return event;
}

rt_bool_t key_edge_selftest(void)
{
    struct key_edge_state state;
    const rt_bool_t samples[] = {RT_FALSE, RT_TRUE, RT_TRUE, RT_FALSE, RT_TRUE};
    const rt_bool_t expected[] = {RT_FALSE, RT_TRUE, RT_FALSE, RT_FALSE, RT_TRUE};
    unsigned int i;

    key_edge_init(&state, RT_FALSE);
    for (i = 0; i < sizeof(samples) / sizeof(samples[0]); i++)
    {
        if (key_edge_update(&state, samples[i]) != expected[i])
            return RT_FALSE;
    }

    /* A key already held at initialization must first be released. */
    key_edge_init(&state, RT_TRUE);
    if (key_edge_update(&state, RT_TRUE)
        || key_edge_update(&state, RT_FALSE)
        || !key_edge_update(&state, RT_TRUE))
        return RT_FALSE;

    return RT_TRUE;
}

static rt_bool_t key1_pressed(void)
{
    return rt_pin_read(BSP_KEY1_PIN) == BSP_KEY1_ACTIVE_HIGH;
}

void key1_init(void)
{
    /* Match the board's existing GPIO input/no-pull configuration. */
    rt_pin_mode(BSP_KEY1_PIN, PIN_MODE_INPUT);
    key_edge_init(&key1_state, key1_pressed());
}

rt_bool_t key1_poll(void)
{
    /* Deliberately no debounce in the first T1 training version. */
    return key_edge_update(&key1_state, key1_pressed());
}
