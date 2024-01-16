#include "sbus.h"

static sbus_data_t __sbus_data;

/**
 * @brief 将原始字节数据转换为各个通道的值
 * @param frame 一帧数据，25 bytes
*/
void byte_convert(const uint8_t *frame)
{
    __sbus_data.channel_datas[0]  = ((frame[1] >> 0)  | (frame[2] << 8)) & 0x07FF;
    __sbus_data.channel_datas[1]  = ((frame[2] >> 3)  | (frame[3] << 5)) & 0x07FF;
    __sbus_data.channel_datas[2]  = ((frame[3] >> 6)  | (frame[4] << 2) | (frame[5] << 10)) & 0x07FF;
    __sbus_data.channel_datas[3]  = ((frame[5] >> 1)  | (frame[6] << 7)) & 0x07FF;
    __sbus_data.channel_datas[4]  = ((frame[6] >> 4)  | (frame[7] << 4)) & 0x07FF;
    __sbus_data.channel_datas[5]  = ((frame[7] >> 7)  | (frame[8] << 1) | (frame[9] << 9)) & 0x07FF;
    __sbus_data.channel_datas[6]  = ((frame[9] >> 2)  | (frame[10] << 6)) & 0x07FF;
    __sbus_data.channel_datas[7]  = ((frame[10] >> 5) | (frame[11] << 3)) & 0x07FF;
    __sbus_data.channel_datas[8]  = ((frame[12] >> 0) | (frame[13] << 8)) & 0x07FF;
    __sbus_data.channel_datas[9]  = ((frame[13] >> 3) | (frame[14] << 5)) & 0x07FF;
    __sbus_data.channel_datas[10] = ((frame[14] >> 6) | (frame[15] << 2) | (frame[16] << 10)) & 0x07FF;
    __sbus_data.channel_datas[11] = ((frame[16] >> 1) | (frame[17] << 7)) & 0x07FF;
    __sbus_data.channel_datas[12] = ((frame[17] >> 4) | (frame[18] << 4)) & 0x07FF;
    __sbus_data.channel_datas[13] = ((frame[18] >> 7) | (frame[19] << 1) | (frame[20] << 9)) & 0x07FF;
    __sbus_data.channel_datas[14] = ((frame[20] >> 2) | (frame[21] << 6)) & 0x07FF;
    __sbus_data.channel_datas[15] = ((frame[21] >> 5) | (frame[22] << 3)) & 0x07FF;
    __sbus_data.channel_datas[16]        = (frame[23] & 0x80) >> 7;
    __sbus_data.channel_datas[17]        = (frame[23] & 0x40) >> 6;
    __sbus_data.frame_lost_flag   = (frame[23] & 0x20) >> 5;
    __sbus_data.failsafe_flag     = (frame[23] & 0x10) >> 4;
}


/**
 * @brief 实现获取一帧数据的逻辑
 * @param buff 缓存区，缓冲区应当 >= 25bytes
 * @param sbus_data_p 解析后的数据的指针
 * */ 
int resolve_sbus_frame(const uint8_t *buff) {
    // 检查帧头是否正确
    if(buff[0] != 0x0F || buff[24] != 0x00)
    {
        return -1;
    }
    
    // 转换数据
    byte_convert(buff);
    return 0;
}


/**
 * @brief 获取指定SBUS通道的数据
 *
 * @param channel_id 要获取的通道ID，范围为 [1, 18]
 * @param channel_data 存储获取到的通道值的指针
 * 
 * @return 返回-1表示失败（channel_id不在范围内），读取成功则返回通道值
 */
int get_sbus_channel(int channel_id)
{
    if (channel_id < 1 || channel_id > 18)
    {
        return -1;
    }

    return __sbus_data.channel_datas[channel_id - 1];
}


int get_sbus_frame_lost_flag(void)
{
    return __sbus_data.frame_lost_flag;
}


int get_sbus_failsafe_flag(void)
{
    return __sbus_data.failsafe_flag;
}


/** 
 * @brief 复制所有的通道值到指定位置
 * @param channel_data 接受通道值，数组应当 >= 18
 * */ 
int get_all_channels(int16_t *channel_data)
{
    if (channel_data == NULL)
    {
        return -1;
    }

    if (sizeof(channel_data) < 18)
    {
        return -1;
    }

    memcpy(channel_data, __sbus_data.channel_datas, 18 * sizeof(int16_t));
    return 0;
}


int get_frame_bytes(const uint8_t *buff, uint8_t *frame_bytes)
{
    if (buff == NULL)
    {
        return -1;
    }

    memcpy(frame_bytes, buff, 25);
    return 0;
}

