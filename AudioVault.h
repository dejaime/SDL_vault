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
    ////////////////////////////////////////////////
    /// Checks if a music exists in the vault and returns a strong reference to it. Loads from disk otherwise.
    /// @param p_sPath The path to the sound file.
    /// @return Shared Pointer to the music. Returns a NULL pointer if it can't find and fails loading the file.
    ////////////////////////////////////////////////
    std::shared_ptr<Mix_Music*> GetMusic (std::string p_sPath);

    ////////////////////////////////////////////////
    /// Pushes a music into the vault.
    /// @param p_sPath The path to the sound file.
    /// @param p_pMusic A pointer to a music. Must be valid.
    /// @return Shared Pointer to the passed music.
    /// @warning This doesn't check if p_sPath is unique. Using a p_sPath that already exists is an error.
    ////////////////////////////////////////////////
    std::shared_ptr<Mix_Music*> PushNewMusic (Mix_Music* p_pMusic, std::string p_sPath);

    ////////////////////////////////////////////////
    /// Checks if a chunk exists in the vault and returns a strong reference to it. Loads from disk otherwise.
    /// @param p_sPath The path to the sound file.
    /// @return Shared Pointer to the chunk. Returns a NULL pointer if it can't find and fails loading the file.
    ////////////////////////////////////////////////
    std::shared_ptr<Mix_Chunk*> GetChunk (std::string p_sPath);

    ////////////////////////////////////////////////
    /// Pushes a chunk into the vault.
    /// @param p_pChunk A pointer to a chunk. Must be valid.
    /// @param p_sPath The path to the sound file.
    /// @return Shared Pointer to the passed chunk.
    /// @warning This doesn't check if p_sPath is unique. Using a p_sPath that already exists is an error.
    ////////////////////////////////////////////////
    std::shared_ptr<Mix_Chunk*> PushNewChunk (Mix_Chunk* p_pChunk, std::string p_sPath);

    ////////////////////////////////////////////////
    /// Checks if a music exists in the vault and returns a direct pointer to it.
    /// @param p_sPath The path to the sound file.
    /// @return A valid pointer if the music is found, NULL otherwise.
    ////////////////////////////////////////////////
    Mix_Music* CheckMusic (std::string p_sPath);

    ////////////////////////////////////////////////
    /// Checks if a chunk exists in the vault and returns a direct pointer to it.
    /// @param p_sPath The path to the sound file.
    /// @return A valid pointer if the chunk is found, NULL otherwise.
    ////////////////////////////////////////////////
    Mix_Chunk* CheckChunk (std::string p_sPath);

    ////////////////////////////////////////////////
    /// Checks if a chunk exists in the vault and returns a strong reference to it. Loads from disk otherwise.
    /// @param p_ulTimeMS Expiration time in milliseconds. If it is <= 0 it will disable the automatic free.
    /// @see StopAutoFree()
    /// @see FreeUnused()
    ////////////////////////////////////////////////
    void SetAutoFree (unsigned long p_ulTimeMS);

    ////////////////////////////////////////////////
    /// Turns automatic freeing off.
    /// @see SetAutoFree()
    /// @see FreeUnused()
    ////////////////////////////////////////////////
    void StopAutoFree ();

    ////////////////////////////////////////////////
    /// Manual call to free all unused (and expired) assets.
    /// @see SetAutoFree()
    /// @see StopAutoFree()
    ////////////////////////////////////////////////
    bool FreeUnused ();

    ////////////////////////////////////////////////
    /// Destroys all assets contained in the vault. Unused or not.
    /// @warning May leave orphan pointers! Remember, the shared_ptr are pointers to pointers.
    ////////////////////////////////////////////////
    void Purge ();

    ////////////////////////////////////////////////
    /// Sets an expiration time that an asset needs to be unused before being freed.
    /// @see FreeUnused();
    /// @see SetAutoFree();
    ////////////////////////////////////////////////
    inline void SetExpirationTime(unsigned long p_ulExpirationTime) {
        m_ulExpirationTime = p_ulExpirationTime;
    }

    ////////////////////////////////////////////////
    /// Constructor for the AudioVault.
    /// @param p_ulExpirationTime The time an asset needs to stay unused before being freed.
    /// @param p_ulAutoFreeTime Automatic FreeUnused() call period. 0 will disable it.
    /// @see StopAutoFree()
    /// @see FreeUnused()
    ////////////////////////////////////////////////
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
