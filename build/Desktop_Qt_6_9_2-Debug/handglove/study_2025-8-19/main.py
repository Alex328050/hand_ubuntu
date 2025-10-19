# author:Dong yang Rong
# date: 2025-8-20
# brief: This is a hand decode file,very long,very shit,and very precise!
# attention! the data must have: pos quat global 开启巨型帧！
# serial_com output 构建串口输出字符串 (格式: OK:拇指23,食指23,中指23,无名指23,小指23,拇指偏移量,食指偏移量,中指偏移量,无名指偏移量,小指偏移量,拇指1,食指1,中指1,无名指1,小指1,手背pitch,手背roll,手背yaw“\n”)示例：OK:-15.393,6.076,7.118,43.266,46.101,46.230,7.271,9.553,46.973,48.733,-15.663,4.460,5.529,28.751,30.656,0.000,180.000,0.000
# 所有的print语句皆为调试用，并不影响与下位机进行串口通信，可以不关闭。（若需要减少延迟可以考虑全部关闭）
#

import socket           #网络udp所需要的库
from operator import truediv
import sys
import serial          #串口所需要的库
import time
import re
import numpy as np    #py的数学库，可以处理四元数
import math
import ctypes


from scipy.spatial.transform import Rotation as R
from typing import Dict, List, Any
from datetime import datetime

import threading
import queue

# 仅在Windows系统生效
if sys.platform.startswith('win'):
    # 隐藏控制台窗口
    ctypes.windll.user32.ShowWindow(ctypes.windll.kernel32.GetConsoleWindow(), 0)

# 配置参数
UDP_IP_RX = "0.0.0.0"   #从手套软件里接收数据
UDP_PORT_RX = 1346
UDP_IP_TX = "127.0.0.1"   #把处理后的数据发送给QT
UDP_PORT_TX_LOCAL = 1347
UDP_PORT_TX_REMOTE = 1348

UDP_IP_TX2= "127.0.0.1"   #接收qt的指令global sock_send
UDP_PORT_TX_LOCAL2 = 1349
UDP_PORT_TX_REMOTE2 =1350
USE_SERIAL = 0              # 1：打开串口功能，   0：关闭串口功能
SERIAL_PORT = "COM3"  # 根据系统修改
BAUDRATE = 9600



# 创建队列用于存储输入
input_queue = queue.Queue()

def input_listener():
    """后台线程，持续读取输入并放入队列"""
    while True:
        try:
            # 阻塞式读取输入
            # num = (input("请输入指令: "))
            data, addr = sock_send2.recvfrom(256)  #在这里选择用键盘还是udp
            num = data.decode('utf-8').strip()
            # print(num)
            input_queue.put(num)
            if num == 's':
                sys.exit(0)
        except ConnectionResetError as e:
            # print('fuck')
            pass
        except ValueError:
            # print("输入无效，请输入数字！")
            pass
        except Exception as e:
            # print(f"输入错误: {e}")
            pass



# 关节数据类，存储位置和四元数信息
class JointData:
    def __init__(self, position, quaternion):
        # 位置信息 (x, y, z)
        self.position = np.array(position, dtype=np.float64)
        # 四元数 (w, x, y, z)
        self.quaternion = np.array(quaternion, dtype=np.float64)
        # 归一化四元数
        self.quaternion = self.quaternion / np.linalg.norm(self.quaternion)


# 四元数乘法
def quaternion_multiply(q1, q2):        #两个四元数相乘
    w1, x1, y1, z1 = q1
    w2, x2, y2, z2 = q2

    w = w1 * w2 - x1 * x2 - y1 * y2 - z1 * z2
    x = x1 * w2 + w1 * x2 + y1 * z2 - z1 * y2
    y = y1 * w2 + w1 * y2 + z1 * x2 - x1 * z2
    z = z1 * w2 + w1 * z2 + x1 * y2 - y1 * x2

    return np.array([w, x, y, z])


# 计算子关节相对于父关节的旋转四元数
def get_relative_rotation(child_quat, parent_quat):
    parent_inv = np.array([parent_quat[0], -parent_quat[1], -parent_quat[2], -parent_quat[3]])
    child_quat = child_quat / np.linalg.norm(child_quat)
    parent_inv = parent_inv / np.linalg.norm(parent_inv)
    return quaternion_multiply(parent_inv, child_quat)


# 根据手指名称确定弯曲参考
class FingerAxisManager:            #这个弯曲轴其实是弯曲面的法向量
    def __init__(self):
        # 初始化 axes 作为类属性
        self.axes = {
            "Thumb": [0, 0, 1],
            "Index": [0, 0, 1],
            "Middle": [0, 0, 1],
            "Ring": [0, 0, 1],
            "Pinky": [0, 0, 1],
            "ThumbZ": [0, 1, 0],
            "IndexZ": [0, 1, 0],
            "MiddleZ": [0, 1, 0],
            "RingZ": [0, 1, 0],
            "PinkyZ": [0, 1, 0],
            "ThumbZero": [1, 0, 0],
            "IndexZero": [1, 0, 0],
            "MiddleZero": [1, 0, 0],
            "RingZero": [1, 0, 0],
            "PinkyZero": [1, 0, 0],
            "Thumb_first_bone": [0, 0, 1],
            "Thumb_second_bone": [0, 0, 1]
        }

    def get_finger_bend_axis(self, finger_name):
        return self.axes.get(finger_name, [1, 0, 0])


FingerAxis = FingerAxisManager()
# 从相对旋转四元数中提取绕弯曲轴的旋转角度（度）
# def quaternion_to_bend_angle(q_rel, bend_axis):
#     rot = R.from_quat([q_rel[1], q_rel[2], q_rel[3], q_rel[0]])
#     rot_mat = rot.as_matrix()
#
#     if bend_axis == 0:  # x轴
#         angle = np.arctan2(rot_mat[2, 1], rot_mat[1, 1])   #默认情况下，函数内部定义的变量会遮蔽（shadow） 同名的全局变量，即函数内优先使用局部变量
#     elif bend_axis == 1:  # y轴
#         angle = np.arctan2(rot_mat[0, 2], rot_mat[0, 0])
#     elif bend_axis == 2:  # z轴
#         angle = np.arctan2(rot_mat[1, 0], rot_mat[0, 0])
#
#     angle_deg = (np.degrees(angle))          #只要没有用global，be free to use angle hh
#     # angle_deg = 100.0 if angle_deg > 100.0 else angle_deg
#     # angle_deg = -100.0 if angle_deg < -100.0 else angle_deg
#       #print(angle_deg)
#     return angle_deg





