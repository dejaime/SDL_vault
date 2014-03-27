#include "AudioVault.h"
#include <iostream>

AudioVault::AudioVault():
    m_vMusics(), m_vChunks(), m_ulExpirationTime(0) {
    //ctor
}

AudioVault::~AudioVault() {
    //dtor
}

std::shared_ptr<Mix_Music*> AudioVault::GetMusic(std::string p_sPath) {
    for (auto t_Entry : m_vMusics)
        if (t_Entry.m_sPath == p_sPath)
            return t_Entry.m_pData;

    Mix_Music* t_pMusic = Mix_LoadMUS(p_sPath.c_str());

    if (t_pMusic == NULL) return std::shared_ptr<Mix_Music*>();


    MusicEntry t_MusicEntry(p_sPath);

    t_MusicEntry.m_pData.reset(&t_pMusic);

    //Pushes it into the vector.
    m_vMusics.push_back(t_MusicEntry);

    //Returns the entry, with the strong reference.
    return t_MusicEntry.m_pData;

}

std::shared_ptr<Mix_Chunk*> AudioVault::GetChunk(std::string p_sPath) {
    for (auto t_Entry : m_vChunks)
        if (t_Entry.m_sPath == p_sPath)
            return std::shared_ptr<Mix_Chunk*>(t_Entry.m_pData);

    //Note:
    //  The following LoadWAV function also supports other formats such
    //      as OGG, MIDI or MP3.
    Mix_Chunk* t_pChunk = Mix_LoadWAV(p_sPath.c_str());

    if (t_pChunk == NULL) return std::shared_ptr<Mix_Chunk*>();

    ChunkEntry t_ChunkEntry(p_sPath);

    t_ChunkEntry.m_pData.reset(&t_pChunk);

    //Pushes it into the vector.
    m_vChunks.push_back(t_ChunkEntry);

    //Returns the entry, with the strong reference.
    return std::shared_ptr<Mix_Chunk*>(&t_pChunk);
}

Mix_Music* AudioVault::CheckMusic(std::string p_sPath) {
    for (auto t_Entry : m_vMusics)
        if (t_Entry.m_sPath == p_sPath)
            return *(t_Entry.m_pData);
    return NULL;
}

Mix_Chunk* AudioVault::CheckChunk(std::string p_sPath) {
    for (auto t_Entry : m_vChunks)
            if (t_Entry.m_sPath == p_sPath)
                return *(t_Entry.m_pData);
    return NULL;
}

bool AudioVault::FreeUnused() {
    bool t_bFreedSomething = false;

    for (auto t_Entry = m_vMusics.begin(); t_Entry < m_vMusics.end(); ++t_Entry) {
        if ( t_Entry->m_pData.unique() ) {
            if (t_Entry->m_ulExpiring == 0)
                t_Entry->m_ulExpiring = SDL_GetTicks();
            if (SDL_GetTicks()-t_Entry->m_ulExpiring >= m_ulExpirationTime) {
                Mix_FreeMusic( *t_Entry->m_pData );
                m_vMusics.erase(t_Entry);
                t_bFreedSomething = true;
            }
        }
    }

    for (auto t_Entry = m_vChunks.begin(); t_Entry < m_vChunks.end(); ++t_Entry) {
        if ( t_Entry->m_pData.unique() ) {
            if (t_Entry->m_ulExpiring == 0)
                t_Entry->m_ulExpiring = SDL_GetTicks();
            if (SDL_GetTicks()-t_Entry->m_ulExpiring >= m_ulExpirationTime) {
                Mix_FreeChunk( *t_Entry->m_pData );
                m_vChunks.erase(t_Entry);
                t_bFreedSomething = true;
            }
        }
    }

    return t_bFreedSomething;
}

void AudioVault::Purge() {
    for (auto t_Entry : m_vMusics)
        Mix_FreeMusic(*t_Entry.m_pData);
    for (auto t_Entry : m_vChunks)
        Mix_FreeChunk(*t_Entry.m_pData);
    m_vMusics.clear();
    m_vChunks.clear();
}

void AudioVault::SetExpirationTime(unsigned long p_ulExpirationTime) {
    m_ulExpirationTime = p_ulExpirationTime;
}
