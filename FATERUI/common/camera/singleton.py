#!/usr/bin/env python2.7
# coding=utf-8

'''
@date = '15/3/25'
@author = 'xiekun'
@email = 'xiekun@cvte.cn'
'''


class Singleton(object):
    '''
    单例模式，所有单例模式的公共基类
    '''

    def __new__(cls, *args, **kw):
        if not hasattr(cls, '_instance'):
            orig = super(Singleton, cls)
            cls._instance = orig.__new__(cls, *args, **kw)
        return cls._instance


def singleton(cls, *args, **kw):
    instances = {}
    def _singleton():
        if cls not in instances:
            instances[cls] = cls(*args, **kw)
        return instances[cls]
    return _singleton