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
    SDL_Renderer *m_pRenderer = NULL;
    unsigned long m_ulExpirationTime = 0;
    SDL_TimerID m_TimerID = 0;
public:
    ////////////////////////////////////////////////
    /// Searches for the path in the loaded textures and return a strong reference if found.
    /// Tries to load the file if it can't find it.
    /// @param p_sPath The path to the image file.
    /// @return Shared Pointer to the texture, if found. If it is not found and it fails to load, return a NULL pointer.
    /// @see CheckTexture()
    ////////////////////////////////////////////////
    std::shared_ptr<SDL_Texture*> GetTexture (std::string p_sPath);

    ////////////////////////////////////////////////
    /// Pushes a texture into the vault.
    /// @param p_pTexture A pointer to a user-loaded texture. Must be valid.
    /// @param p_sPath The path to the texture file. Doesn't really need to be a path, any string is valid in this case.
    /// @return Shared Pointer to the passed chunk.
    /// @warning This doesn't check if p_sPath is unique. Using a p_sPath that already exists is an error.
    /// @see CheckTexture()
    /// @see GetTexture()
    ////////////////////////////////////////////////
    std::shared_ptr<SDL_Texture*> PushNewTexture (SDL_Texture* p_pTexture, std::string p_sPath);

    ////////////////////////////////////////////////
    /// Searches for the path in the loaded textures and return a direct pointer if found.
    /// Returns NULL if it is not found.
    /// @param p_sPath The path to the image file.
    /// @return Direct pointer to the texture, if found; NULL otherwise.
    /// @see GetTexture()
    ////////////////////////////////////////////////
    SDL_Texture* CheckTexture (std::string p_sPath);

    ////////////////////////////////////////////////
    /// Returns the renderer passed when creating the vault object.
    /// @return The SDL_Renderer related to this specific vault.
    ////////////////////////////////////////////////
    SDL_Renderer* GetRenderer () const { return m_pRenderer; }

    ////////////////////////////////////////////////
    /// Returns the renderer passed when creating the vault object.
    /// @param p_pRenderer The new SDL_Renderer.
    /// @note Best practices - Set the renderer in the ctor or right after the object was created and leave it as that. If you need a new renderer, it is wise to create a new Texture Vault for it rather than changing the renderer of another one.
    /// @warning Changing the renderer at runtime won't update the contained textures, they will still belong to the old renderer.
    ////////////////////////////////////////////////
    void SetRenderer (SDL_Renderer* p_pRenderer) { m_pRenderer = p_pRenderer; }

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
    inline void SetExpirationTime (unsigned long p_ulExpirationTime) {
        m_ulExpirationTime = p_ulExpirationTime;
    }

    ////////////////////////////////////////////////
    /// Constructor for the AudioVault.
    /// @param p_Renderer The renderer of the vault.
    /// @param p_ulExpirationTime The time an asset needs to stay unused before being freed.
    /// @param p_ulAutoFreeTime Automatic FreeUnused() call period. 0 will disable it.
    /// @see StopAutoFree()
    /// @see FreeUnused()
    ////////////////////////////////////////////////
    TextureVault(SDL_Renderer *p_Renderer = NULL, unsigned long p_ulExpirationTime = 0, unsigned long p_ulAutoFreeTime = 0);
    virtual ~TextureVault();

protected:
    //static void TimedFreeUnused (TextureVault* p_TexVault);
    static unsigned int TimedFreeUnused(unsigned int, void* p_TexVault);

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

    ////////////////////////////////////////////////
    /// Helper function that abstracts the Surface-to-Texture conversion.
    /// Using this function doesn't add the texture to any Vault objects.
    /// @param p_pRenderer The renderer used to load the texture.
    /// @param p_pcPath The path for the file
    /// @see GetTexture()
    ///////////////////////////////////////////
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
        m_vTextures(), m_pRenderer(NULL), m_ulExpirationTime(0){}
    TextureVault& operator= (const TextureVault&) {return *this;}

};

#endif // TEXTUREVAULT_H
