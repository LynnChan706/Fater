#!/usr/bin/env python2.7
# coding=utf-8

'''
@date = '15-5-31'
@author = 'xiekun'
@email = 'xiekun@cvte.com'
'''
import os
import time

import cv2

from .camera_pointgrey import Pointgrey


camera = Pointgrey()

print(camera.get_camera_description())
print(camera.get_picture_info())
if camera.open() is False:
    exit()

index = 0
before = time.time()
for i in range(100):
    img = camera.take_picture()
    print(index)
    index += 1
    if img is not None and img.size > 1:
        # cv2.imshow("tests", img)
        filename = os.path.expanduser("~/Pictures/%d.png" % index)
        cv2.imwrite(filename, img)

        # cv2.waitKey(100)
cv2.destroyAllWindows()
after = time.time()

print('error frame:', camera.get_error_frame_count())
print('total frame count:', camera.get_frame_count())
print('total time:', after - before)
