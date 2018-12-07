/* CameraBobcat.i */
%module CameraBobcat


%{
/* 相关引用 */
	#define SWIG_FILE_WITH_INIT
    #include "CameraBobcat.h"
%}

/* 对STL的支持 */
%include stl.i
%include "typemaps.i"
%include numpy.i

%init %{
	import_array();
%}

%apply (unsigned char** ARGOUTVIEWM_ARRAY3, int* DIM1, int* DIM2, int* DIM3) {(unsigned char **arr_out, int *dim1_out, int *dim2_out, int *dim3_out)}

/* Wrapper并生成代码 */
%include "CameraBobcat.h"
%extend CameraBobcat{
	void CameraBobcat::get_image_in_numpy(unsigned char **arr_out, int *dim1_out, int *dim2_out, int *dim3_out){
        unsigned char * image = self->capture_one_frame();
        int retry_time = 1;
        while (image == NULL && retry_time < CAPTURE_RETRY_TIME) {
            image = self->capture_one_frame();
            retry_time++;
        }
        if (image!=NULL){
    	    *arr_out = image;
            *dim1_out = self->height;
            *dim2_out = self->width;
            *dim3_out = 3;
        }else{
            *arr_out = new unsigned char[1];
            *dim1_out = 1;
            *dim2_out = 1;
            *dim3_out = 1;
        }
	};
}