def quaternion_to_bend_angle(q_rel, bend_axis=[0, 1, 0]):
    """
    从相对旋转四元数中提取绕任意过原点直线的纯旋转角度

    参数:
        q_rel: 四元数，格式为 [w, x, y, z]
        bend_axis: 弯曲轴方向向量，格式为 [x, y, z]（任意过原点的直线）

    返回:
        绕弯曲轴的旋转角度（度）
    """
    # 1. 转换四元数为旋转对象（注意四元数格式转换）
    # 确保输入是单位四元数
    q_norm = np.linalg.norm(q_rel)
    if q_norm < 1e-12:
        return 0.0
    q_normalized = q_rel / q_norm

    # 转为scipy需要的[x,y,z,w]格式
    rot = R.from_quat([q_normalized[1], q_normalized[2], q_normalized[3], q_normalized[0]])

    # 2. 标准化弯曲轴向量
    bend_axis_norm = np.linalg.norm(bend_axis)
    if bend_axis_norm < 1e-12:
        raise ValueError("弯曲轴向量不能为零向量")
    bend_axis_vec = bend_axis / bend_axis_norm  # 标准化为单位向量

    # 3. 创建一个与弯曲轴垂直的向量
    # 找到一个与弯曲轴不共线的向量
    if not np.allclose(np.abs(bend_axis_vec), [1, 0, 0]):
        v = np.array([1, 0, 0])  # 使用x轴作为参考
    else:
        v = np.array([0, 1, 0])  # 如果弯曲轴是x轴，则使用y轴作为参考

    # 计算与弯曲轴垂直的向量
    v_perp = np.cross(bend_axis_vec, v)
    v_perp = v_perp / np.linalg.norm(v_perp)  # 标准化

    # 4. 对垂直向量进行旋转
    v_rotated = rot.apply(v_perp)

    # 5. 计算旋转后的向量在垂直于弯曲轴的平面上的投影
    # 这一步确保我们只考虑绕弯曲轴的旋转
    v_proj = v_rotated - np.dot(v_rotated, bend_axis_vec) * bend_axis_vec
    v_proj = v_proj / np.linalg.norm(v_proj)  # 标准化

    # 6. 计算旋转前后垂直向量的夹角，这就是绕弯曲轴的旋转角度
    dot_product = np.dot(v_perp, v_proj)
    # 确保dot_product在[-1, 1]范围内，避免数值误差导致的问题
    dot_product = np.clip(dot_product, -1.0, 1.0)
    angle_rad = np.arccos(dot_product)

    # 7. 确定旋转方向（使用右手定则）
    cross_product = np.cross(v_perp, v_proj)
    direction = np.sign(np.dot(cross_product, bend_axis_vec))
    angle_rad = direction * angle_rad

    # 8. 转换为度并返回
    return np.degrees(angle_rad)
