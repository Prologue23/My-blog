import serial
import keyboard

# 配置区
COM_PORT = "COM6"
BAUD_RATE = 9600   #波特率

key_config = {
    "w": b'\xAA',
    "s": b'\xAA',
    "a": b'\xAA',
    "d": b'\xAA',
    "space": b'\xAA',
    "up": b'\xBB\x04\x01\xC0',
    "down": b'\xBB\x04\x02\xC1',
    "left": b'\xBB\x04\x03\xC2',
    "right": b'\xBB\x04\x04\xC3',
}

scan_map = {
    79: b'\xBB\x04\x05\xC4',   #小键盘1
    80: b'\xBB\x04\x06\xC5',   #小键盘2
    81: b'\xBB\x04\x07\xC6'    #小键盘3
}

# 数据包→显示文字对照表（全局，不要放进函数里）
data_desc = {
    b'\xAA': "通用指令AA",
    b'\xBB\x04\x01\xC0': "方向上",
    b'\xBB\x04\x02\xC1': "方向下",
    b'\xBB\x04\x03\xC2': "方向左",
    b'\xBB\x04\x04\xC3': "方向右",
    b'\xBB\x04\x05\xC4': "小键盘1",
    b'\xBB\x04\x06\xC5': "小键盘2",
    b'\xBB\x04\x07\xC6': "小键盘3",
}

try:
    ser = serial.Serial(COM_PORT, BAUD_RATE, timeout=0.1)
    print(f"成功打开串口 {COM_PORT}")
    print("按键列表：")
    for k,v in key_config.items():
        print(f"按下 {k} →发送 {repr(v)}")
    print("小键盘1/2/3 使用扫描码79/80/81")
    print("按ESC退出程序")

    def on_key(event):
        sc = event.scan_code
        key_name = event.name
        send_data = None

        if key_name in key_config:
            send_data = key_config[key_name]
        if sc in scan_map:
            send_data = scan_map[sc]

        if send_data is not None:
            ser.write(send_data)
            desc = data_desc.get(send_data, "未知指令")
            print(f"发送 hex:{send_data.hex()} | {desc}")

    keyboard.on_press(on_key)
    keyboard.wait("esc")

except Exception as e:
    print("错误：", e)
finally:
    if 'ser' in locals() and ser.is_open:
        ser.close()
        print("串口已关闭")
