import asyncio
import cv2
import time
import socket
import threading
import os
import json

import mediapipe as mp
from mediapipe import solutions
from mediapipe.framework.formats import landmark_pb2

import numpy as np

# Constants and controls - see the README.md file for details
NUM_DOFS = 18  # Number of DOF's transmitted to hand

# text drawing
MARGIN = 10  # pixels
FONT_SIZE = 1
FONT_THICKNESS = 1
HANDEDNESS_TEXT_COLOR = (88, 205, 54)  # vibrant green

# wrist recognition
wrist_enabled = True

# Port socket
HOST = "127.0.0.1"
PORT = 8666
latest_data = b"Initialing..."
lock = threading.Lock()

# Options
console_debug_enabled = False
socket_debug_enabled = True
image_debug_enabled = False
sleep_time = 0


# data = {"handedness": "", "pose": "", "points": [], "angles": []}
ANGLES = []
POINTS = []
HANDEDNESS = ""
POSE = ""


def millis():
    """Get the current time in milliseconds."""
    return int(round(time.time() * 1000))


def get_output():
    data = {
        "handedness": HANDEDNESS,
        "pose": POSE,
        "points": list(POINTS),
        "angles": list(ANGLES),
    }
    res = json.dumps(data)
    return res


def console_debug():
    print(get_output())


def tcp_server():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((HOST, PORT))
        s.listen()
        print(f"[TCP] Listening on {HOST}:{PORT}")
        while True:
            conn, addr = s.accept()
            with conn:
                with lock:
                    conn.sendall(latest_data)
                print(f"[TCP] Sent data to {addr}")


def socket_debug():
    global latest_data
    pkg = bytes(get_output(), encoding="utf-8")
    with lock:
        latest_data = pkg
    # print(latest_data)


def angle_between(
    p1: np.array, midpt: np.array, p2: np.array, plane=np.array([1, 1, 1])
):
    """Computes the angle with cosine rule between three points in 3D space."""
    ba: np.array = (p1 - midpt) * plane
    bc: np.array = (p2 - midpt) * plane

    cosine_angle = np.dot(ba, bc) / (np.linalg.norm(ba) * np.linalg.norm(bc))
    angle = np.arccos(cosine_angle)

    return np.degrees(angle)


# Compute the angle between two planes in 3D space.
def angle_between_planes(pl1, pl2):
    l1 = pl1[1] - pl1[0]
    l2 = pl1[2] - pl1[0]
    f1 = np.cross(l1, l2)
    l3 = pl2[1] - pl2[0]
    l4 = pl2[2] - pl2[0]
    f2 = np.cross(l3, l4)
    return np.degrees(
        np.arccos(np.dot(f1, f2) / (np.linalg.norm(f1) * np.linalg.norm(f2)))
    )


def bend_degree(angles):
    return angles[0] + angles[2] * 0.3


def classify_pose(joint_angles):
    bend_degrees = [bend_degree(joint_angles[i : i + 3]) for i in range(0, 13, 3)]
    # for d in bend_degrees:
    #     print("{:9.2f}".format(d), end="")
    # print()
    paper_like = 90 * 4 + 60 - sum(bend_degrees)
    rock_like = sum(bend_degrees)
    scissors_like = 180 - bend_degrees[0] - bend_degrees[1] + sum(bend_degrees[2:])
    # print("{:9.2f} {:9.2f} {:9.2f}".format(paper_like, rock_like, scissors_like))
    res = ["paper", "rock", "scissors"]
    global POSE
    POSE = res[np.argmax([paper_like, rock_like, scissors_like])]

    return POSE


