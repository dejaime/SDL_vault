/////////////////////////////////////////////////////////////////////////
//
// Copyright (c) Dejaime Antônio de Oliveira Neto
//     Created on 20140309 ymd
//
// For more:
//      http://www.buildandgun.com
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

#ifndef TYPES_H_INCLUDED
#define TYPES_H_INCLUDED

#include <memory>

#include <vector>
#include <inttypes.h>
#include <string>

#include <v2.h>

#define INVALID_UNIQUE_ID GET_INVALID_VAULT_ID()
static inline unsigned int GET_INVALID_VAULT_ID () {
    return (unsigned int)-1;
}

#endif // TYPES_H_INCLUDED
