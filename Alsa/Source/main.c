#include <stdio.h>
#include <alsa/asoundlib.h>
/*
    https://www.alsa-project.org/alsa-doc/alsa-lib/index.html
*/
int main()
{
    printf("hello ALSA\n");
    int card_num = -1;
    int err;

    // 初始化，获取第一个声卡
    err = snd_card_next(&card_num);
    if (err < 0) {
        printf("no sound card found: %s\n", snd_strerror(err));
        return -1;
    }

    // 如果没有声卡
    if (card_num < 0) {
        printf("no sound card found\n");
        return 0;
    }

    int count = 0;
    while (card_num >= 0) {
        count++;
        err = snd_card_next(&card_num);
        if (err < 0) {
            printf("snd_card_next error: %s\n", snd_strerror(err));
            break;
        }
        char* name = NULL;
        snd_card_get_name(card_num, &name);
        printf("sound card %s\n", name);
    }

    printf("we get %d sound card\n", count);
    return 0;
}