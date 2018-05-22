#ifndef WAVFILE
#define WAVFILE
#include <cstdio>
#include <cstring>
#include <cstdlib>

typedef struct WAV_HEADER{
  char        RIFF[4];    // RIFF Header   Magic header
  unsigned int    ChunkSize;   // RIFF Chunk Size
  char        WAVE[4];    // WAVE Header
  char        fmt[4];     // FMT header
  unsigned int    Subchunk1Size; // Size of the fmt chunk
  unsigned short   AudioFormat;  // Audio format 1=PCM,6=mulaw,7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
  unsigned short   NumOfChan;   // Number of channels 1=Mono 2=Sterio
  unsigned int     SamplesPerSec; // Sampling Frequency in Hz
  unsigned int    bytesPerSec;  // bytes per second
  unsigned short   blockAlign;   // 2=16-bit mono, 4=16-bit stereo
  unsigned short   bitsPerSample; // Number of bits per sample
  char        Subchunk2ID[4]; // "data" string
  unsigned int    Subchunk2Size; // Sampled data length
}wav_hdr;

class createWavHead {

public:
    wav_hdr *CHead( int samprate,int ch ,int bit,int size)
    {
        wav_hdr *head = new wav_hdr;
        if(head!=nullptr){
        strncpy(head->RIFF,"RIFF",4);
        strncpy(head->WAVE,"WAVE",4);
        strncpy(head->fmt,"fmt ",4);
        head->ChunkSize = size-8;
        head->Subchunk1Size = 0x0010;
        head->AudioFormat = 1;
        head->NumOfChan = ch;
        head->SamplesPerSec =samprate;
        head->bytesPerSec = samprate * (bit / 8) * ch;
        head->blockAlign=(bit * ch / 8);
        head->bitsPerSample=bit;
        strncpy(head->Subchunk2ID,"data",4);
        head->Subchunk2Size= size-44;
        }
        return head;
    }

    void deletehead(wav_hdr *head){
        if(head !=nullptr)delete head;

    }

};

#endif // WAVFILE

