/*
	打开V4L2设备: open
	关闭V4L2设备: close
	Read from a V4L2 device:
		ssize_t read(int fd, void *buf, size_t count)

	Map device memory into application address space:
		void *mmap(void *start, size_t length, int prot, int flags, int fd, off_t offset)

	Query device capabilities:
		int ioctl(int fd, VIDIOC_QUERYCAP, struct v4l2_capability *argp)

	Enumerate video outputs:
		int ioctl(int fd, VIDIOC_ENUMOUTPUT, struct v4l2_output *argp)

	Enumerate video outputs:
		int ioctl(int fd, VIDIOC_ENUMOUTPUT, struct v4l2_output *argp)

	Enumerate audio inputs:
		int ioctl(int fd, VIDIOC_ENUMAUDIO, struct v4l2_audio *argp)

	Enumerate audio outputs:
		int ioctl(int fd, VIDIOC_ENUMAUDOUT, struct v4l2_audioout *argp)

	Enumerate controls and menu control items:
		int ioctl(int fd, int VIDIOC_QUERYCTRL, struct v4l2_queryctrl *argp)
		int ioctl(int fd, VIDIOC_QUERY_EXT_CTRL, struct v4l2_query_ext_ctrl *argp)
		int ioctl(int fd, VIDIOC_QUERYMENU, struct v4l2_querymenu *argp)

	Get or set tuner attributes:
		int ioctl(int fd, VIDIOC_G_TUNER, struct v4l2_tuner *argp)
		int ioctl(int fd, VIDIOC_S_TUNER, const struct v4l2_tuner *argp)

	Get or set modulator attributes:
		int ioctl(int fd, VIDIOC_G_MODULATOR, struct v4l2_modulator *argp)
		int ioctl(int fd, VIDIOC_S_MODULATOR, const struct v4l2_modulator *argp)

	Get or set the data format, try a format:
		int ioctl(int fd, VIDIOC_G_FMT, struct v4l2_format *argp)
		int ioctl(int fd, VIDIOC_S_FMT, struct v4l2_format *argp)
		int ioctl(int fd, VIDIOC_TRY_FMT, struct v4l2_format *argp)
	Get or set the current cropping rectangle:
		int ioctl(int fd, VIDIOC_G_CROP, struct v4l2_crop *argp)
		int ioctl(int fd, VIDIOC_S_CROP, const struct v4l2_crop *argp)

	Get or set streaming parameters:
		int ioctl(int fd, VIDIOC_G_PARM, v4l2_streamparm *argp)
		int ioctl(int fd, VIDIOC_S_PARM, v4l2_streamparm *argp)

	Subscribe or unsubscribe event:
		int ioctl(int fd, VIDIOC_SUBSCRIBE_EVENT, struct v4l2_event_subscription *argp)
		int ioctl(int fd, VIDIOC_UNSUBSCRIBE_EVENT, struct v4l2_event_subscription *argp)

	Dequeue event:
		int ioctl(int fd, VIDIOC_DQEVENT, struct v4l2_event *argp)

	Input Types:
		V4L2_INPUT_TYPE_TUNER
		V4L2_INPUT_TYPE_CAMERA
		V4L2_INPUT_TYPE_TOUCH

	Control IDs:
		V4L2_CID_BASE					First predefined ID, equal to V4L2_CID_BRIGHTNESS.
		V4L2_CID_USER_BASE				Synonym of V4L2_CID_BASE.
		V4L2_CID_BRIGHTNESS (integer)	Picture brightness, or more precisely, the black level.
		V4L2_CID_CONTRAST (integer)		Picture contrast or luma gain.

	Interfaces:
		Video Capture Interface
		Video Overlay Interface
		Video Output Interface
		Video Output Overlay Interface
		Video Memory-To-Memory Interface
		Raw VBI Data Interface
		Sliced VBI Data Interface
		Radio Interface
		RDS Interface
		Software Defined Radio Interface (SDR)
		Touch Devices
		Event Interface
		Sub-device Interface
		Metadata Interface
*/