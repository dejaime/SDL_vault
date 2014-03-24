#include "Texture.h"

unsigned int TEntry::s_uNextID = 0;

VaultTextures::VaultTextures(SDL_Renderer *p_Renderer, unsigned long p_ulExpireTime)
    :m_pRenderer(p_Renderer), m_ulExpireTime(p_ulExpireTime) {
    //ctor
}

VaultTextures::~VaultTextures() {
    //dtor
}

std::shared_ptr<SDL_Texture*> VaultTextures::GetTexture(unsigned int p_uID) {
    for (auto t_Entry : m_vTextures)
        if (t_Entry.m_uID == p_uID) {
            t_Entry.m_ulExpiring = 0;
            return t_Entry.m_pTexture;
        }

    //This function can't load the texture, as it doesn't have the file path.
    return std::shared_ptr<SDL_Texture*>();
}

std::shared_ptr<SDL_Texture*> VaultTextures::GetTexture(std::string p_sPath) {
    for (auto t_Entry : m_vTextures)
        if (t_Entry.m_sPath == p_sPath) {
            t_Entry.m_ulExpiring = 0;
            return t_Entry.m_pTexture;
        }

    //Reaching here means the texture was not previously loaded.

    //Loads the Texture using the LoadTexture helper function.
    SDL_Texture* t_pTexture = LoadTexture(p_sPath.c_str());
    //If LoadTexture returns NULL, we couldn't load the Texture.
    //An invalid ID entry is then returned.
    if (t_pTexture  == NULL) return std::shared_ptr<SDL_Texture*>();
    //Otherwise, it was successfully loaded.

    //Sets the path into our texture entry.
    TEntry t_TextureEntry(p_sPath);
    //Sets the strong reference (shared_ptr).
    t_TextureEntry.m_pTexture.reset(&t_pTexture);

    //Pushes it into the vector.
    m_vTextures.push_back(t_TextureEntry);

    //Returns the entry, with the strong reference.
    return t_TextureEntry.m_pTexture;
}

SDL_Texture* VaultTextures::CheckTexture(unsigned int p_uID) {
    for (auto t_Entry : m_vTextures)
        if (t_Entry.m_uID == p_uID)
            return *(t_Entry.m_pTexture);
    return NULL; //Not Found.
    //This function won't load the texture if it was not found.
}

SDL_Texture* VaultTextures::CheckTexture(std::string p_sPath) {
    for (auto t_Entry : m_vTextures)
        if (t_Entry.m_sPath == p_sPath)
            return *(t_Entry.m_pTexture);
    return NULL; //Not Found.
    //This function won't load the texture if it was not found.
}

bool VaultTextures::FreeUnused() {
    bool t_bFreedSomething = false;

    for (auto t_Entry = m_vTextures.begin(); t_Entry < m_vTextures.end(); ++t_Entry) {
        if ( t_Entry->m_pTexture.unique() ) {
            if (t_Entry->m_ulExpiring == 0)
                t_Entry->m_ulExpiring = SDL_GetTicks();
            if (SDL_GetTicks()-t_Entry->m_ulExpiring >= m_ulExpireTime) {
                FreeTexture( &*t_Entry );
                m_vTextures.erase(t_Entry);
                t_bFreedSomething = true;
            }
        }
    }

    return t_bFreedSomething;
}

void VaultTextures::SetExpireTime(unsigned long p_ulExpireTime) {
    m_ulExpireTime = p_ulExpireTime;
}
