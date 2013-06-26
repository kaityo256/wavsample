/*
  Copyright (C) 2013, Hiroshi Watanabe
  All rights reserved.

  Redistribution and use in source and binary forms,
  with or without modification, are permitted provided
  that the following conditions are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials
      provided with the distribution.
    * Neither the name of <organization> nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include <stdio.h>
#include <math.h>

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;

const int WAVE_FORMAT_PCM = 1;
typedef struct { 
  WORD  wFormatTag; 
  WORD  nChannels; 
  DWORD nSamplesPerSec; 
  DWORD nAvgBytesPerSec; 
  WORD  nBlockAlign; 
  WORD  wBitsPerSample; 
  WORD  cbSize; 
} WAVEFORMATEX; 

//---------------------------------------------------------------------------
void SaveWavFile(const char *filename){

  const DWORD SRATE = 11025; //Sampling Rate (11025Hz)
  const int totalsec = 2;  //Length of Data [sec]
  const DWORD dwFmtSize = 16;
  const DWORD dwWaveSize = SRATE*totalsec;  //Size of Data
  const DWORD dwFileSize = dwWaveSize + 36; //Size of File

  WAVEFORMATEX wfe;

  FILE *fp = fopen(filename,"wb");
  if(fp == NULL){
    printf("Failed to open %s.",filename);
    return;
  }

  fwrite("RIFF",sizeof(BYTE),4,fp);
  fwrite(&dwFileSize,sizeof(DWORD),1,fp);
  fwrite("WAVE",sizeof(BYTE),4,fp);

  wfe.wFormatTag = WAVE_FORMAT_PCM;  //Fileformat (PCM)
  wfe.nChannels = 1;
  wfe.nSamplesPerSec = SRATE;
  wfe.nAvgBytesPerSec = SRATE;
  wfe.wBitsPerSample = 8;
  wfe.nBlockAlign = wfe.nChannels * wfe.wBitsPerSample / 8;

  unsigned char *bWave = new unsigned char[dwWaveSize];

  const double Pi = M_PI;
  double f = (double)440.0/(double)SRATE; //(440.0Hz)
  for(int i=0;i<dwWaveSize;i++){
    double phase = (double)i*f*Pi*2.0;
    bWave[i] = (unsigned char)(sin(phase)*128+128);
  }

  fwrite("fmt ",sizeof(BYTE),4,fp);
  fwrite(&dwFmtSize,sizeof(DWORD),1,fp);
  fwrite(&wfe,1,16,fp);
  fwrite("data",sizeof(BYTE),4,fp);
  fwrite(&dwWaveSize,sizeof(DWORD),1,fp);
  fwrite(bWave,sizeof(BYTE),dwWaveSize,fp);

  delete bWave;
  fclose(fp);
}
//---------------------------------------------------------------------------
int main(void){
  SaveWavFile("test.wav");
}
//---------------------------------------------------------------------------

