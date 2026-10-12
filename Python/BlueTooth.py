import serial
import keyboard
# 配置区
COM_PORT = "COM6"
BAUD_RATE = 9600   #波特率
# 按键配置：按键名 → (包头, 数据字节列表)
# 0xAA类字母键：大写触发加速帧，小写触发普通帧
key_config = {
    "w": (0xAA, [0x01]),
    "s": (0xAA, [0x02]),
    "a": (0xAA, [0x03]),
    "d": (0xAA, [0x04]),
    "q": (0xAA, [0x05]),
    "e": (0xAA, [0x07]),
    "j": (0xCC, [0x01]),
    "k": (0xCC, [0x02]),
    "l": (0xCC, [0x03]),
    "up": (0xBB, [0x01]),
    "down": (0xBB, [0x02]),
    "left": (0xBB, [0x03]),
    "right": (0xBB, [0x04]),
}
# 小键盘扫描码配置：扫描码 → (包头, 数据字节列表)
# 0xBB类指令不受Shift影响，不加挡位
scan_map = {
    79: (0xBB, [0x05]),   #小键盘1
    80: (0xBB, [0x06]),   #小键盘2
    81: (0xBB, [0x07]),   #小键盘3
}
# 数据包 → 控制台显示文字对照表
data_desc = {
    b'\xaa\x05\x01\x00\xb0': "前进",
    b'\xaa\x05\x02\x00\xb1': "后退",
    b'\xaa\x05\x03\x00\xb2': "向左",
    b'\xaa\x05\x04\x00\xb3': "向右",
    b'\xaa\x05\x05\x00\xb4': "左转",
    b'\xaa\x05\x07\x00\xb6': "右转",
    b'\xaa\x05\x01\x01\xb1': "前进(加速)",
    b'\xaa\x05\x02\x01\xb2': "后退(加速)",
    b'\xaa\x05\x03\x01\xb3': "向左(加速)",
    b'\xaa\x05\x04\x01\xb4': "向右(加速)",
    b'\xaa\x05\x05\x01\xb5': "左转(加速)",
    b'\xaa\x05\x07\x01\xb7': "右转(加速)",
    b'\xbb\x04\x01\xc0': "机械臂向前",
    b'\xbb\x04\x02\xc1': "机械臂向后",
    b'\xbb\x04\x03\xc2': "机械臂向左",
    b'\xbb\x04\x04\xc3': "机械臂向右",
    b'\xbb\x04\x05\xc4': "机械臂上升",
    b'\xbb\x04\x06\xc5': "机械臂向下",
    b'\xbb\x04\x07\xc6': "夹爪张合",
}
def build_frame(header, data_list):
    """
    自动构建串口帧
    帧结构：[包头][总帧长][数据...][校验和]
    校验规则：包头 + 总帧长 + 所有数据字节 求和取低8位
    """
    total_len = 2 + len(data_list) + 1  # 包头1 + 长度1 + 数据N + 校验1 = 总字节数
    frame_body = [header, total_len] + data_list
    checksum = sum(frame_body) & 0xFF
    return bytes(frame_body + [checksum])
try:
    ser = serial.Serial(COM_PORT, BAUD_RATE, timeout=0.1)
    print(f"成功打开串口 {COM_PORT}")
    print("[AA类字母键] 小写=普通帧,按住Shift+字母(大写)=加速帧")
    print("[BB类方向键/小键盘] 不受Shift影响,格式不变")
    print("[CC(j/k/l)] 机构扩展指令,不受Shift,不加挡位")
    print("按ESC退出程序")
    def on_event(event):
        sc = event.scan_code
        key_name = event.name
        send_data = None
        # ESC退出
        if key_name.lower() == "esc" and event.event_type == keyboard.KEY_DOWN:
            raise KeyboardInterrupt
        # 只在按键按下时发包，松开不处理
        if event.event_type != keyboard.KEY_DOWN:
            return
        # 互斥区分：小键盘走扫描码（BB包头，不加挡位）
        if event.is_keypad:
            if sc in scan_map:
                hdr, data = scan_map[sc]
                send_data = build_frame(hdr, data)
        else:
            # 统一转小写查配置表
            key_lower = key_name.lower()
            if key_lower in key_config:
                hdr, base_data = key_config[key_lower]
                # AA包头字母键：按键名是大写 → 加速挡位0x01；小写 → 普通挡位0x00
                if hdr == 0xAA:
                    gear = 0x01 if key_name.isupper() else 0x00
                    full_data = base_data + [gear]
                    send_data = build_frame(hdr, full_data)
                # BB / CC包头：原样发送，不加挡位
                else:
                    send_data = build_frame(hdr, base_data)
        if send_data is not None:
            ser.write(send_data)
            desc = data_desc.get(send_data, "未知指令")
            print(f"发送：{desc} | hex:{send_data.hex()}")
    keyboard.hook(on_event)
    keyboard.wait("esc")
except Exception as e:
    print("错误：", e)
finally:
    if 'ser' in locals() and ser.is_open:
        ser.close()
        print("串口已关闭")
