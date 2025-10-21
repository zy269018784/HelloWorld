#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>

int EnumerateAudioInputs(char *File)
{
	int FD = open(File, O_RDONLY);

	struct v4l2_audio Arg;
	memset(&Arg, 0, sizeof(Arg));
	Arg.index = 0;

	int ret = 0;

	while (1)
	{
		ret = ioctl(FD, VIDIOC_ENUMAUDIO, &Arg);
		if (0 != ret)
		{		
			if (EINVAL == errno)
			{
				break;
			}
			break;
		}
		Arg.index++;
		printf("Arg.index %d\n", Arg.index);
	}


	close(FD);
}


int EnumerateVideoInputs(char* File)
{
	int FD = open(File, O_RDONLY);

	struct v4l2_input  Arg;
	memset(&Arg, 0, sizeof(Arg));
	Arg.index = 0;

	int ret = 0;

	while (1)
	{
		ret = ioctl(FD, VIDIOC_ENUMINPUT, &Arg);
		if (0 != ret)
		{
			if (EINVAL == errno)
			{
				break;
			}
			break;
		}
		Arg.index++;
		printf("Arg.index %d\n", Arg.index);
		printf("Arg.name %s\n", Arg.name);
		printf("Arg.type %d\n", Arg.type);
		printf("Arg.audioset %d\n", Arg.audioset);
		printf("Arg.capabilities %d\n", Arg.capabilities);
	}


	close(FD);
}
