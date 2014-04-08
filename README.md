SDL_vault
=========
An asset loading helper library for SDL
=======================================

X11 Licensed code.

SDL_vault is a library that helps loading/freeing Texture and Audio assets from SDL.

It prevents multiple copies of the same asset and frees them automatically once they are no longer in use
or after a defined expiration time (in case you want to define one), where the asset will need to be
unused for some time before being freed.

It is based on std::shared_ptr, so you won't need to manually free the resources in order to reduce
the usage count. It also allows us to get direct pointers to assets, that doesn't increase the usage
count, so it won't prevent the asset from being deleted (not recommended, but possible).

WIP, by a newbie, feel free to help me!

  http://www.buildandgun.com
