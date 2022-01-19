# PWM Servo
Servo device RT-Thread package for the HKUST Intelligent Racing Team

## Installation
1. Go to your env/packages/custom_packages/peripherals, add a new folder called "pwm_servo", copy Kconfig and package.json into the folder
2. Edit your env/packages/custom_packages/peripherals/Kconfig, add a line ```source "$PKGS_DIR/custom_packages/peripherals/pwm_servo/Kconfig"```
3. In your RT-Thread setting, enable pwm_servo under custom online packages
4. Enter ```pkgs --update`` in your terminal

## Usage
Find the device
```c=
rt_device_t servo = rt_device_find("servo0");
```

Open the device
```c=
rt_device_open(servo,0);
```

Set Degree
```c=
rt_device_write(servo,1020,RT_NULL,RT_NULL);
```
Degree is between 0 and 1800, 900 is the middle position, or speed 0 for motors.
