# Netsurf 3DS

Work in progress homebrew 3DS port of the lightweight [NetSurf web browser](https://www.netsurf-browser.org/).

# Installing 

The legacy release downloads on the GitHub Releases tab were created before the
TLS fixes landed in this fork and are unreliable for HTTPS. For a stable build
with mbedTLS 2.x and the current resource bundle you should create your own
build (or let GitHub Actions do it for you) and copy the output to the SD card
manually.

## Quick checklist for a working HTTPS setup

- ✅ Use a recent build that links against `3ds-mbedtls` (the default when you
  build from this repository). Old binaries without the updated TLS stack fail
  on many HTTPS sites.
- ✅ Copy the runtime resources to **`SD:/share/netsurf/`** directly. Do **not**
  add an extra `resources/` directory level.
- ✅ Place the `NetSurf.3dsx` binary in **`SD:/3ds/`** so it shows up in the
  Homebrew Launcher.
- ✅ Provide a current certificate bundle as
  **`SD:/share/netsurf/ca-bundle.crt`** (PEM format, >300 KiB, extension must be
  `.crt`).
- ✅ Include the extra files in the same `netsurf` folder: `mime.types`,
  `Messages`, and the directories `icons/`, `pointers/`, and `fonts/`.
- ✅ Enable file extensions in Windows Explorer before copying to the SD card to
  avoid `ca-bundle.crt.txt` mistakes.
- ✅ Format the SD card as **FAT32**. The 3DS bootloader cannot read exFAT.
- ✅ Prefer the **FullHeap** build on a New 3DS – it unlocks the extra RAM
  available on that hardware revision.

## Build and download via GitHub Actions (recommended)

1. Fork this repository on GitHub and enable GitHub Actions for your fork.
2. Trigger the *NetSurf 3DS build* workflow manually (or push a commit). The
   workflow sets up devkitARM, builds the FullHeap configuration, and packages
   the runtime resources including the latest `ca-bundle.crt` and `mime.types`.
3. Wait for the workflow run to finish, then download the two artifacts:
   - `netsurf-3dsx.zip` – contains `NetSurf.3dsx`.
   - `resources.zip` – contains `ca-bundle.crt`, `mime.types`, `Messages`,
     and the `icons/`, `pointers/`, and `fonts/` directories.

If you prefer to build locally, follow the steps in [Building](#building) and
run `make bundle` to produce the same archives in the `bundle/` directory.
Either way, you only need to extract the ZIP files and copy the contents to the
correct SD paths described below.

## Prepare the SD card

1. Extract `netsurf-3dsx.zip` and copy `NetSurf.3dsx` to **`SD:/3ds/`**.
2. Extract `resources.zip` and copy everything into **`SD:/share/netsurf/`**.
   The folder should contain the files and directories listed in the checklist
   above – no additional nesting.
3. Verify that `ca-bundle.crt` opens in a text editor and starts with
   `-----BEGIN CERTIFICATE-----`.

## Smoke test

1. Launch NetSurf via the Homebrew Launcher on your New 3DS.
2. Navigate to `http://example.com` to confirm plain HTTP works.
3. Navigate to `https://example.com` to confirm HTTPS loads without a
   certificate error. A blank screen or an "SSL CA path" message almost always
   means `ca-bundle.crt` is missing, incorrectly named, or corrupted.

## Troubleshooting tips

- "Fetching page error" only on HTTPS: re-check the CA bundle location, size,
  and extension; ensure `mime.types` is next to it.
- "ssl ca path? access rights?": confirm the bundle is PEM-formatted and try a
  different `ca-bundle.crt` if the file is suspiciously small (<250 KiB).
- Immediate blank screen at boot: the resources folder is missing or located in
  the wrong directory.

## Why not use the default web browser?

Nintendo's 3DS browser is based on an old version of Apple's WebKit, and only supports SSL up to 1.1, which many websites are already dropping. Even among websites that it *can* load, it often hopelessly mangles them. It also doesn't support downloading anything but JPEG images to the SD card, which is unhelpful for downloading homebrew.

This port, while a work in progress, already supports SSL 1.2, and has the potential to be a much better option than the default browser as development continues.

## Why port NetSurf?

The 3DS (especially the original version) is a very slow, low-memory system (only 64MB is provided to most apps, something more like 80MB to HIMEM apps), so many modern web engines would require an obscene amount of modification to even start running on the handheld. 

This is where NetSurf comes in. It is a lightweight, super fast web browser that already can run on systems with memory requirements as low as 32MB. It has a minimal amount of dependencies, and even has a built-in "framebuffer" backend that doesn't require any external toolkits, and it has fairly good support for modern HTML and CSS standards given the constraints.

## What it can do:
- browse websites with up to SSL 1.2
- browse the SD card and open local text, html, and image files (go to file:///)
- html and css support

## What it might be able to do soon&trade;:

The NetSurf codebase contains some features that don't work yet on 3DS, but might work soon with further porting efforts.
- Font support (FreeType)
- super basic JavaScript support (via internal "duktape" library)
- SVG and WebM support (requires external libraries to be ported first)
- Webpage zoom (+/-)

Here are some things not in the code base, but are planned to be added:
- Downloading of files to the SD card (perhaps with a "save as" dialog)
- Additional 3DS-specific button controls

## Known issues
- SSL implementation is partially broken (i.e. you can visit SSL websites, but you can't view the SSL certificate information)
- some things (e.g. larger images) distort while scrolling.
- Text is often cut off or wraps incorrectly
- JavaScript support is disabled in the build process
- Support for several media types is disabled. (SVG, WebP)

## What it will probably never do:
NetSurf is a long way off from a Google Chrome replacement, and (given the processing and memory constraints) will never be close. Here are some things that people might ask about that are probably impossible:
- Playing HTML5/WebGL, Java, or Flash games
- Playing YouTube videos or streaming movies (even though NetSurf has experimental video support in the code).
- Loading and running complex Web 2.0 applications like Google Docs, VS Code, Discord, etc.
- super accurate website rendering on par with desktop and mobile phone browsers.

# Building 


Building only works in Linux at the moment.

To build, ensure you have the DevKitARM toolchain installed with libctru, citro2d, citro3d, etc. You will also need the following libraries:

- 3ds-sdl
- 3ds-mbedtls
- 3ds-curl
- 3ds-libpng
- 3ds-libjpeg-turbo
- 3ds-libiconv
- 3ds-freetype (for future font support)

Ensure your environment has the `DEVKITARM` and `DEVKITPRO` variables set, and that DevKitARM's `bin` directory is in your `PATH`. You should then be able to build NetSurf with the `make` command. Using multiple jobs (`-j <cpu cores>`) is reccomended to speed up compilation.

## Special thanks

Thanks to the [NetSurf developers](https://www.netsurf-browser.org/) for making such an awesome lightweight and portable browser! Also thanks to [DevKitPro](https://devkitpro.org/) for making the 3DS homebrew toolchain and library ports.