# 计算单个手指相对于手背传感器的弯曲角度
def calculate_finger_bend(hand_data, finger_name, weights=[0.4, 0.3, 0.3]):#默认的，但其实每根手指已经配置好了
    # hand_base = hand_data["RightHand"]
    j1 = hand_data[f"RightHand{finger_name}1"]
    j2 = hand_data[f"RightHand{finger_name}2"]
    j3 = hand_data[f"RightHand{finger_name}3"]

    q_j1_rel = j1.quaternion
    q_j2_rel = j2.quaternion
    q_j3_rel = j3.quaternion
    # # if RightHandThumb1
    # j2.quaternion = quaternion_multiply(j1.quaternion, j2.quaternion)
    j3.quaternion = quaternion_multiply(j2.quaternion, j3.quaternion)
    if finger_name== "Thumb":  #对大拇指和非大拇指进行区分
        angle_all = quaternion_multiply(j2.quaternion, j3.quaternion)
        # angle_all = j3.quaternion
        initial_vector_finger=[1,0,0]# 手背为-1，0，0，手指为1，0，0
    else:
        angle_all = quaternion_multiply(j1.quaternion, j2.quaternion)
        # angle_all = quaternion_multiply(angle_all, j3.quaternion)
        # angle_all =j1.quaternion

        initial_vector_finger = [1, 0, 0]# 手背为-1，0，0，手指为1，0，0
    # 计算每个关节相对于手背坐标系的旋转
    # q_j1_rel = get_relative_rotation(j1.quaternion, hand_base.quaternion)
    # q_j2_rel = get_relative_rotation(j2.quaternion, hand_base.quaternion)
    # q_j3_rel = get_relative_rotation(j3.quaternion, hand_base.quaternion)

    # 获取该手指的弯曲参考轴

    # bend_axis = FingerAxis.get_finger_bend_axis(finger_name)      #这里调用是为了与大拇指经行区分
    # bend_axisZ = FingerAxis.get_finger_bend_axis(f"{finger_name}Z")
    # 计算关节之间的相对旋转角度 ，其实没有用啦
    # q_j2_j1 = get_relative_rotation(j2.quaternion, j1.quaternion)
    # q_j3_j2 = get_relative_rotation(j3.quaternion, j2.quaternion)

    # 计算每个关节的弯曲角度
    # angle1 = angle2 = angle3 = angle4 =0
    test_bend_all_vector = quaternion_to_vector(angle_all,initial_vector_finger)  # 手背为-1，0，0，手指为1，0，0
    test_bend_1_vector= quaternion_to_vector(q_j1_rel,initial_vector_finger)
    test_bend_2_vector = quaternion_to_vector(q_j2_rel, initial_vector_finger)
    test_bend_3_vector = quaternion_to_vector(q_j3_rel, initial_vector_finger)

    angle1= vector_plane_angle(test_bend_1_vector, FingerAxis.axes[finger_name])
    # angle2 =vector_plane_angle(test_bend_2_vector, FingerAxis.axes[finger_name])
    # angle3 =vector_plane_angle(test_bend_3_vector, FingerAxis.axes[finger_name])
    angle2 =vector_angle(test_bend_2_vector, initial_vector_finger)
    if test_bend_2_vector[2]<=0:angle2*=-1

    angle3 = vector_angle(test_bend_3_vector, initial_vector_finger)
    if test_bend_3_vector[2] <= 0: angle3 *= -1
    weights2 = [1, 1.2, 0.3]
    weights3 = [0.8,0.5, 0.3]

    if finger_name== "Thumb":  #对大拇指和非大拇指进行区分
        # angle4 = vector_plane_angle(test_bend_all_vector, FingerAxis.axes[f"{finger_name}Z"])

        angle2=vector_angle(test_bend_2_vector, FingerAxis.axes[f"{finger_name}_second_bone"])
        angle4 = vector_angle(test_bend_1_vector, FingerAxis.axes[f"{finger_name}_first_bone"])*weights2[0]+angle2*weights2[1]
        angle1 =vector_angle(test_bend_1_vector, FingerAxis.axes[f"{finger_name}_first_bone"])*weights3[0]+angle2*weights3[1]#大拇指第一舵机
        # print(angle1)
    else :
        angle4= vector_plane_angle(test_bend_1_vector, FingerAxis.axes[f"{finger_name}Z"])
    # angle4 =vector_plane_angle(test_bend_all_vector, FingerAxis.axes[f"{finger_name}Z"])
    # if finger_name=="Index":
    #     print(angle_all)

    # angle1 = quaternion_to_bend_angle(q_j1_rel, bend_axis)  # 第一个关节相对"手背"
    # angle2 = quaternion_to_bend_angle(q_j2_rel, bend_axis)  # 第二个关节相对手背
    # angle3 = quaternion_to_bend_angle(q_j3_rel, bend_axis)  # 第三个关节相对手背

    # angle4 = quaternion_to_bend_angle(q_j1_rel, 2)  # 第一个关节相对手背z轴
    # angle5 = quaternion_to_bend_angle(q_j2_rel, 2)  # 第二个关节相对手背z轴
    # angle6 = quaternion_to_bend_angle(q_j3_rel, 2)  # 第三个关节相对手背z轴

    # angle4 = (quaternion_to_euler(q_j1_rel[0], q_j1_rel[1], q_j1_rel[2], q_j1_rel[3]))[2]
    # angle5 = (quaternion_to_euler(q_j2_rel[0], q_j2_rel[1], q_j2_rel[2], q_j2_rel[3]))[2]
    # angle6 = (quaternion_to_euler(q_j3_rel[0], q_j3_rel[1], q_j3_rel[2], q_j3_rel[3]))[2]

    # angle4 = quaternion_to_bend_angle(q_j1_rel, bend_axisZ)

    # print((quaternion_to_euler(q_j1_rel[0], q_j1_rel[1], q_j1_rel[2], q_j1_rel[3]))[1])
    # threshold = 5.0   #过滤噪声，也可以把这个设小一点
    # angle1 = angle1 if angle1 > threshold else 0  #本质上就是个三元运算符 C: angle2 = angle2 ？ angle2 > threshold ： 0
    # angle2 = angle2 if angle2 > threshold else 0
    # angle3 = angle3 if angle3 > threshold else 0
    #
    # angle4 = angle4 if angle4 > threshold else 0
    # angle5 = angle5 if angle5 > threshold else 0
    # angle6 = angle6 if angle6 > threshold else 0

    total_angle = angle1 * weights[0] + angle2 * weights[1] + angle3 * weights[2]

    # total_z_deflection_angle = angle4 * weights[0] + angle5 * weights[1] + angle6 * weights[2]
    total_z_deflection_angle = angle4
    # total_angle = angle3
    # total_z_deflection_angle = angle4-180 if angle4>0 else angle4+180
    # print(finger_name,angle1)
    max_angle = 100.0   #上下bend 与偏移量公用一个max——angle
    # total_angle = min(total_angle, max_angle)
    # total_z_deflection_angle = min(total_z_deflection_angle, max_angle)
    return {   #return 了一个 dict(词典)
        f"{finger_name}_joint_angles": [angle1, angle2, angle3],
        # f"{finger_name}_Z_deflection_angles": [angle4, angle5, angle6],
        f"{finger_name}_weighted_angle": total_angle,
        f"{finger_name}_Z_deflection_angle": total_z_deflection_angle,
        f"{finger_name}_bend_percentage": ( total_angle / max_angle * 100),
        f"{finger_name}_Z_deflection_percentage": ( total_z_deflection_angle / max_angle * 100),
        # f"{finger_name}_bend_axis": bend_axis
    }


def quaternion_to_euler(w, x, y, z):  #这个函数可以自动归一化
    # w, x, y, z - - 四元数分量
    # 返回:
    # (pitch, roll, yaw)
    # 单位：度数
    # 归一化四元数
    norm = np.sqrt(w**2 + x**2 + y**2 + z**2)
    if norm == 0:
        raise ValueError("四元数不能全为零")
    w /= norm
    x /= norm
    y /= norm
    z /= norm
    # print(f"w: {w:.2f}  x: {x:.2f}  y: {y:.2f}  z: {z:.2f}")
    quat = R.from_quat(quat=[x, y, z, w])  # 这里传入的传入顺序 [x, y, z, w]

    # euler = quat.as_euler('xyz', degrees=True)  # 'xyz' 对应偏航-俯仰-横滚

    # euler = quat.as_euler('yxz', degrees=True)  #返回[z轴角, x轴角, y轴角]  X代表外在轴  实际是外在旋转zxy

    # pitch,roll,yaw = euler
    euler = quat.as_euler('xzy', degrees=True)  # 返回[z轴角, x轴角, y轴角]
    # 解包顺序对应：z轴旋转（yaw）、y轴旋转（pitch）、x轴旋转（roll）


    pitch,yaw,roll = euler

    return pitch,roll,yaw    #实际手的pitch and roll是反的


