
from CameraSVS import CameraSVS
import cv2

if __name__ == '__main__':
    camera = CameraSVS()
    camera.open()
    for i in range(1000):
        img = camera.get_image_in_numpy()
        print('frame rate: ', camera.get_frame_rate())
        if img.size > 1:
            cv2.imshow("tests", img)
            cv2.waitKey(1)
    print('end.')
