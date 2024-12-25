// 门禁系统
// ch422g显示当前输入的密码(和当前的模式代码)
// aip1944点阵显示
// 按键输入密码
// NFC卡开门和录卡
// 门禁系统的主要功能是通过密码或者NFC卡开门
// stk陀螺仪检测门禁系统安全状态
// 蜂鸣器负责报警
// 电机模拟开门过程
// led灯显示门禁系统状态
// 温度检测负责门禁系统安全状态

#include "main.h"

int main()
{
    // Create security monitor thread
    pthread_t security_thread;
    pthread_create(&security_thread, NULL, (void *(*)(void *))security_moitor, NULL);

    while (1) {
        if (security_breach) {
            led_boom();  // Show alarm status
            usleep(500000);  // Wait 500ms before next check
            continue;
        }

        unsigned int passwd = read_passwd();
        if (passwd == -1) continue;  // Invalid password due to security breach
        
        if (passwd == admin_passwd) {
            admin_mode();
        } else if (passwd == key_passwd || passwd == nfc_passwd) {
            right_passwd();
        } else {
            wrong_passwd();
        }
    }
    return 0;
}
