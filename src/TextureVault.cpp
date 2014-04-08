#include "TextureVault.h"

TextureVault::TextureVault(SDL_Renderer *p_Renderer, unsigned long p_ulExpirationTime, unsigned long p_ulAutoFreeTime)
    :m_vTextures(), m_pRenderer(p_Renderer), m_ulExpirationTime(p_ulExpirationTime) {

    if (p_ulAutoFreeTime > 0) SetAutoFree(p_ulAutoFreeTime);
}

TextureVault::~TextureVault() {
    StopAutoFree();
    for (auto t_Entry : m_vTextures)
        SDL_DestroyTexture( *(t_Entry.m_pData) );
}

std::shared_ptr<SDL_Texture*> TextureVault::GetTexture(std::string p_sPath) {
    if (!m_pRenderer) return std::shared_ptr<SDL_Texture*>();

    for (auto t_Entry : m_vTextures)
        if (t_Entry.m_sPath == p_sPath) {
            t_Entry.m_ulExpiring = 0;
            return std::shared_ptr<SDL_Texture*>(t_Entry.m_pData);
        }

    //Reaching here means the texture was not previously loaded.

    //Loads the Texture using the LoadTexture helper function.
    SDL_Texture* t_pTexture = LoadTexture(p_sPath.c_str());
    //If LoadTexture returns NULL, we couldn't load the Texture.
    //An invalid entry is then returned.
    if (t_pTexture  == NULL) return std::shared_ptr<SDL_Texture*>();
    //Otherwise, it was successfully loaded.

    //Returns the entry, with the strong reference.
    return PushNewTexture(t_pTexture, p_sPath);
}

std::shared_ptr<SDL_Texture*> TextureVault::PushNewTexture(SDL_Texture* p_pTexture, std::string p_sPath) {
    //Sets the path into our texture entry.
    TEntry t_TextureEntry(p_sPath);
    //Sets the strong reference (shared_ptr).
    t_TextureEntry.m_pData = std::make_shared<SDL_Texture*>(p_pTexture);

    //Pushes it into the vector.
    m_vTextures.push_back(t_TextureEntry);

    //Returns the entry, with the strong reference.
    return std::shared_ptr<SDL_Texture*>(t_TextureEntry.m_pData);
}

SDL_Texture* TextureVault::CheckTexture(std::string p_sPath) {
    for (auto t_Entry : m_vTextures)
        if (t_Entry.m_sPath == p_sPath)
            return *(t_Entry.m_pData);
    return NULL; //Not Found.
    //This function won't load the texture if it was not found.
}

bool TextureVault::FreeUnused() {
    bool t_bFreedSomething = false;

    for (auto t_Entry = m_vTextures.begin(); t_Entry < m_vTextures.end(); ++t_Entry) {
        if ( t_Entry->m_pData.unique() ) {
            if (t_Entry->m_ulExpiring == 0)
                t_Entry->m_ulExpiring = SDL_GetTicks();
            if (SDL_GetTicks()-t_Entry->m_ulExpiring >= m_ulExpirationTime) {
                FreeTexture( &*t_Entry );
                m_vTextures.erase(t_Entry);
                t_bFreedSomething = true;
            }
        }
    }

    return t_bFreedSomething;
}

void TextureVault::Purge() {
    for (auto t_Entry : m_vTextures)
        SDL_DestroyTexture( *(t_Entry.m_pData) );
    m_vTextures.clear();
}

SDL_Texture* TextureVault::LoadTexture (const char* p_pcPath) {
    //Load the texture
    SDL_Surface* t_pSurface = IMG_Load(p_pcPath);
    if (t_pSurface == NULL) return NULL;

    SDL_Texture* t_pRetTexture = SDL_CreateTextureFromSurface(m_pRenderer, t_pSurface);

    SDL_FreeSurface(t_pSurface);

    return t_pRetTexture;
}

unsigned int TextureVault::TimedFreeUnused(unsigned int, void* p_TexVault) {
    ((TextureVault*)p_TexVault)->FreeUnused();
    return 0;
}

void TextureVault::SetAutoFree(unsigned long p_ulTimeMS) {
    if(m_TimerID != 0) StopAutoFree();
    if (p_ulTimeMS == 0) return;
    m_TimerID = SDL_AddTimer(p_ulTimeMS, TextureVault::TimedFreeUnused, this);
}

void TextureVault::StopAutoFree() {
    if (m_TimerID != 0){
        SDL_RemoveTimer(m_TimerID);
        m_TimerID = 0;
    }
}

