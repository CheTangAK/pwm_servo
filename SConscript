import rtconfig
from building import *

cwd = GetCurrentDir()
src = ['drv_servo.c']
CPPPATH = [cwd]
    
group = DefineGroup('pwm_servo', src, depend = [''], CPPPATH = CPPPATH)

Return('group')
