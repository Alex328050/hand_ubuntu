import socket
import json
import time
import threading


class MockSuperior:
    def __init__(self, host="127.0.0.1", port=8666):
        self.host = host
        self.port = port
        self.running = False
        self.connection = None
        self.receive_thread = None

    def start(self):
        """启动客户端，开始接收数据"""
        self.running = True
        self.receive_thread = threading.Thread(target=self._receive_data, daemon=True)
        self.receive_thread.start()
        print(f"模拟上级已启动，正在连接到 {self.host}:{self.port}...")

    def stop(self):
        """停止客户端"""
        self.running = False
        if self.connection:
            self.connection.close()
        if self.receive_thread and self.receive_thread.is_alive():
            self.receive_thread.join()
        print("模拟上级已停止")

    def _receive_data(self):
        """循环接收数据的线程函数"""
        while self.running:
            try:
                # 创建连接
                with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
                    self.connection = s
                    s.connect((self.host, self.port))
                    print(f"成功连接到 {self.host}:{self.port}")

                    # 接收数据
                    while self.running:
                        data = s.recv(4096)  # 接收缓冲区大小
                        if not data:
                            print("连接已关闭，尝试重新连接...")
                            break

                        # 解析数据
                        try:
                            # 先将字节数据解码为字符串
                            str_data = data.decode("utf-8").strip()

                            # 处理初始数据（非JSON格式）
                            if str_data == "Initialing...":
                                print("\n" + "=" * 50)
                                print(f"接收时间: {time.strftime('%Y-%m-%d %H:%M:%S')}")
                                print(f"状态: {str_data}")
                                print("=" * 50 + "\n")
                                continue

                            # 解析JSON数据
                            hand_data = json.loads(str_data)
                            self._display_data(hand_data)
                        except json.JSONDecodeError:
                            print(f"收到无效的JSON数据: {str_data}")
                        except Exception as e:
                            print(f"解析数据出错: {str(e)}")

                # 如果断开连接，短暂延迟后重试
                if self.running:
                    time.sleep(1)

            except ConnectionRefusedError:
                print("连接被拒绝，目标服务器可能未启动，将重试...")
                time.sleep(2)
            except Exception as e:
                print(f"连接出错: {str(e)}")
                if self.running:
                    time.sleep(2)

    def _display_data(self, data):
        """格式化并显示接收到的数据"""
        print("\n" + "=" * 50)
        print(f"接收时间: {time.strftime('%Y-%m-%d %H:%M:%S')}")
        print(f"手型: {data.get('handedness', '未知')}")
        print(f"姿态: {data.get('pose', '未知')}")

        points = data.get("points", [])
        angles = data.get("angles", [])

        print(f"关键点数量: {len(points)}")
        print(f"角度数据数量: {len(angles)}")

        # 显示部分关键点数据（如果数据太多）
        if len(points) > 0:
            print("前3个关键点数据:", points[:3])

        # 显示部分角度数据
        if len(angles) > 0:
            print("前3个角度数据:", angles[:3])
        print("=" * 50 + "\n")


if __name__ == "__main__":
    try:
        mock_superior = MockSuperior()
        mock_superior.start()
        # 保持程序运行
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        print("\n用户中断，正在停止...")
        mock_superior.stop()
