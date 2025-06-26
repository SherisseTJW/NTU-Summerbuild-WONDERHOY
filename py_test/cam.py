from ultralytics import YOLO
import cv2


class Cam:
    def __init__(self):
        self.model = YOLO("yolo11n-pose.pt")  # load an official model

    def process_frame(frame) -> tuple[float, float]:
        results = self.model(frame)
        for p in results[0].keypoints.xy[0][9:11]:
            x, y = p
            return (x.item(), y.item())
            # frame = cv2.circle(frame, (x, y), 50, (255, 0, 0), -1) # Blue-Green (Cyan) filled circle