def quaternion_to_vector(q, initial_vector=[0, 1, 0]):
    """将四元数转换为空间中的三维向量"""
    # 归一化四元数
    q_norm = np.linalg.norm(q)
    if q_norm == 0:
        raise ValueError("四元数不能全为零")
    q = q / q_norm
    w, x, y, z = q

    # 计算旋转矩阵
    rotation_matrix = np.array([
        [1 - 2 * y ** 2 - 2 * z ** 2, 2 * x * y - 2 * z * w, 2 * x * z + 2 * y * w],
        [2 * x * y + 2 * z * w, 1 - 2 * x ** 2 - 2 * z ** 2, 2 * y * z - 2 * x * w],
        [2 * x * z - 2 * y * w, 2 * y * z + 2 * x * w, 1 - 2 * x ** 2 - 2 * y ** 2]
    ])

    # 计算旋转后的向量
    vector = rotation_matrix @ np.array(initial_vector)
    return vector


def vector_angle(v1, v2, unit='degree'):
    """
    计算两个三维向量之间的夹角

    参数:
        v1: 第一个三维向量，格式为列表或数组 [x1, y1, z1]
        v2: 第二个三维向量，格式为列表或数组 [x2, y2, z2]
        unit: 输出单位，'degree' 为度（默认），'radian' 为弧度

    返回:
        两个向量的夹角（根据unit参数返回度或弧度）

    异常:
        ValueError: 若输入不是三维向量或输入为零向量
    """
    # 转换为numpy数组并确保是三维向量
    v1 = np.asarray(v1)
    v2 = np.asarray(v2)

    if v1.shape != (3,) or v2.shape != (3,):
        raise ValueError("输入必须是三维向量")

    # 计算向量的模长
    norm_v1 = np.linalg.norm(v1)
    norm_v2 = np.linalg.norm(v2)

    # 检查零向量
    if norm_v1 < 1e-12 or norm_v2 < 1e-12:
        raise ValueError("输入向量不能为零向量")

    # 计算点积
    dot_product = np.dot(v1, v2)

    # 计算夹角的余弦值（并限制在[-1, 1]范围内避免数值误差）
    cos_theta = dot_product / (norm_v1 * norm_v2)
    cos_theta = np.clip(cos_theta, -1.0, 1.0)

    # 计算夹角（弧度）
    angle_rad = np.arccos(cos_theta)

    # 根据单位参数返回结果
    if unit == 'degree':
        return np.degrees(angle_rad)
    else:
        return angle_rad


def vector_plane_angle(v, plane_normal, unit='degree'):
    """
    计算三维向量与过原点平面的夹角

    参数:
        v: 三维向量，格式为列表或数组 [x, y, z]
        plane_normal: 平面的法向量（过原点的平面），格式为列表或数组 [a, b, c]
                      平面方程为 ax + by + cz = 0
        unit: 输出单位，'degree' 为度（默认），'radian' 为弧度

    返回:
        向量与平面的夹角（根据unit参数返回度或弧度，范围0到90度或0到π/2弧度）

    异常:
        ValueError: 若输入不是三维向量、法向量为零向量
    """
    # 转换为numpy数组并验证维度
    v = np.asarray(v)
    plane_normal = np.asarray(plane_normal)

    if v.shape != (3,) or plane_normal.shape != (3,):
        raise ValueError("输入向量和法向量必须是三维向量")

    # 验证法向量不为零向量
    norm_normal = np.linalg.norm(plane_normal)
    if norm_normal < 1e-12:
        raise ValueError("平面法向量不能为零向量")

    # 标准化向量和法向量
    v_norm = np.linalg.norm(v)
    if v_norm < 1e-12:
        raise ValueError("输入向量不能为零向量")

    v_unit = v / v_norm
    n_unit = plane_normal / norm_normal

    # 向量与平面的夹角θ = 90° - 向量与法向量的夹角φ
    # 推导：sinθ = cosφ = (v·n)/(|v||n|)，因此θ = arcsin(v·n)
    dot_product = np.dot(v_unit, n_unit)
    dot_product = np.clip(dot_product, -1.0, 1.0)  # 处理数值误差

    # 计算夹角（弧度）
    # angle_rad = np.arcsin(np.abs(dot_product))  # 取绝对值确保角度在0~90度
    angle_rad = np.arcsin(dot_product)  # 取绝对值确保角度在0~90度

    # 根据单位参数返回结果
    if unit == 'degree':
        return np.degrees(angle_rad)
    else:
        return angle_rad


def get_orthogonal_vectors(q1, q2, initial_vector=[0, 1, 0]):
    """
    输入两个四元数，返回对应的向量及相关正交向量

    参数:
        q1, q2: 两个四元数，格式为 (w, x, y, z)
        initial_vector: 初始参考向量，默认为 [0, 0, 1]

    返回:
        元组 (v1, v2, v3, v4)，其中：
        - v1: q1对应的三维向量
        - v2: q2对应的三维向量
        - v3: 与v1和v2都正交的向量（v1 × v2）
        - v4: 与v1和v3都正交的向量（v1 × v3）
    """
    # 将四元数转换为三维向量
    v1 = quaternion_to_vector(q1, initial_vector)
    v2 = quaternion_to_vector(q2, initial_vector)

    # 计算与v1和v2都正交的向量（叉积）
    v3 = np.cross(v1, v2)

    # 处理v3为零向量的情况（当v1和v2共线时）
    if np.allclose(v3, [0, 0, 0]):
        # 选择一个不与v1共线的向量来计算正交向量
        if not np.allclose(v1, [1, 0, 0]):
            temp = [1, 0, 0]
        else:
            temp = [0, 1, 0]
        v3 = np.cross(v1, temp)

    # 计算与v1和v3都正交的向量
    v4 = np.cross(v1, v3)

    # 归一化所有向量（可选步骤，根据需要）
    v1 = v1 / np.linalg.norm(v1)
    v2 = v2 / np.linalg.norm(v2)
    v3 = v3 / np.linalg.norm(v3)
    v4 = v4 / np.linalg.norm(v4)

    return v1, v2, v3, v4


