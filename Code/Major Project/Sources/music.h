#ifndef MUSIC_HEADER
#define MUSIC_HEADER


void initMusic();

void setMusic(char *command, int loop);

__interrupt void noteInterrupt(void);
__interrupt void nextNoteInterrupt(void);

#endif