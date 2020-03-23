CSRCS += lv_tutorial_images.c
CSRCS += red_flower.c
CSRCS += red_rose_16.c
CSRCS += flower_icon_alpha.c

DEPPATH += --dep-path $(LVGL_DIR)/lv_examples/lv_tutorial/6_images
VPATH += :$(LVGL_DIR)/lv_examples/lv_tutorial/6_images

CFLAGS += "-I$(LVGL_DIR)/lv_examples/lv_tutorial/6_images"