def quaternion_to_spherical(q, initial_vector=[-1, 0, 0]):
    """
    将四元数转换为对应的三维向量和球坐标

    参数:
        q: 四元数，格式为 (w, x, y, z)
        initial_vector: 初始参考向量，默认为沿z轴方向 [0, 0, 1]

    返回:
        元组 (vector, r, theta, phi, theta_deg, phi_deg)
        其中：
            vector: 旋转后的三维向量 [x, y, z]
            r: 球坐标半径（向量模长）
            theta: 极角（与z轴夹角，弧度）
            phi: 方位角（xy平面与x轴夹角，弧度）
            theta_deg: 极角（角度）
            phi_deg: 方位角（角度）
    """

    vector=quaternion_to_vector(q, initial_vector)
    x_vec, y_vec, z_vec = vector
    # 4. 三维向量转球坐标
    r = np.linalg.norm(vector)

    if r == 0:
        return vector, 0, 0, 0

    # 极角θ（与z轴夹角）
    theta = math.acos(np.clip(z_vec / r, -1.0, 1.0))  # 限制范围避免数值误差
    # 方位角φ（xy平面与x轴夹角）
    phi = math.atan2(y_vec, x_vec)

    # 转换为角度制
    theta_deg = math.degrees(theta)
    phi_deg = math.degrees(phi)

    return theta_deg,phi_deg
# 处理整只手的数据
def process_hand_data(hand_data):
 fingers = [    #各个手指的名字一个各个关节预期的权重
     {"name": "Thumb", "weights": [0, 0,1]},#可以看到这里把每根指头的第一个关节的权重设为零
     {"name": "Index", "weights": [0, 0.5,0.5]},
     {"name": "Middle", "weights": [0, 0.5,0.5]},
     {"name": "Ring", "weights": [0, 0.5,0.5]},
     {"name": "Pinky", "weights": [0, 0.5,0.5]}
 ]

 result = {}
 for finger in fingers:
     finger_result = calculate_finger_bend(
         hand_data,
         finger["name"],
         finger["weights"]
     )

     #print(finger_result)  #调试用，可以注释掉
     result.update(finger_result)  #把所有的新dict加入进来，重复的进行更新
 # print("\nhand_data: ", hand_data["RightHand"].quaternion)
 result.update({"euler_Right_Hand": quaternion_to_euler(hand_data["RightHand"].quaternion[0],
                                                               hand_data["RightHand"].quaternion[1],
                                                               hand_data["RightHand"].quaternion[2],
                                                               hand_data["RightHand"].quaternion[3])})  # 计算手背的方位角，有点shi
 # print(result)  # 调试用，可以注释掉

 result.update({"euler_RightHandIndex1": quaternion_to_euler(hand_data["RightHandIndex1"].quaternion[0],
                                                             hand_data["RightHandIndex1"].quaternion[1],
                                                             hand_data["RightHandIndex1"].quaternion[2],
                                                             hand_data["RightHandIndex1"].quaternion[
                                                                 3])})  # 计算食指的方位角，有点shi
 result.update({"euler_RightHandIndex3": quaternion_to_euler(hand_data["RightHandIndex3"].quaternion[0],
                                                        hand_data["RightHandIndex3"].quaternion[1],
                                                        hand_data["RightHandIndex3"].quaternion[2],
                                                        hand_data["RightHandIndex3"].quaternion[3])})  # 计算食指的方位角，有点shi

 # raletive_abn = quaternion_multiply(hand_data["RightHandIndex2"].quaternion, hand_data["RightHandIndex3"].quaternion)
 # result.update({"euler_RightHandIndex3": quaternion_to_euler(raletive_abn[0],raletive_abn[1],raletive_abn[2],raletive_abn[2])})  # 计算食指的方位角，有点shi
 RightHandIndex3_relative=get_relative_rotation(hand_data["RightHandIndex3"].quaternion, hand_data["RightHandIndex1"].quaternion)
 result.update({"euler_RightHandIndex3_relative": quaternion_to_euler(RightHandIndex3_relative[0],
                                                             RightHandIndex3_relative[1],
                                                             RightHandIndex3_relative[2],
                                                             RightHandIndex3_relative[3])})  # 计算食指的相对于手背的方位角，有点shi
 result.update({"euler_RightHandIndex3_relative_quat": get_relative_rotation(hand_data["RightHandIndex3"].quaternion,hand_data["RightHand"].quaternion)})  # 计算食指的相对于手背的方位角，有点shi
 return result


