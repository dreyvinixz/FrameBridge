# Third-party notices

## OptiScaler MFG source

`MfgUnlock.cpp` and `MfgUnlock.h` adapt the RTX 40 Ada implementation from
[`y4my4my4m/OptiScaler_DLSSNR_Multipass_MFG`](https://github.com/y4my4my4m/OptiScaler_DLSSNR_Multipass_MFG),
tag `v4` (`7b7220bb`), under GPL-3.0. Their reviewed signature and kernel work
was compared with [`evairx/OptiScaler-MFG`](https://github.com/evairx/OptiScaler-MFG),
release `dev_8`, commit `e69d6f87072e381a77f65b5d4852b7167b5d10d5`.

The Ada patch is experimental, disabled by default, and does not distribute a
modified NVIDIA DLL.

The Ada interpolation kernel compatibility work includes material derived
from [`mavismmg/MFGAdaUnlock-RenoDx`](https://github.com/mavismmg/MFGAdaUnlock-RenoDx),
copyright (c) 2026 ImDreamt, under the MIT License.

MIT License

Copyright (c) 2026 ImDreamt

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

## SM75/SM86 MFG loader

`AmpereMfgLoader.cpp`, `AmpereMfgLoader.h`, `AmpereMfgStatusText.h`, and
`DlssgRuntimeGeneration.h` are adapted from
[`ShyVortex/OptiScaler-NR-PreSR-MFG`](https://github.com/ShyVortex/OptiScaler-NR-PreSR-MFG),
GPL-3.0, at commit `0e1e7e541a2e1a1f81cb562e1075fd89bc6c3213`.

The loader can use the separately maintained
[`sdli1995/dlssg_for_sm86`](https://github.com/sdli1995/dlssg_for_sm86)
release `0.4.1` (commit `44f40064aed100ff1104a555e4d0bc3122086d64`) when a user
places the compatible files under `OptiScaler/dlssg_sm86/`. FrameBridge does
not ship these binaries. The runtime and extracted NVIDIA assets retain their
upstream NVIDIA and third-party license terms; see that project's
`LICENSE.md` and `THIRD_PARTY_NOTICES.txt` before redistributing them.

The external MFG path is experimental and off by default. It is enabled only
for RTX 20/30 in this integration. GTX 16 and GTX 10 are not supported by this
path.
