import cv2
import os
import numpy as np
import time

output_path = "/home/minh/Documents/detec_sign/captured_images"

pTime = 0
cTime = 0
cap = cv2.VideoCapture(0)

while True:
    success, frame = cap.read()

    cTime = time.time()
    fps = 1 / (cTime - pTime)
    pTime = cTime
    cv2.putText(frame, str(int(fps)), (10, 70), cv2.FONT_HERSHEY_PLAIN, 3, (255, 0, 255), 3)

    # Tạo tên file ảnh với thời gian hiện tại
    timestamp = time.strftime("%Y%m%d_%H%M%S")
    image_filename = f"{output_path}/captured_image_{timestamp}.jpg"

    # Lưu ảnh
    cv2.imwrite(image_filename, frame)

    cv2.imshow("Image", frame)

    # Sửa đổi dòng này
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()

# Thư mục chứa dataset
output_dataset_path = '/home/minh/Documents/detec_sign/dataset'

def draw_shape_info(img, cnt, shape_name, i):
    # Lấy tọa độ của bounding box chứa hình dạng
    x, y, w, h = cv2.boundingRect(cnt)
    area = w * h
    if area > 1000 and area < 10000:
        # Vẽ bounding box
        cv2.rectangle(img, (x, y), (x + w, y + h), (0, 255, 0), 2)
        # Cắt tấm ảnh
        cropped_img = img[max(0, y - 50):y + h + 50, max(0, x - 50):x + w + 50]

        # Lưu ảnh vào thư mục tương ứng
        img_path = os.path.join(output_dataset_path, f"image_{i}.png")
        cv2.imwrite(img_path, cropped_img)

        # Đặt tên hình dạng
        font = cv2.FONT_HERSHEY_SIMPLEX
        cv2.putText(img, shape_name, (x, y - 10), font, 0.5, (255, 255, 255), 2, cv2.LINE_AA)


# Lặp qua tất cả các file trong thư mục
for filename in os.listdir(output_path):
    if filename.endswith(".png") or filename.endswith(".jpg"):
        # Đọc ảnh từ file
        image_path = os.path.join(output_path, filename)
        frame = cv2.imread(image_path)

        new_width = int(frame.shape[1])
        new_height = int(frame.shape[0])

        # Sử dụng hàm cv2.resize để phóng to ảnh
        frame = cv2.resize(frame, (640, 640))
        new_frame = frame[:, :]
        image_array = np.array(new_frame)

        def keep_values_in_range(arr, lower_bound, upper_bound):
            result = np.clip(arr, lower_bound, upper_bound)
            return result

        image_array = keep_values_in_range(image_array, 100, 200)

        average_color = np.mean(image_array, axis=(0, 1))
        img_gray = cv2.cvtColor(new_frame, cv2.COLOR_BGR2GRAY)

        threshold_value = int(np.mean(average_color))
        _, threshold = cv2.threshold(img_gray, threshold_value - 25, threshold_value + 10, cv2.THRESH_BINARY)

        contours, _ = cv2.findContours(threshold, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
        i = 0
        for cnt in contours:
            approx = cv2.approxPolyDP(cnt, 0.01 * cv2.arcLength(cnt, True), True)
            x = approx.ravel()[0]
            y = approx.ravel()[1]
            shape_name = ''
            if len(approx) >= 1:
                shape_name = "Tron"
                draw_shape_info(new_frame, cnt, shape_name, i)
                i += 1

        print(f"Processed: {filename}")


cv2.imshow("Threshold", threshold)

def delete_files_in_directory(directory):
    for filename in os.listdir(directory):
        file_path = os.path.join(directory, filename)
        try:
            if os.path.isfile(file_path) or os.path.islink(file_path):
                os.unlink(file_path)
            elif os.path.isdir(file_path):
                os.rmdir(file_path)
        except Exception as e:
            print(f"Không thể xóa {file_path}. Lỗi: {e}")


delete_files_in_directory(output_path)
