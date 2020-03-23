CSRCS += lv_test_cpicker.c

DEPPATH += --dep-path $(LVGL_DIR)/lv_examples/lv_tests/lv_test_objx/lv_test_cpicker
VPATH += :$(LVGL_DIR)/lv_examples/lv_tests/lv_test_objx/lv_test_cpicker

CFLAGS += "-I$(LVGL_DIR)/lv_examples/lv_tests/lv_test_objx/lv_test_cpicker"
