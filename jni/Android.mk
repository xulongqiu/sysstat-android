LOCAL_PATH:= $(call my-dir)
TARGET_PLATFORM := android-27
TARGET_ARCH_ABI := armeabi-v7a

include $(CLEAR_VARS)
LOCAL_SRC_FILES:= \
	../sysstat/activity.c \
	../sysstat/common.c \
	../sysstat/count.c \
	../sysstat/format.c \
	../sysstat/ioconf.c \
	../sysstat/json_stats.c \
	../sysstat/pcp_def_metrics.c \
	../sysstat/pcp_stats.c \
	../sysstat/pr_stats.c \
	../sysstat/raw_stats.c \
	../sysstat/rd_sensors.c \
	../sysstat/rd_stats.c \
	../sysstat/rndr_stats.c \
	../sysstat/sa_conv.c \
	../sysstat/sadf_misc.c \
	../sysstat/sa_wrap.c \
	../sysstat/svg_stats.c \
	../sysstat/systest.c \
	../sysstat/sa_common.c \
	../sysstat/xml_stats.c

	#LOCAL_LDFLAGS:= \
	-L ./
LOCAL_MODULE := sysstat
LOCAL_MODULE_FILENAME := libsysstat
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_SRC_FILES:= \
	../sysstat/mpstat.c

LOCAL_STATIC_LIBRARIES := libsysstat
LOCAL_MODULE := mpstat
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_SRC_FILES:= \
	../sysstat/iostat.c \

LOCAL_STATIC_LIBRARIES := \
	libsysstat

LOCAL_MODULE := iostat
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_SRC_FILES:= \
	../sysstat/pidstat.c \

LOCAL_STATIC_LIBRARIES := \
	libsysstat

LOCAL_MODULE := pidstat
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_SRC_FILES:= \
	../sysstat/tapestat.c \

LOCAL_STATIC_LIBRARIES := \
	libsysstat

LOCAL_MODULE := tapestat
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_SRC_FILES:= \
	../sysstat/cifsiostat.c \

LOCAL_STATIC_LIBRARIES := \
	libsysstat

LOCAL_MODULE := cifsiostat
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_SRC_FILES:= \
	../sysstat/sar.c \

LOCAL_STATIC_LIBRARIES := \
	libsysstat

LOCAL_MODULE := sar
include $(BUILD_EXECUTABLE)

#include $(CLEAR_VARS)
#LOCAL_SRC_FILES:= \
	../sysstat/sadc.c \

#LOCAL_STATIC_LIBRARIES := \
	libsysstat

#LOCAL_MODULE := sadc
#include $(BUILD_EXECUTABLE)

#include $(CLEAR_VARS)
#LOCAL_SRC_FILES:= \
	../sysstat/sadf.c \

#LOCAL_STATIC_LIBRARIES := \
	libsysstat

#LOCAL_MODULE := sadf
#include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_SRC_FILES:= \
	stress.c \

LOCAL_MODULE := stress
include $(BUILD_EXECUTABLE)