# 手套数据解析器
class GloveDataParser:
 BONE_ORDER = [
     "RightHand", "RightHandThumb1", "RightHandThumb2", "RightHandThumb3",
     "RightHandIndex1", "RightHandIndex2", "RightHandIndex3",
     "RightHandMiddle1", "RightHandMiddle2", "RightHandMiddle3",
     "RightHandRing1", "RightHandRing2", "RightHandRing3",
     "RightHandPinky1", "RightHandPinky2", "RightHandPinky3",
     "LeftHand", "LeftHandThumb1", "LeftHandThumb2", "LeftHandThumb3",
     "LeftHandIndex1", "LeftHandIndex2", "LeftHandIndex3",
     "LeftHandMiddle1", "LeftHandMiddle2", "LeftHandMiddle3",
     "LeftHandRing1", "LeftHandRing2", "LeftHandRing3",
     "LeftHandPinky1", "LeftHandPinky2", "LeftHandPinky3"
 ]

 def __init__(self):
     self.reset()

 def reset(self):
     self.device_name = None
     self.timestamp = None
     self.has_pos = False
     self.rotation_type = None
     self.euler_order = None
     self.rotation_frame = None
     self.other_info = {}
     return self

 def parse(self, data_frame: str) -> Dict[str, Any]:
     self.reset()
     parts = re.split(r'\s+subpackage\s+', data_frame, maxsplit=1)
     if len(parts) != 2:
         raise ValueError("数据帧格式错误，未找到subpackage分隔符")

     control_part, data_part = parts
     self._parse_control_part(control_part)
     bone_data = self._parse_data_part(data_part)

     return {
         "device": self.device_name,
         "timestamp": self.timestamp,
         "rotation_frame": self.rotation_frame,
         "bones": bone_data,
         "other_info": self.other_info
     }

 def _parse_control_part(self, control_str: str):
     tokens = control_str.strip().split()
     if not tokens:
         raise ValueError("控制字段为空")

     self.device_name = tokens[0]
     current_idx = 1

     if current_idx < len(tokens) and tokens[current_idx] == "time":
         if current_idx + 2 >= len(tokens):
             raise ValueError("时间字段不完整")
         self.timestamp = f"{tokens[current_idx + 1]} {tokens[current_idx + 2]}"
         current_idx += 3

     while current_idx < len(tokens):
         token = tokens[current_idx]
         if token == "pos":
             self.has_pos = True
             current_idx += 1
         elif token in ("quat", "euler"):
             if self.rotation_type is not None:
                 raise ValueError(f"同时存在{self.rotation_type}和{token}，互斥字段")
             self.rotation_type = token
             current_idx += 1
             if token == "euler" and current_idx < len(tokens):
                 self.euler_order = tokens[current_idx].upper()
                 current_idx += 1
         elif token in ("global", "relative"):
             self.rotation_frame = token
             current_idx += 1
         else:
             if current_idx + 1 < len(tokens):
                 self.other_info[token] = tokens[current_idx + 1]
                 current_idx += 2
             else:
                 self.other_info[token] = None
                 current_idx += 1

     if self.rotation_type is None:
         raise ValueError("控制字段中未找到quat或euler")

 def _parse_data_part(self, data_str: str) -> Dict[str, Dict[str, Any]]:
     data_items = data_str.strip().split(',')#这里将字符串处理成数据字符串：  {“1,2,3,4”}-->{"1","2","3","4"}
     if not data_items:
         raise ValueError("数据字段为空")

     values = data_items[1:]  #把第一个元素跳过了，一般为“1/1”
     try:
         values = [float(v) for v in values]   #字符串转化为浮点数
     except ValueError as e:
         raise ValueError(f"数据转换为数值失败: {str(e)}")

     if self.rotation_type == "quat":
         group_size = 7 if self.has_pos else 4
     elif self.rotation_type == "euler":
         group_size = 6 if self.has_pos else 3
     else:
         raise ValueError(f"不支持的旋转类型: {self.rotation_type}")

     expected_length = 32 * group_size
     if len(values) != expected_length:
         raise ValueError(f"数据长度不匹配，预期{expected_length}个值，实际{len(values)}个值")

     bone_data = {}
     for i in range(32):
         bone_name = self.BONE_ORDER[i]
         start_idx = i * group_size
         end_idx = start_idx + group_size
         group_values = values[start_idx:end_idx]

         bone_info = {}
         pos_start = 0
         if self.has_pos:
             bone_info["position"] = {
                 "x": group_values[0],
                 "y": group_values[1],
                 "z": group_values[2]
             }
             pos_start = 3

         if self.rotation_type == "quat":
             bone_info["rotation"] = {
                 "type": "quat",
                 "w": group_values[pos_start],
                 "x": group_values[pos_start + 1],
                 "y": group_values[pos_start + 2],
                 "z": group_values[pos_start + 3]
             }
         else:
             bone_info["rotation"] = {
                 "type": "euler",
                 "x": group_values[pos_start],
                 "y": group_values[pos_start + 1],
                 "z": group_values[pos_start + 2],
                 "order": self.euler_order
             }

         bone_data[bone_name] = bone_info
     return bone_data


# 数据格式转换：将解析后的骨骼数据转换为process_hand_data所需的JointData字典
def convert_to_joint_data(parsed_bones):
 joint_data_dict = {}
 # 只处理右手相关骨骼
 right_hand_bones = [bone for bone in parsed_bones if bone.startswith("RightHand")]
 # print("joint_data_dict[bone_name]:")
 for bone_name in right_hand_bones:
     bone_info = parsed_bones[bone_name]
     # 提取位置信息 (x, y, z)
     position = [
         bone_info["position"]["x"],
         bone_info["position"]["y"],
         bone_info["position"]["z"]
     ] if "position" in bone_info else [0, 0, 0]

     # 提取四元数并转换为 (w, x, y, z) 格式
     rotation = bone_info["rotation"]

     if rotation["type"] == "quat":
         quaternion = [
             rotation["w"],  # w
             rotation["x"],  # x
             rotation["y"],  # y
             rotation["z"]  # z
         ]
     else:
         # 如果是欧拉角，转换为四元数 (w, x, y, z)
         r = R.from_euler(rotation["order"],
                          [rotation["x"], rotation["y"], rotation["z"]],
                          degrees=True)
         quat = r.as_quat()  # scipy返回 (x, y, z, w)
         quaternion = [quat[3], quat[0], quat[1], quat[2]]  # 转换为 (w, x, y, z)

     joint_data_dict[bone_name] = JointData(position, quaternion)

 return joint_data_dict

