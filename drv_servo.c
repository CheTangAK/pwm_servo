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
#ifdef PKG_USING_PWM_SERVO

#include "drv_servo.h"

static rt_err_t servo_init(rt_device_t dev){
    rt_servo_t servo_dev = (rt_servo_t) dev;
    return rt_pwm_set(servo_dev->pwm_device,servo_dev->pwm_channel,servo_dev->period,servo_dev->degree * servo_dev->convert_ratio + servo_dev->min_pos);
}

static rt_err_t servo_open(rt_device_t dev, rt_uint16_t oflag){
    rt_servo_t servo_dev = (rt_servo_t) dev;
    return rt_pwm_enable(servo_dev->pwm_device, servo_dev->pwm_channel);
}

static rt_err_t servo_close(rt_device_t dev){
    rt_servo_t servo_dev = (rt_servo_t) dev;
    return rt_pwm_disable(servo_dev->pwm_device, servo_dev->pwm_channel);
}

static rt_size_t servo_read(rt_device_t dev, rt_off_t pos,void* buffer, rt_size_t size){
    rt_servo_t servo_dev = (rt_servo_t) dev;
    rt_memset(buffer,servo_dev->degree,sizeof(servo_dev->degree));
    return sizeof(servo_dev->degree);
}

static rt_size_t servo_write(rt_device_t dev, rt_off_t pos, const void *buffer, rt_size_t size){
    if(pos > 1800 || pos < 0){
        return 0;
    }
    rt_servo_t servo_dev = (rt_servo_t) dev;
    servo_dev->degree = pos;
    rt_pwm_set(servo_dev->pwm_device,servo_dev->pwm_channel,servo_dev->period,servo_dev->degree * servo_dev->convert_ratio + servo_dev->min_pos);
    return sizeof(servo_dev->degree);
}

static rt_err_t servo_control(rt_device_t dev, int cmd, void *args){
    rt_servo_t servo_dev = (rt_servo_t) dev;
    switch(cmd){
        case RT_SERVO_POWER_OFF:{
            return rt_pwm_disable(servo_dev->pwm_device, servo_dev->pwm_channel);
        }
        case RT_SERVO_POWER_ON:{
            return rt_pwm_enable(servo_dev->pwm_device, servo_dev->pwm_channel);
        }
    }
}

rt_servo_t servo_user_init(char* pwm_name, rt_uint8_t pwm_channel, rt_uint32_t frequency,rt_uint32_t min_pos_width,rt_uint32_t max_pos_width){

    //Check PWM device exists
    struct rt_device_pwm* pwm_device = (struct rt_device_pwm*) rt_device_find(pwm_name);
    RT_ASSERT(pwm_device != RT_NULL)

    //Check frequency
    RT_ASSERT(frequency != 0)

    //Check existing servo with same channel
    rt_uint8_t dev_num = 0;
    char dev_name[RT_NAME_MAX];
    rt_sprintf(dev_name, "servo%d", pwm_channel);
    if(rt_device_find(dev_name)){
        return RT_NULL;
    }

    rt_servo_t dev_obj = rt_malloc(sizeof(struct rt_servo));

    if (dev_obj)
	{
		rt_memset(dev_obj, 0x0, sizeof(struct rt_servo));
        dev_obj->pwm_device = pwm_device;
        dev_obj->pwm_channel = pwm_channel;
        dev_obj->period = 1000000000 / frequency;
        dev_obj->min_pos = min_pos_width;
        dev_obj->convert_ratio = (float) (max_pos_width - min_pos_width) / 1800;
        dev_obj->degree = 900;

        dev_obj->parent.type = RT_Device_Class_Miscellaneous;
        dev_obj->parent.init = servo_init;
        dev_obj->parent.open = servo_open;
        dev_obj->parent.close = servo_close;
        dev_obj->parent.read = servo_read;
        dev_obj->parent.write = servo_write;
        dev_obj->parent.control = servo_control;

		rt_device_register(&(dev_obj->parent), dev_name, RT_DEVICE_FLAG_DEACTIVATE);
		return (rt_servo_t)rt_device_find(dev_name);
    }
    else{
        return RT_NULL;
    }
}

#ifdef PKG_SERVO_USING_KCONFIG

#ifdef PKG_USING_SERVO_1
static rt_servo servo1;
#endif
#ifdef PKG_USING_SERVO_2
static rt_servo servo2;
#endif

static int servo_hw_init(void){
#ifdef PKG_USING_SERVO_1
    servo1 = servo_user_init(PKG_PWM_SERVO_1_PWM_NAME,PKG_PWM_SERVO_1_PWM_CHANNEL,PKG_PWM_SERVO_1_FREQUENCY,PKG_PWM_SERVO_1_MIN_POS_WIDTH,PKG_PWM_SERVO_1_MAX_POS_WIDTH);
#endif
#ifdef PKG_USING_SERVO_2
    servo2 = servo_user_init(PKG_PWM_SERVO_2_PWM_NAME,PKG_PWM_SERVO_2_PWM_CHANNEL,PKG_PWM_SERVO_2_FREQUENCY,PKG_PWM_SERVO_2_MIN_POS_WIDTH,PKG_PWM_SERVO_2_MAX_POS_WIDTH);
#endif
}
INIT_DEVICE_EXPORT(servo_hw_init);
#endif

#endif