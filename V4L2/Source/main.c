#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>

int EnumerateAudioInputs(char* File);
int EnumerateVideoInputs(char* File);
int main(int argc, char **argv)
{

	return EnumerateVideoInputs("/dev/video0");
	printf("hello v4l2\n");

	int fd = open("/dev/video0", O_CREAT);
	struct v4l2_capability capability;
	
	ioctl(fd, VIDIOC_QUERYCAP, &capability);

	printf("%s\n", capability.driver);
	printf("%s\n", capability.card);
	printf("%s\n", capability.bus_info);
	printf("%u\n", capability.version);

	if ((V4L2_CAP_VIDEO_CAPTURE & capability.capabilities) | (V4L2_CAP_VIDEO_CAPTURE_MPLANE & capability.capabilities))
		printf("support video capture interface\n");

	close(fd);
	return 0;
}