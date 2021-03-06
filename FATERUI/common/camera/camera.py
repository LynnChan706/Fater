#!/usr/bin/env python2.7
# coding=utf-8

'''
@date = '15/3/30'
@author = 'xiekun'
@email = 'xiekun@cvte.cn'
'''

from .singleton import Singleton


class Camera(Singleton):
    '''
    Camera定义成单例模式，防止多个类同时打开，
    '''
    # TODO： 本类的所有子类需要脱离Settings依赖

    CAMERA_STATUS_UNKNOWN = -1
    CAMERA_STATUS_CONNECTED = 0
    CAMERA_STATUS_UNCONNECTED = 1
    CAMERA_STATUS_CONNECTING = 2

    def __init__(self):
        '''
        如果底层驱动不支持callback形式的回调的话，可以去掉该函数.
        :return: Camera的实例
        '''
        pass

    def get_camera_status(self):
        raise NotImplementedError()

    def open(self):

        '''
        这个方法可以看下是否需要异步操作，通过callback_camera_status_changed返回是否打
        开成功。有些摄像头的打开都需要一定的阻塞时间，如果有阻塞时间，就异步，否则就当前
        的接口定义就足够了

        另外，如果当前摄像机已经打开了摄像头，就直接返回True就成了

        :param camera_name: 如果为None就是打开默认的相机，如果是
            CAMERA_NAME_TEST就是虚拟相机
        :return: 是否打开成功
        '''
        raise NotImplementedError()

    def close(self):
        '''

        :return: 是否关闭成功
        '''
        raise NotImplementedError()

    def take_picture(self, index=None):
        '''

        :return: numpy的图像
        '''
        raise NotImplementedError()

    def get_camera_name(self):
        '''
        获得相机的名称
        :return: str，相机的名称
        '''
        raise NotImplementedError()

    def get_camera_description(self):
        '''
        获得相机的描述
        :return: str， 相机的描述
        '''
        raise NotImplementedError()

    def get_picture_info(self):
        '''
        获得图片的信息
        :return: str
        '''
        raise NotImplementedError()

    def set_parameter(self, **kwargs):
        '''
        设置相机的参数，即时生效
        :param kwargs:
        :return: 是否设置成功
        '''
        raise NotImplementedError()

    def get_parameter(self):
        '''
        获得相机的参数
        :return: dict
        '''
        raise NotImplementedError()