def analyze_hand_landmarks(hand_landmarks):
    """Analyze the hand landmarks and return the joint angles."""

    # We have the hand tracker configured to return only one hand, so we can make some assumptions
    # and just grab the first hand_landmarks object, which contains the normalized coordinates
    # of the detected hand.
    hand_landmarks = hand_landmarks.hand_landmarks[0]

    # Convert the hand landmark data into a numpy array of joint positions
    num_landmarks = len(hand_landmarks)
    assert num_landmarks == 21  # Sanity check - demo is built around 21 landmark model

    pt = np.zeros((num_landmarks, 3))
    for i in range(num_landmarks):
        pt[i] = np.array(
            [hand_landmarks[i].x, hand_landmarks[i].y, hand_landmarks[i].z]
        )

    # Create storage for the angles
    ag = np.zeros(NUM_DOFS)

    # First finger, fore or index
    # Angles calculated correspond to knuckle flex, knuckle yaw and long tendon length for all fingers,
    ag[0] = 180 - angle_between(pt[0], pt[5], pt[6])
    ag[1] = 90 - angle_between(pt[9], pt[5], pt[6])
    ag[2] = (
        360 - angle_between(pt[5], pt[6], pt[7]) - angle_between(pt[6], pt[7], pt[8])
    )

    # Second finger, middle
    ag[3] = 180 - angle_between(pt[0], pt[9], pt[10])
    ag[4] = angle_between(pt[5], pt[9], pt[10]) - 90
    ag[5] = (
        360
        - angle_between(pt[9], pt[10], pt[11])
        - angle_between(pt[10], pt[11], pt[12])
    )
    # Third finger, ring
    ag[6] = 180 - angle_between(pt[0], pt[13], pt[14])
    ag[7] = angle_between(pt[9], pt[13], pt[14]) - 90
    ag[8] = (
        360
        - angle_between(pt[13], pt[14], pt[15])
        - angle_between(pt[14], pt[15], pt[16])
    )

    # Fourth finger, pinky
    ag[9] = 180 - angle_between(pt[0], pt[17], pt[18])
    ag[10] = angle_between(pt[13], pt[17], pt[18]) - 90
    ag[11] = (
        360
        - angle_between(pt[17], pt[18], pt[19])
        - angle_between(pt[18], pt[19], pt[20])
    )

    # Thumb, upper, lower, and flexion
    ag[12] = 180 - angle_between(pt[1], pt[2], pt[4])
    ag[13] = 60 - angle_between(pt[2], pt[1], pt[5])
    ag[14] = 180 - angle_between(pt[2], pt[3], pt[4])

    # Palm curl

    ag[15] = 180 - (angle_between_planes([pt[1], pt[0], pt[5]], [pt[0], pt[5], pt[17]]))

    # Wrist pitch and yaw
    ag[16] = 90 - angle_between(
        pt[13],
        pt[0],
        (pt[0] + np.array([0, 0, 1])),
        plane=[0, 1, 1],
    )
    ag[17] = 90 - angle_between(
        pt[13],
        pt[0],
        (pt[0] + np.array([1, 0, 0])),
        plane=[1, 1, 0],
    )

    global POINTS
    POINTS = pt.tolist()
    global ANGLES
    ANGLES = ag

    return ag


