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

#ifndef TEXTUREVAULT_H
#define TEXTUREVAULT_H

#include "SDL_image.h"

#include <types.h>

#include <VaultEntry.h>

typedef vault_entry<SDL_Texture> TEntry;

class TextureVault {
protected:
    std::vector<TEntry> m_vTextures;
    SDL_Renderer *m_pRenderer;
    unsigned long m_ulExpireTime;
public:
    std::shared_ptr<SDL_Texture*> GetTexture (std::string p_sPath);

    SDL_Texture* CheckTexture (std::string p_sPath);

    SDL_Renderer* GetRenderer () const { return m_pRenderer; }

    bool FreeUnused ();
    void Purge ();

    void SetExpirationTime (unsigned long p_ulExpireTime);

    TextureVault(SDL_Renderer *p_Renderer = NULL, unsigned long p_ulExpireTime = 0);
    virtual ~TextureVault();

protected:
    void FreeTexture (TEntry *p_Entry) {
        SDL_DestroyTexture( *(p_Entry->m_pData) );
        p_Entry->m_pData.reset();
    }

    //Function to abstract the SDL_image surface to texture procedure.
    //Used internally, but public in case needed outside.
    //Useful If the texture will be rendered to and you don't want
    //  to use a shared texture for that (what wouldn't be wise).
    SDL_Texture* LoadTexture (const char* p_pcPath);

public:
    static inline SDL_Texture* LoadTexture (SDL_Renderer *p_pRenderer, const char* p_pcPath) {
        //Load the texture
        SDL_Surface* t_pSurface = IMG_Load(p_pcPath);
        if (t_pSurface == NULL) return NULL;

        SDL_Texture* t_pRetTexture = SDL_CreateTextureFromSurface(p_pRenderer, t_pSurface);

        SDL_FreeSurface(t_pSurface);

        return t_pRetTexture;
    }

private:
    //protecting copy ctor and assign
    TextureVault(const TextureVault&):
        m_vTextures(), m_pRenderer(NULL), m_ulExpireTime(0){}
    TextureVault& operator= (const TextureVault&) {return *this;}

};

#endif // TEXTUREVAULT_H
