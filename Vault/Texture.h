/////////////////////////////////////////////////////////////////////////
//
// Copyright (c) Dejaime Antônio de Oliveira Neto
//     Created on 20140317 ymd
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

#ifndef TEXTURE_H
#define TEXTURE_H

#include <vector>
#include <memory>
#include <string>

#include <SDL.h>
#include <SDL_image.h>

#include <ID.h>

typedef struct VAULT_TEXTURE_ENTRY {
    unsigned int m_uID;
    unsigned long m_ulExpiring;
    std::string m_sPath;
    std::shared_ptr<SDL_Texture*> m_pTexture;

    static unsigned int s_uNextID;

    VAULT_TEXTURE_ENTRY (const int p_id):
        m_uID(p_id), m_pTexture(NULL) {}
    VAULT_TEXTURE_ENTRY (const std::string p_sPath):
        m_uID(s_uNextID), m_sPath(p_sPath), m_pTexture(NULL){++s_uNextID;}
} TEntry;


class VaultTextures {
protected:

public:
    std::shared_ptr<SDL_Texture*> GetTexture (unsigned int p_uID);
    std::shared_ptr<SDL_Texture*> GetTexture (std::string p_sPath);

    SDL_Texture* CheckTexture (unsigned int p_uID);
    SDL_Texture* CheckTexture (std::string p_sPath);

    SDL_Renderer* GetRenderer () const { return m_pRenderer; }

    //bool Register (unsigned int p_uID, std::string p_sPath, SDL_Texture* p_Texture);

    bool FreeUnused ();

    void SetExpireTime (unsigned long p_ulExpireTime);

    VaultTextures(SDL_Renderer *p_Renderer = NULL, unsigned long p_ulExpireTime = 0);
    virtual ~VaultTextures();


protected:
    std::vector<TEntry> m_vTextures;
    SDL_Renderer *m_pRenderer;
    unsigned long m_ulExpireTime;

    void FreeTexture (TEntry *t_Entry) {
        SDL_DestroyTexture( *(t_Entry->m_pTexture) );
        t_Entry->m_pTexture.reset();
        t_Entry->m_uID = INVALID_VAULT_ID;
    }

public:

    //Function to abstract the SDL_image surface to texture procedure.
    //Used internally, but public in case needed outside.
    //Useful If the texture will be rendered to and you don't want
    //  to use a shared texture for that (what wouldn't be wise).
    SDL_Texture* LoadTexture (const char* p_pcPath) {
        //Load the texture
        SDL_Surface* t_pSurface = IMG_Load(p_pcPath);
        if (t_pSurface == NULL) return NULL;

        SDL_Texture* t_pRetTexture = SDL_CreateTextureFromSurface(m_pRenderer, t_pSurface);

        SDL_FreeSurface(t_pSurface);

        return t_pRetTexture;
    }

    static inline SDL_Texture* LoadTexture (SDL_Renderer *p_pRenderer, const char* p_pcPath) {
        //Load the texture
        SDL_Surface* t_pSurface = IMG_Load(p_pcPath);
        if (t_pSurface == NULL) return NULL;

        SDL_Texture* t_pRetTexture = SDL_CreateTextureFromSurface(p_pRenderer, t_pSurface);

        SDL_FreeSurface(t_pSurface);

        return t_pRetTexture;
    }

};

#endif // TEXTURE_H