def draw_landmarks_on_image(rgb_image, detection_result):
    """Draws the hand landmarks on the image for debugging purposes."""
    hand_landmarks_list = detection_result.hand_landmarks
    handedness_list = detection_result.handedness
    annotated_image = np.copy(rgb_image)

    # Loop through the detected hands to visualize.
    for idx in range(len(hand_landmarks_list)):
        if idx != 0:
            print(
                "WARNING: Only one hand can be detected at a time. Only the first hand will be visualized."
            )
        hand_landmarks = hand_landmarks_list[idx]
        handedness = handedness_list[idx]

        # Draw the hand landmarks.
        hand_landmarks_proto = landmark_pb2.NormalizedLandmarkList()
        hand_landmarks_proto.landmark.extend(
            [
                landmark_pb2.NormalizedLandmark(
                    x=landmark.x, y=landmark.y, z=landmark.z
                )
                for landmark in hand_landmarks
            ]
        )
        solutions.drawing_utils.draw_landmarks(
            annotated_image,
            hand_landmarks_proto,
            solutions.hands.HAND_CONNECTIONS,
            solutions.drawing_styles.get_default_hand_landmarks_style(),
            solutions.drawing_styles.get_default_hand_connections_style(),
        )

        # Get the top left corner of the detected hand's bounding box.
        height, width, _ = annotated_image.shape
        x_coordinates = [landmark.x for landmark in hand_landmarks]
        y_coordinates = [landmark.y for landmark in hand_landmarks]
        text_x = int(min(x_coordinates) * width)
        text_y = int(min(y_coordinates) * height) - MARGIN

        # Draw handedness (left or right hand) on the image.
        handedness_text = ("Right" if handedness[0].index == 1 else "Left") + " " + POSE
        global HANDEDNESS
        HANDEDNESS = handedness_text
        cv2.putText(
            annotated_image,
            handedness_text,
            (text_x, text_y),
            cv2.FONT_HERSHEY_DUPLEX,
            FONT_SIZE,
            HANDEDNESS_TEXT_COLOR,
            FONT_THICKNESS,
            cv2.LINE_AA,
        )

        # Draw marker numbers on the image
        for i in range(21):
            cv2.putText(
                annotated_image,
                f"{i}",
                (int(hand_landmarks[i].x * width), int(hand_landmarks[i].y * height)),
                cv2.FONT_HERSHEY_DUPLEX,
                FONT_SIZE / 2,
                (0, 0, 0),
                FONT_THICKNESS,
                cv2.LINE_AA,
            )

    return annotated_image


# Angle debug display
def draw_angles_on_image(image, joint_angles):
    """Draw the joint angles on the image for debugging purposes"""
    height, width, _ = image.shape

    # Draw the finger angles
    for i in range(NUM_DOFS):
        cv2.putText(
            image,
            f"{i}: {int(joint_angles[i])}",
            (
                int(width * 0.05 + int(i / 3) * width * 0.1),
                int(height * 0.1 + (i % 3) * height * 0.05),
            ),
            cv2.FONT_HERSHEY_DUPLEX,
            FONT_SIZE / 2,
            (0, 0, 0),
            FONT_THICKNESS,
            cv2.LINE_AA,
        )

    # Draw titles for the columns
    cv2.putText(
        image,
        "idx",
        (int(width * 0.05), int(height * 0.05)),
        cv2.FONT_HERSHEY_DUPLEX,
        FONT_SIZE / 2,
        (0, 0, 0),
        FONT_THICKNESS,
        cv2.LINE_AA,
    )
    cv2.putText(
        image,
        "mid",
        (int(width * 0.15), int(height * 0.05)),
        cv2.FONT_HERSHEY_DUPLEX,
        FONT_SIZE / 2,
        (0, 0, 0),
        FONT_THICKNESS,
        cv2.LINE_AA,
    )
    cv2.putText(
        image,
        "rng",
        (int(width * 0.25), int(height * 0.05)),
        cv2.FONT_HERSHEY_DUPLEX,
        FONT_SIZE / 2,
        (0, 0, 0),
        FONT_THICKNESS,
        cv2.LINE_AA,
    )
    cv2.putText(
        image,
        "pnk",
        (int(width * 0.35), int(height * 0.05)),
        cv2.FONT_HERSHEY_DUPLEX,
        FONT_SIZE / 2,
        (0, 0, 0),
        FONT_THICKNESS,
        cv2.LINE_AA,
    )
    cv2.putText(
        image,
        "thb",
        (int(width * 0.45), int(height * 0.05)),
        cv2.FONT_HERSHEY_DUPLEX,
        FONT_SIZE / 2,
        (0, 0, 0),
        FONT_THICKNESS,
        cv2.LINE_AA,
    )
    cv2.putText(
        image,
        "wri",
        (int(width * 0.55), int(height * 0.05)),
        cv2.FONT_HERSHEY_DUPLEX,
        FONT_SIZE / 2,
        (0, 0, 0),
        FONT_THICKNESS,
        cv2.LINE_AA,
    )

    return image


