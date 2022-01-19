/*
MIT License

Copyright (c) 2022 CheTangAK

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef __DRV_SERVO_H__
#define __DRV_SERVO_H__

#include <rtthread.h>
#include <rtdevice.h>

struct rt_servo{
	struct rt_device parent;
	struct rt_device_pwm* pwm_device;
    rt_uint8_t pwm_channel;
    rt_uint32_t period;
    float min_pos;
	float convert_ratio;
	rt_uint16_t degree;
};

typedef struct rt_servo* rt_servo_t;

#define RT_SERVO_POWER_OFF 0x01
#define RT_SERVO_POWER_ON 0x02

#endif