test_bend_all_bend_axis=[0,1,0]
def main():
 global vector1
 # print("四元数转方位角：", quaternion_to_euler(0.62,0.77,0.05,0.05))#  0.73  x: 0.18  y: 0.37  z: 0.55  这个在测试函数，可以删掉
 # print(get_relative_rotation([0.717/2,0,0,0.717/2],[0.717/2,0.717/2,0,0]))
 # 初始化UDP socket
 global sock_receive
 global sock_send
 global sock_send2
 sock_receive= socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
 sock_receive.bind((UDP_IP_RX, UDP_PORT_RX))
 sock_receive.settimeout(0.01)  # 设置超时时间0.001秒（1ms）
 sock_receive.setblocking(False)  # 设置为非阻塞模式

 sock_send = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
 sock_send.bind((UDP_IP_TX, UDP_PORT_TX_LOCAL))
 sock_send2 = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
 sock_send2.bind((UDP_IP_TX2, UDP_PORT_TX_LOCAL2))
 # print(f"UDP服务器启动，监听 {UDP_IP_RX}:{UDP_PORT_RX},发送 {UDP_IP_TX}:{UDP_PORT_TX_LOCAL}")

 # 初始化串口
 if USE_SERIAL:
     ser = serial.Serial(
         port=SERIAL_PORT,
         baudrate=BAUDRATE,
         bytesize=8,
         parity='N',
         stopbits=1,
         timeout=1
     )
     # print(f"串口已打开: {ser.name}")

 # 初始化解析器
 parser = GloveDataParser()
 # 启动输入监听线程
 thread = threading.Thread(target=input_listener, daemon=True)
 thread.start()
 try:
     while True:
         # 接收UDP数据
         # try:
         #     data, addr = sock_receive.recvfrom(4096)  # 缓冲区大小4096字节  这是一个阻塞函数，如果没有接收到数据就一直等待
         #     data_str = data.decode('utf-8').strip()
         #     print(f"收到UDP数据: {data_str[:50]}...")  # 打印前50字符

         real_udp_flag=1
         while real_udp_flag:
             try:
                 # 循环读取，直到缓冲区为空
                 while True:
                     data, addr = sock_receive.recvfrom(4096)
                     latest_data = data  # 不断更新为最新数据
                     latest_addr = addr
                     data_str = data.decode('utf-8').strip()
                     # print(f"收到UDP数据: {data_str[:50]}...")  # 打印前50字符
             except BlockingIOError:
                 # 缓冲区已空，没有更多数据
                 # print("缓冲区已空，没有更多数据")
                 time.sleep(0.001)
                 real_udp_flag = 0
                 pass
             except socket.timeout:   #此时是真的没数据了，因为我设置的时间只有1ms
                 # 超时无数据，可忽略或打印提示
                 # print("超时未收到数据")
                 real_udp_flag=0
                 pass

         try:
             start = time.time()
             # 解析数据帧
             parsed_data = parser.parse(data_str)
             # print('\nparsed_data',parsed_data,"\n")
             # 转换为JointData格式
             joint_data = convert_to_joint_data(parsed_data["bones"])
             # print('\njoint_data',joint_data,"\n")

             now = datetime.now()
             # print("当前时间：", now,"time_data_time:", parsed_data["timestamp"])
             # 处理手部数据获取弯曲角度
             bend_results = process_hand_data(joint_data)
             #bend_results = process_hand_data(sample_data)
             # print("\nbend_results:\n",bend_results)
             # 提取各手指平均弯曲度
             fingers = ["Thumb", "Index", "Middle", "Ring", "Pinky"]
             # bend_percentages = [parsed_data["timestamp"]]

             bend_percentages = [#bend_percentages包含了水平偏摆和竖直偏摆
                 f"{bend_results[f'{finger}_bend_percentage']:.3f}"
                 for finger in fingers
             ]
             bend_percentages += [  # 加上数值偏摆量
                 f"{bend_results[f'{finger}_Z_deflection_percentage']:.3f}"
                 for finger in fingers
             ]
             bend_percentages += [  # 加上每根手指的第一个关节
                 f"{bend_results[f'{finger}_joint_angles'][0]:.3f}"
                 for finger in fingers
             ]
             #
             # bend_percentages += [  # 加上手背的三个方位角 顺序 pitch roll yaw
             #     f"{bend_results["euler_Right_Hand"][i]:.3f}"
             #     for i in range(3)
             # ]
             # bend_percentages = [  # 加上食指指尖的三个方位角 顺序 pitch roll yaw
             #     f"{bend_results["euler_RightHandIndex3"][i]:.3f}"
             #     for i in range(3)
             # ]
             # bend_percentages += [  # 加上食指指尖相对于手背的四个四元数
             #     f"{bend_results["euler_RightHandIndex3_relative_quat"][i]:.3f}"
             #     for i in range(4)
             # ]
             # bend_percentages += [  # 加上食指指尖相对于手背的三个欧拉角 顺序 pitch roll yaw
             #     f"{bend_results["euler_RightHandIndex3_relative"][i]:.3f}"
             #     for i in range(3)
             # ]
             # bend_percentages += [  # 加上食指指尖相对于食指根部的三个欧拉角 顺序 pitch roll yaw
             #     f"{bend_results["euler_RightHandIndex3_relative"][i]:.3f}"
             #     for i in range(3)
             # ]
             # 构建串口输出字符串 (格式: 拇指,食指,中指,无名指,小指,拇指偏移量,食指偏移量,中指偏移量,无名指偏移量,小指偏移量)
             # output_str = "OK:" + "时间"+str(bend_percentages[0])+"手背："+",".join(bend_percentages[1:4]) + "食指指尖绝对："+",".join(bend_percentages[4:7]) +"食指指尖相对手背四元数："+",".join(bend_percentages[7:11]) +"食指指尖相对手背欧拉角："+",".join(bend_percentages[11:14]) +"\n"

             output_str = "OK:" + ",".join(bend_percentages)+"\n"
             #####################################################全是调试用的################################
             # output_str=str((quaternion_to_spherical(joint_data["RightHand"].quaternion[0],joint_data["RightHand"].quaternion[1],joint_data["RightHand"].quaternion[2],joint_data["RightHand"].quaternion[3]))[1:3])#quaternion_to_spherical
             # output_str += str((quaternion_to_spherical(joint_data["RightHand"].quaternion,[-1,0,0])))  # quaternion_to_spherical
             # output_str += "向量"+str(quaternion_to_vector(joint_data["RightHand"].quaternion, initial_vector=[-1, 0, 0]))
             # output_str = "OK:" + "时间" + str(bend_percentages[0]) + "手背：" + ",".join(bend_percentages)
             # output_str+="手背四元数"+str(joint_data["RightHandIndex3"].quaternion)+'\n'
             # output_str += "指根四元数" + str(joint_data["RightHandIndex1"].quaternion)
             # output_str += "四元数wxyz" + str(joint_data["RightHand"].quaternion)+'\n'
             #
             # output_str += "OK:" + "时间"+str(bend_percentages[0])+"手背："+",".join(bend_percentages[1:4]) + "食指指根相对："+",".join(bend_percentages[4:7]) +"食指指尖相对手背四元数："+",".join(bend_percentages[7:11]) +"食指指尖相对手背欧拉角："+",".join(bend_percentages[11:14]) +"\n"
             # 获取用户输入的字符串并赋值给变量
             # commend = input()  # 括号中的内容是提示信息，会显示在屏幕上
             # test_bend_all_Thumb = quaternion_multiply(joint_data["RightHandThumb1"].quaternion,
             #                                     joint_data["RightHandThumb2"].quaternion)


             # test_bend_all_vector = quaternion_to_vector(joint_data["RightHand"].quaternion, [-1,0,0])  # 手背为-1，0，0，手指为0，1，0
             # angle1 = angle2 = angle3 = vector_plane_angle(test_bend_all_vector, FingerAxis.axes[finger_name])
             # angle1 = angle2 = angle3 = vector_angle(test_bend_all_vector, [0, 1, 0])
             # angle4 = vector_plane_angle(test_bend_all_vector, FingerAxis.axes[f"{finger_name}Z"])
             # test_bend_all_Thumb = joint_data["RightHand"].quaternion
             # print(test_bend_all_vector)
             # print("angle1", test_bend_all_angle, "angle2", test_bend_all_angle2)
             # output_str = "angle1:"+str(test_bend_all_angle)+ '\n'+ "angle2:"+str(test_bend_all_angle2) + '\n'
             #

             if not input_queue.empty():
                 commend = input_queue.get()
                 # test_bend_all_Thumb = quaternion_multiply(joint_data["RightHandThumb2"].quaternion,
                 #                                     joint_data["RightHandThumb3"].quaternion)
                 # test_bend_all_Index = quaternion_multiply(joint_data["RightHandIndex1"].quaternion,
                 #                                     joint_data["RightHandIndex2"].quaternion)
                 # test_bend_all_Middle = quaternion_multiply(joint_data["RightHandMiddle1"].quaternion,
                 #                                           joint_data["RightHandMiddle2"].quaternion)
                 # test_bend_all_Ring = quaternion_multiply(joint_data["RightHandRing1"].quaternion,
                 #                                           joint_data["RightHandRing2"].quaternion)
                 # test_bend_all_Pinky = quaternion_multiply(joint_data["RightHandPinky1"].quaternion,
                 #                                           joint_data["RightHandPinky2"].quaternion)
                 test_bend_all_Thumb = quaternion_multiply(joint_data["RightHandThumb2"].quaternion,
                                                           joint_data["RightHandThumb3"].quaternion)
                 test_bend_all_Index = joint_data["RightHandIndex1"].quaternion

                 test_bend_all_Middle =joint_data["RightHandMiddle1"].quaternion

                 test_bend_all_Ring = joint_data["RightHandRing1"].quaternion

                 test_bend_all_Pinky = joint_data["RightHandPinky1"].quaternion
                 if commend=='t':  #校验大拇指
                     FingerAxis.axes["Thumb_first_bone"] = quaternion_to_vector(
                         joint_data["RightHandThumb1"].quaternion, [1, 0, 0])
                     FingerAxis.axes["Thumb_second_bone"] = quaternion_to_vector(
                         joint_data["RightHandThumb2"].quaternion, [1, 0, 0])
                     sock_send2.sendto("Thumb!".encode('utf-8'), (UDP_IP_TX2, UDP_PORT_TX_REMOTE2))
                 if commend == 'a':
                     vector1= test_bend_all_Thumb,test_bend_all_Index,test_bend_all_Middle,test_bend_all_Ring,test_bend_all_Pinky

                     # print("first quat", vector1)
                     sock_send2.sendto("yes!".encode('utf-8'), (UDP_IP_TX2, UDP_PORT_TX_REMOTE2))
                 if commend == 'd':
                     vector2 = test_bend_all_Thumb,test_bend_all_Index,test_bend_all_Middle,test_bend_all_Ring,test_bend_all_Pinky
                     v1, v2, v3, v4 = get_orthogonal_vectors(vector1[0], vector2[0], initial_vector=[1, 0, 0])  # 手背为-1，0，0，手指为0，1，0
                     FingerAxis.axes["Thumb"] = v4.tolist()
                     FingerAxis.axes["ThumbZ"] = v3.tolist()
                     v1, v2, v3, v4 = get_orthogonal_vectors(vector1[1], vector2[1],
                                                             initial_vector=[1, 0, 0])  # 手背为-1，0，0，手指为0，1，0
                     FingerAxis.axes["Index"] = v4.tolist()
                     FingerAxis.axes["IndexZ"] = v3.tolist()
                     v1, v2, v3, v4 = get_orthogonal_vectors(vector1[2], vector2[2],
                                                             initial_vector=[1, 0, 0])  # 手背为-1，0，0，手指为0，1，0
                     FingerAxis.axes["Middle"] = v4.tolist()
                     FingerAxis.axes["MiddleZ"] = v3.tolist()
                     v1, v2, v3, v4 = get_orthogonal_vectors(vector1[3], vector2[3],
                                                             initial_vector=[1, 0, 0])  # 手背为-1，0，0，手指为0，1，0
                     FingerAxis.axes["Ring"] = v4.tolist()
                     FingerAxis.axes["RingZ"] = v3.tolist()
                     v1, v2, v3, v4 = get_orthogonal_vectors(vector1[4], vector2[4],
                                                             initial_vector=[1, 0, 0])  # 手背为-1，0，0，手指为0，1，0
                     FingerAxis.axes["Pinky"] = v4.tolist()
                     FingerAxis.axes["PinkyZ"] = v3.tolist()
                     # print(v1, v2, v3, v4)
                     # print("second quat", vector1)  # 打印输入的内容
                     # print('done!')
                     sock_send2.sendto("done!".encode('utf-8'), (UDP_IP_TX2, UDP_PORT_TX_REMOTE2))
                 if commend == 's':
                     sys.exit(0)
             if USE_SERIAL:
                 ser.write(output_str.encode('utf-8'))

             sock_send.sendto(output_str.encode('utf-8'), (UDP_IP_TX, UDP_PORT_TX_REMOTE))   #这里把数据发送给QT
             # print(f"发送到串口还有QT: {output_str.strip()}")
             # print("time:",parsed_data["timestamp"])
             # print("四元数转方位角：",quaternion_to_euler(4, 1, 2, 3))

             end = time.time()
             # print("time:",(end - start)*1000)
         except Exception as e:
              # print(f"数据处理错误: {str(e)}")
              pass

         # 短暂延迟
         time.sleep(0.05)

 except KeyboardInterrupt:
     # print("\n程序终止")
     pass
 finally:
     if USE_SERIAL:
         # 关闭资源
         sock_receive.close()
         if ser.is_open:
             ser.close()

     # print("资源已释放")


if __name__ == "__main__":
 main()