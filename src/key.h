#ifndef COURSE_KEY_H
#define COURSE_KEY_H

#include <rtthread.h>

struct key_edge_state
{
    rt_bool_t was_pressed;
};

void key_edge_init(struct key_edge_state *state, rt_bool_t pressed);
rt_bool_t key_edge_update(struct key_edge_state *state, rt_bool_t pressed);
rt_bool_t key_edge_selftest(void);
void key1_init(void);
rt_bool_t key1_poll(void);

#endif