# [MAIN]
async def hand_tracking(tx_queue):
    BaseOptions = mp.tasks.BaseOptions
    HandLandmarker = mp.tasks.vision.HandLandmarker
    HandLandmarkerOptions = mp.tasks.vision.HandLandmarkerOptions
    VisionRunningMode = mp.tasks.vision.RunningMode

    # Create a hand landmarker instance in video mode. This gets mediapipe to
    # run in a synchronous mode. It's a slight deviation from the asynch mode they
    # suggest for real-time, but easier to manage for this demo.
    path = os.path.dirname(os.path.abspath(__file__))
    options = HandLandmarkerOptions(
        base_options=BaseOptions(
            model_asset_path=f"{path}/models/hand_landmarker.task"
        ),
        running_mode=VisionRunningMode.VIDEO,
    )
    with HandLandmarker.create_from_options(options) as landmarker:

        cap = cv2.VideoCapture(0)
        # cut image to 1:1 aspect ratio

        # Get elapsed millis
        ts = millis()

        try:
            while cap.isOpened():

                # Read franme from OpenCV
                ret, frame = cap.read()
                if not ret:
                    break

                # mirror image display
                frame = cv2.flip(frame, 1)

                mp_image = mp.Image(image_format=mp.ImageFormat.SRGB, data=frame)

                # Run hand detection
                detection_result = landmarker.detect_for_video(mp_image, millis() - ts)

                # Draw the hand annotations on the image.
                annotated_image = draw_landmarks_on_image(frame, detection_result)

                # Analyze the hand landmarks
                if detection_result.hand_landmarks:
                    joint_angles = analyze_hand_landmarks(detection_result)
                    if joint_angles is not None:

                        # Push the joint angles into the transmit queue
                        await tx_queue.put(joint_angles)

                        # pose type classification
                        pose = classify_pose(joint_angles)

                        # Draw debug info on image
                        if image_debug_enabled:
                            annotated_image = draw_angles_on_image(
                                annotated_image, joint_angles
                            )

                    # Debug
                    if console_debug_enabled:
                        console_debug()
                    if socket_debug_enabled:
                        socket_debug()

                # Draw the frame
                cv2.imshow("DexHand BLE", annotated_image)

                # Yield
                await asyncio.sleep(sleep_time)

                # Check for keypress
                key = cv2.waitKey(1) & 0xFF

                # Check for escape key
                if key == 27:
                    # Cancel all tasks
                    for task in asyncio.all_tasks():
                        task.cancel()
                # Check for w key to toggle wrist
                elif key == ord("w"):
                    global wrist_enabled
                    wrist_enabled = not wrist_enabled
                    print(f"Wrist enabled: {wrist_enabled}")

        except asyncio.CancelledError:
            print("Hand tracking task has been cancelled.")
            # Clean Up
            cap.release()
            cv2.destroyAllWindows()


# Services and characteristics we use for communication with the hand
UART_SERVICE_UUID = "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
UART_RX_CHAR_UUID = "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
UART_TX_CHAR_UUID = "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

DOF_SERVICE_UUID = "1e16c1b4-1936-4f0e-ab62-5e0a702a4935"
DOF_CHAR_UUID = "1e16c1b5-1936-4f0e-ab62-5e0a702a4935"


async def main():

    # Create the transmit queue
    ble_tx_queue = asyncio.Queue()

    # Create the tasks
    hand_tracking_task = asyncio.create_task(hand_tracking(ble_tx_queue))

    # Start TCP server
    threading.Thread(target=tcp_server, daemon=True).start()

    # Run the tasks concurrently
    try:
        await asyncio.gather(hand_tracking_task, return_exceptions=True)
    except asyncio.exceptions.CancelledError:
        print("Sub tasks have been cancelled. Shutting down.")


asyncio.run(main())
