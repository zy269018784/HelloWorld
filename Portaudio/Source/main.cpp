#include <stdio.h>
#include <stdlib.h>
#include <portaudio.h>

#include <boost/filesystem.hpp>

#define SAMPLE_RATE 48000
#define FRAMES_PER_BUFFER 64
typedef struct {
    FILE* file;
} UserData;

float* PCM_DATA = nullptr;
size_t PCMOffset = 0;
size_t PCMCount = 0;
size_t FileSize = 0;
static int paCallback(const void* inputBuffer, void* outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void* userData)
{
    UserData* data = (UserData*)userData;
    float* out = (float*)outputBuffer;
   // size_t bytesRead = fread(out, sizeof(float), framesPerBuffer * 2, data->file);

    for (int i = 0; i < framesPerBuffer; i++)
    {
        //*out++ = std::min(1.f, std::max(0.f, PCM_DATA[PCMOffset++]));
        //*out++ = std::min(1.f, std::max(0.f, PCM_DATA[PCMOffset++]));

        *out++ = PCM_DATA[PCMOffset++];
        *out++ = PCM_DATA[PCMOffset++];
        //if (PCMOffset >= PCMCount)
        //    PCMOffset = 0;
    }

    if (PCMCount - PCMOffset < framesPerBuffer)
    {
        return paComplete;
    }

    //if (bytesRead < framesPerBuffer) {
    //    // End of file reached, stop the stream
    //    return paComplete;
    //}
    return paContinue;
}
int main()
{
    Pa_Initialize();

    UserData userData;

    /*
        ffmpeg -i 1.mp3 -ch_layout stereo  -acodec pcm_s32le -f s32le output.pcm
        ffplay  -ch_layout stereo -f s32le -ar 48000  output.pcm
        ffplay  -ac 2 -f s32le -ar 48000  output.pcm
    */
    userData.file = fopen("output.pcm", "rb");

    if (!userData.file) {
        printf("Failed to open PCM file\n");
        return -1;
    }
    FileSize = boost::filesystem::file_size("output.pcm");
    PCMCount = FileSize / sizeof(float);
    PCM_DATA = new float[PCMCount];
    printf("PCMCount %u\n", PCMCount);
    memset(PCM_DATA, 0, PCMCount * sizeof(float));
    size_t bytesRead = fread(PCM_DATA, sizeof(float), PCMCount, userData.file);


    PaStreamParameters outputParameters;
    outputParameters.device = Pa_GetDefaultOutputDevice();
    outputParameters.channelCount = 2; // Mono
    outputParameters.sampleFormat = paFloat32; // Float32 format
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;
    PaStream* stream;
    Pa_OpenStream(&stream, NULL, &outputParameters, SAMPLE_RATE,
        FRAMES_PER_BUFFER, paClipOff, paCallback, &userData);
    if (stream == NULL) {
        printf("Failed to open PortAudio stream\n");
        return -1;
    }
    Pa_StartStream(stream);
    printf("Playing PCM file...\n");
    Pa_Sleep(1000 * 157); // Play for 5 seconds (adjust as needed)
    Pa_StopStream(stream);
    Pa_CloseStream(stream);
    fclose(userData.file);
    Pa_Terminate();
    return 0;
}