import paho.mqtt.client as mqtt
import time
from datetime import datetime
import sys

# تنظیمات MQTT Broker
BROKER_ADDRESS = "192.168.1.147"  # آدرس بروکر MQTT
#BROKER_ADDRESS = "11.42.0.1"  # آدرس بروکر MQTT
BROKER_PORT = 1883            # پورت بروکر MQTT
USERNAME = "your_username"     # نام کاربری (اگر نیاز باشد)
PASSWORD = "your_password"     # رمز عبور (اگر نیاز باشد)
TOPIC = "codeRed/heartBox1/display1"  # تاپیک MQTT

# تابع ایجاد و تنظیم کلاینت MQTT
def create_mqtt_client():
    client = mqtt.Client()
    client.username_pw_set(USERNAME, PASSWORD)
    
    try:
        client.connect(BROKER_ADDRESS, BROKER_PORT, 60)
        return client
    except Exception as e:
        print(f"Error connecting to MQTT broker: {e}")
        sys.exit(1)

# تابع اصلی ارسال پیام‌های زمانی
def send_timed_messages():
    client = create_mqtt_client()
    client.loop_start()  # شروع حلقه شبکه در پس‌زمینه
    
    try:
        print("Starting to send time messages...")
        
        while True:
            # دریافت زمان فعلی
            now = datetime.now()
            minute = now.minute
            second = now.second
            
            # ساخت پیام با فرمت MMSS
            message = f"{minute:02d}{second:02d}"
            
            # ارسال پیام با QoS 3
            result = client.publish(TOPIC, message, qos=2)
            
            # بررسی نتیجه ارسال
            if result.rc == mqtt.MQTT_ERR_SUCCESS:
                print(f"Sent to {TOPIC} (QoS 2): {message} - {datetime.now().strftime('%H:%M:%S')}")
            else:
                print(f"Failed to send to {TOPIC}: {result.rc}")
            
            # محاسبه زمان باقیمانده تا ثانیه بعد
            current_millisecond = datetime.now().microsecond // 1000
            sleep_time = (1000 - current_millisecond) / 1000.0
            
            # انتظار برای ثانیه بعد
            time.sleep(sleep_time)
            
    except KeyboardInterrupt:
        print("\nStopping message sender...")
    finally:
        client.loop_stop()
        client.disconnect()
        print("MQTT client disconnected.")

if __name__ == "__main__":
    send_timed_messages()
