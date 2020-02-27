#pragma once

#include "common.h"
#include <fstream>

void test_open_image();
void test_open_images_fld();
void test_image_open_and_save();
void my_call_back_func(int event, int x, int y, int flags, void* param);
void test_mouse_click();
int open_new_file(char *fname);
void write_to_new_file(Mat *img);
char clamp_to_char(int x);
int get_file_size(char *fname);
