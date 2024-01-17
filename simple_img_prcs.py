--allow-unrelated-historiesfrom scipy import signal
import numpy as np
import cv2
from multiprocessing import Pool
from functools import partial


def lane_histogram(img, height_start=800, height_end=1250):
    histogram = np.sum(img[int(height_start):int(height_end), 🙂, axis=0)

    return histogram


def lane_peaks(histogram):
    peaks = signal.find_peaks_cwt(histogram, np.arange(1, 150), min_length=150)

    midpoint = int(histogram.shape[0] / 2)
    # if we found at least two peaks use the signal approach (better in shadows)
    if len(peaks) > 1:
        # in case more then 2 found just get the left and right one
        peak_left, *_, peak_right = peaks

    # otherwise just choose the highest points in left and right of center segments
    else:

        peak_left = np.argmax(histogram[:midpoint])
        peak_right = np.argmax(histogram[midpoint:]) + midpoint

    return peak_left, peak_right


class WindowBox(object):
    def __init__(self, binimg, x_center, y_top, width=100, height=80, mincount=50, lane_found=False):
        self.x_center = x_center
        self.y_top = y_top
        self.width = width
        self.height = height
        self.mincount = mincount
        self.lane_found = lane_found

        # derived
        self.x_left = self.x_center - int(self.width / 2)
        self.x_right = self.x_center + int(self.width / 2)
        self.y_bottom = self.y_top - self.height
        self.imgwindow = binimg[self.y_bottom:self.y_top,
                         self.x_left:self.x_right]
        self.nonzeroy = self.imgwindow.nonzero()[0]
        self.nonzerox = self.imgwindow.nonzero()[1]

    def center(self):
        return (self.x_center, int(self.y_top - self.y_bottom) / 2)

    def next_windowbox(self, binimg):
        if self.has_line():
            # recenter based on mean
            x_center = int(np.mean(self.nonzerox + self.x_left))
        else:
            # use existing center
            x_center = self.x_center

        # next box top starts at lasts bottom
        y_top = self.y_bottom

        return WindowBox(binimg, x_center, y_top,
                         width=self.width, height=self.height, mincount=self.mincount,
                         lane_found=self.lane_found)

    def _nonzerox_count(self):
        return len(self.nonzerox)

    def _is_noise(self):
        return self._nonzerox_count() > self.imgwindow.shape[0] * self.imgwindow.shape[1] * .75

    def has_line(self):
        return (self._nonzerox_count() > self.mincount) ^ self._is_noise()

    def has_lane(self):
        if not self.lane_found and self.has_line():
            self.lane_found = True
        return self.lane_found

    def __str__(self):
        return "WindowBox [%.3f, %.3f, %.3f, %.3f]" % (self.x_left,
                                                       self.y_bottom,
                                                       self.x_right,
                                                       self.y_top)


def find_lane_windows(window_box, binimg):
    boxes = []

    # keep searching up the image for a lane line and append the boxes
    continue_lane_search = True
    contiguous_box_no_line_count = 0
    while (continue_lane_search and window_box.y_top >= 0):
        if window_box.has_line():
            boxes.append(window_box)

        window_box = window_box.next_windowbox(binimg)

        # if we've found the lane and can no longer find a box with a line in it
        # then its no longer worth while searching
        if window_box.has_lane():
            if window_box.has_line():
                contiguous_box_no_line_count = 0
            else:
                contiguous_box_no_line_count += 1

                if contiguous_box_no_line_count >= 4:
                    continue_lane_search = False

    return boxes


def calc_lane_windows(binimg, nwindows=12, width=100):
    # calc height of each window
    height = int(binimg.shape[0] / nwindows)

    # find left and right lane centers to start with
    peak_left, peak_right = lane_peaks(lane_histogram(binimg))

    # Initialise left and right window boxes
    ytop = binimg.shape[0]

    wbl = WindowBox(binimg, peak_left, ytop, width=width, height=height)
    #     left_boxes = find_lane_windows(wbl, binimg)

    wbr = WindowBox(binimg, peak_right, ytop, width=width, height=height)
    #     right_boxes = find_lane_windows(wbr, binimg)

    # paralleise the search
    find_lane_windows_binimg = partial(find_lane_windows, binimg=binimg)
    left_boxes, right_boxes = map(find_lane_windows_binimg, [wbl, wbr])

    return (left_boxes, right_boxes)


def calc_fit_from_boxes(boxes):
    if len(boxes) > 0:
        # flaten and adjust all boxes for the binary images
        xs = np.concatenate([b.nonzerox + b.x_left for b in boxes])
        ys = np.concatenate([b.nonzeroy + b.y_bottom for b in boxes])

        # return the polynominal
        return np.polyfit(ys, xs, 2)
    else:
        return None


def poly_fitx(fity, line_fit):
    fit_linex = line_fit[0] * fity ** 2 + line_fit[1] * fity + line_fit[2]
    return fit_linex


def calc_lr_fit_from_polys(binimg, left_fit, right_fit, margin=100):
    nonzero = binimg.nonzero()
    nonzeroy = np.array(nonzero[0])
    nonzerox = np.array(nonzero[1])

    def window_lane(poly):
        return (
                (nonzerox > (poly[0] * (nonzeroy ** 2) + poly[1] * nonzeroy + poly[2] - margin))
                & (nonzerox < (poly[0] * (nonzeroy ** 2) + poly[1] * nonzeroy + poly[2] + margin))
        )

    def window_polyfit(lane_inds):
        xs = nonzerox[lane_inds]
        ys = nonzeroy[lane_inds]

        # return the polynominal
        return np.polyfit(ys, xs, 2)

    new_left_fit = left_fit
    if left_fit is not None:
        new_left_fit = window_polyfit(window_lane(left_fit))

    new_right_fit = right_fit
    if right_fit is not None:
        new_right_fit = window_polyfit(window_lane(right_fit))

    return (new_left_fit, new_right_fit)


def read_image_and_draw_circle(video_path, threshold_value=128, window_width=20):
    # Mở video
    cap = cv2.VideoCapture(video_path)

    while cap.isOpened():
        # Đọc frame từ video
        ret, frame = cap.read()

        if not ret:
            break

        # Chuyển đổi frame sang ảnh xám
        gray_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        # Áp dụng ngưỡng để nhận diện đường
        _, binary_frame = cv2.threshold(gray_frame, threshold_value, 255, cv2.THRESH_BINARY)
        cv2.imshow('binary',binary_frame)
        # Tính histogram và peaks
        hist = lane_histogram(binary_frame, 150, 200)
        peak_left, peak_right = lane_peaks(hist)
        cv2.line(frame, (0, 150), (frame.shape[1], 150), (255, 0, 255), 2)
        cv2.line(frame, (0, 200), (frame.shape[1], 200), (255, 0, 255), 2)
        if peak_right<250:
            peak_right=peak_left+300
        if peak_left>50:
            peak_left=peak_right-300
        if abs(peak_left-peak_right)<250:
            print('250',peak_left,peak_right)
        # Tính tọa độ trung tâm
        center_point = int((peak_left + peak_right) / 2)

        # Vẽ circle trên frame
        cv2.circle(frame, (center_point, 200), 5, (180, 180, 255), 2)
        cv2.circle(frame, (peak_left, 200), 5, (50, 0, 50), 2)
        cv2.circle(frame, (peak_right, 200), 5, (50, 0, 50), 2)
        cv2.circle(frame, (160, 200), 5, (0, 0, 255), 2)

        # Hiển thị frame
        cv2.imshow('Frame', frame)

        # Kiểm tra phím ESC để thoát
        if cv2.waitKey(25) & 0xFF == 27:
            break

    # Giải phóng video capture và đóng cửa sổ hiển thị khi kết thúc
    cap.release()
    cv2.destroyAllWindows()

# Đường dẫn của video
video_path = 'D:/pythonProject1/test_simple_video.mp4'

# Gọi hàm để đọc ảnh từ video và vẽ circle
read_image_and_draw_circle(video_path)