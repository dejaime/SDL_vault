/////////////////////////////////////////////////////////////////////////
//
// Copyright (c) Dejaime Antônio de Oliveira Neto
//     Created on 20140325 ymd
//
// X11 Licensed Code
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
/////////////////////////////////////////////////////////////////////////

#ifndef AUDIOVAULT_H
#define AUDIOVAULT_H

#define DEFAULT_FREQUENCY 22050
#define DEFAULT_SAMPLE_SIZE 4096
#define VOLUME_MAX 2
#define VOLUME_MIN 0

#include "SDL_mixer.h"
#include <SDL.h>

#include <types.h>

#include <VaultEntry.h>

typedef vault_entry<Mix_Music> MusicEntry;
typedef vault_entry<Mix_Chunk> ChunkEntry;

class AudioVault {
protected:
    std::vector<MusicEntry> m_vMusics;
    std::vector<ChunkEntry> m_vChunks;
    unsigned long m_ulExpirationTime = 0;
    SDL_TimerID m_TimerID = 0;

public:

    std::shared_ptr<Mix_Music*> GetMusic (std::string p_sPath);

    std::shared_ptr<Mix_Chunk*> GetChunk (std::string p_sPath);

    Mix_Music* CheckMusic (std::string p_sPath);

    Mix_Chunk* CheckChunk (std::string p_sPath);

    void SetAutoFree (unsigned long p_ulTimeMS);
    void StopAutoFree ();

    bool FreeUnused ();
    void Purge ();

    inline void SetExpirationTime(unsigned long p_ulExpirationTime) {
        m_ulExpirationTime = p_ulExpirationTime;
    }

    AudioVault(unsigned long p_ulExpirationTime = 0, unsigned long p_ulAutoFreeTime = 0);
    virtual ~AudioVault();
protected:
    static unsigned int TimedFreeUnused(unsigned int, void* p_AudioVault);

    void FreeMusic(MusicEntry p_Entry){
        Mix_FreeMusic( *(p_Entry.m_pData) );
        p_Entry.m_pData.reset();
    }
    void FreeChunk(ChunkEntry p_Entry){
        Mix_FreeChunk( *(p_Entry.m_pData) );
        p_Entry.m_pData.reset();
    }
private:

};

#endif // AUDIOVAULT_H
