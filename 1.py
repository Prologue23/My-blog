import keyboard

def callback(event):
    print(f"event.name = {repr(event.name)} | scan_code = {event.scan_code}")

print("开始监听,按各个按键,ESC退出")
keyboard.on_press(callback)
keyboard.wait("esc")
