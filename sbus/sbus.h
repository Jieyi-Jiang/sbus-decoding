#ifndef SBUS_H_
#define SBUS_H_

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct sbus_data_t
{
    uint16_t channel_datas[18];
    uint8_t frame_lost_flag;
    uint8_t failsafe_flag;
} sbus_data_t;

extern sbus_data_t sbus_data;

int resolve_sbus_frame(const uint8_t *buff);
int get_frame_bytes(const uint8_t *buff, uint8_t *frame_bytes);
int get_sbus_channel(int channel_id);
int get_sbus_frame_lost_flag(void);
int get_sbus_failsafe_flag(void);
int get_all_channels(int16_t *channel_data);

#endif

