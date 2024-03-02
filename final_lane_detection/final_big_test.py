import cv2
from Lane_detection import Lane  # Replace 'your_module_name' with the actual name of your module
import pickle
import time

# Function to pause and resume video
def pause_video():
    while True:
        key = cv2.waitKey(0)
        if key == ord('p'):
            break

# Initialize the camera calibration parameters
# with open('/home/lucis/PycharmProject/Bosch_car/Lane_detection/calibration_data.pkl', 'rb') as f:
#     objpoints, imgpoints, chessboards = pickle.load(f)
# ret, mtx, dist, dst = camera_calibrate(objpoints, imgpoints, img)
# Your camera calibration function (camera_calibrate) should be called here to get calibration parameters

# Initialize Lane object
lane = Lane(img_height=240, img_width=320)  # Replace with actual calibration parameters


# Hàm khởi tạo một lane mới
# Định nghĩa các biến toàn cục
lane_threshold = 0  # Giả sử ban đầu không có ngưỡng nào

def create_new_lane_if_needed(distance_threshold,distance_threshold_far):
    global lane
    global lane_threshold

    # Lấy thời gian hiện tại
    current_time = cv2.getTickCount()

    # Kiểm tra nếu khoảng cách nhỏ hơn ngưỡng
    if distance < distance_threshold or distance > distance_threshold_far:
        # Khởi tạo một lane mới
        lane.road_line_left = None
        lane.road_line_right = None
        # Cập nhật lại ngưỡng để tránh tạo lane liên tục trong một khoảng thời gian ngắn
        lane_threshold = current_time + 5 * cv2.getTickFrequency()  # Ví dụ: sau 5 giây mới được tạo lane mới


# Initialize VideoCapture with the video file path
video_path = ('/home/lucis/PycharmProject/Bosch_car/Folder_video_test/vid_2.mp4')
cap = cv2.VideoCapture(video_path)

# Check if video capture was successful
if not cap.isOpened():
    print("Error: Could not open video.")
    exit()

# Variable to control pausing
paused = False




# Loop through each frame of the video
while True:
    # Capture frame-by-frame
    ret, frame = cap.read()

    # Check if reading the frame was successful
    if not ret:
        print("Error: Could not read frame.")
        break

    # If the video is paused, skip processing and display the current frame
    if not paused:
        # Apply lane detection on the frame
        #frame_roi = frame[300:1110, 0:1440]

        lane.image = frame  # This will trigger the processing pipeline in your Lane class

        # Get the decorated result with lane information
        result,distance = lane.result_decorated
        #print('distance =',distance)
        create_new_lane_if_needed(150,160)

        # Display the result
        cv2.imshow('Lane Detection', result)

    # Check for key presses
    key = cv2.waitKey(1)

    # Pause or resume the video on 'p' key press
    if key == ord('p'):
        paused = not paused
        if paused:
            print("Video paused. Press 'p' to resume.")
            pause_video()

    # Exit the loop if 'q' key is pressed
    elif key == ord('q'):
        break



# Release resources and close all OpenCV windows
cap.release()
cv2.destroyAllWindows()
