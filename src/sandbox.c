/*
 * The MIT License (MIT)
 *
 * Copyright © 2016 Franklin "Snaipe" Mathieu <http://snai.pe/>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "addr.h"
#include "exe.h"
#include "sandbox.h"

static int bxfi_main(void)
{
    struct bxfi_map local_ctx;
    if (bxfi_init_sandbox_ctx(&local_ctx) < 0)
        abort();

    struct bxfi_addr addr = {
        .soname = (char *)(local_ctx.ctx + 1),
        .addr   = local_ctx.ctx->fn,
    };
    bxf_fn *fn = bxfi_denormalize_fnaddr(&addr);

    if (!fn)
        abort();

    if (bxfi_term_sandbox_ctx(&local_ctx) < 0)
        abort();

    return fn();
}

BXFI_INITIALIZER(patch_main)
static void patch_main(void)
{
    if (!bxfi_check_sandbox_ctx())
        return;

    if (bxfi_exe_patch_main((bxfi_exe_fn *) bxfi_main) < 0)
        abort();
}