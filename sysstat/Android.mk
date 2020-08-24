LOCAL_PATH:= $(call my-dir)
TARGET_PLATFORM := android-27
TARGET_ARCH_ABI := armeabi-v7a

include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	demo.c  \
	audiofile.c \
	activity.c \
	cifsiostat.c \
	common.c \
	count.c \
	format.c \
	ioconf.c \
	iostat.c \
	json_stats.c \
	mpstat.c \
	pcp_def_metrics.c \
	pcp_stats.c \
	pidstat.c \
	pr_stats.c \
	raw_stats.c \
	rd_sensors.c \
	rd_stats.c \
	rndr_stats.c \
	sa_common.c \
	sa_conv.c \
	sadc.c \
	sadf.c \
	sadf_misc.c \
	sar.c \
	sa_wrap.c \
	svg_stats.c \
	systest.c \
	tapestat.c \
	xml_stats.c

#LOCAL_LDFLAGS:= \
	-L ./

LOCAL_LDLIBS := \
	-llog		\

LOCAL_STATIC_LIBRARIES := \

LOCAL_MODULE := mpstat

LOCAL_C_INCLUDES :=  $(LOCAL_PATH)/include

include $(BUILD_EXECUTABLE)

