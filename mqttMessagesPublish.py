import paho.mqtt.client as mqtt
import threading

# تنظیمات MQTT Broker
broker = "10.42.0.1"  # آدرس بروکر MQTT
port = 1883  # پورت بروکر MQTT
username = "your_username"  # نام کاربری (اگر نیاز باشد)
password = "your_password"  # رمز عبور (اگر نیاز باشد)

# لیست تاپیک‌ها و پیام‌ها
topics = [
    "codeRed/powerPanel/servo1", "codeRed/powerPanel/servo2", "codeRed/powerPanel/servo3", "codeRed/powerPanel/servo4",
    "codeRed/powerPanel/servo5", "codeRed/powerPanel/servo6", "codeRed/powerPanel/servo7", "codeRed/powerPanel/servo8"
]
messages = [
    "-67", "-45", "-22", "0",
    "22", "45", "67", "90"
]

# تابع برای ارسال پیام به یک تاپیک
def publish_message(topic, message):
    client = mqtt.Client()
    client.username_pw_set(username, password)  # If authentication is required
    client.connect(broker, port, 60)
    
    # Publish the message with QoS 2
    result = client.publish(topic, message, qos=2)
    
    # Check if the message was published successfully
    if result.rc == mqtt.MQTT_ERR_SUCCESS:
        print(f"Sent to {topic} (QoS 2): {message}")
    else:
        print(f"Failed to send to {topic}: {result.rc}")
    
    client.disconnect()

# ایجاد و اجرای Threadها برای ارسال همزمان
threads = []
for i in range(8):
    thread = threading.Thread(target=publish_message, args=(topics[i], messages[i]))
    threads.append(thread)
    thread.start()

# منتظر ماندن تا تمام Threadها تمام شوند
for thread in threads:
    thread.join()

print("All messages sent successfully.")
