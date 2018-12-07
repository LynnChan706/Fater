目标
=============

代码结构说明
=============

代码从顶级包开始划分有以下职能：
    camera
        \__ _camera_interface.py #摄像头应用封装
        \__ _camera.py     #摄像头底层操作
        \__ camera_aoi.py  #摄像图驱动封装
        \__ _camera_aoi.so #驱动文件

V2.0发布目标
=============

相机物理和testdata拍照模式

本期不完成以下功能：



V2.0 TODO
=============

* 完成相机类的封装
    * 【最高优先级】完成相机主要接口设计，并使用testdata文件夹模拟相机工作
    * 完成物理相机的工作流程
    * 实地测试相机是否工作
    * 完成相机的设置项目


代码风格
=============

Python代码依照[Google 开源项目代码规范-python风格规范](http://zh-google-styleguide.readthedocs.org/en/latest/google-python-styleguide/python_style_rules/)书写代码
