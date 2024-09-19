
# Windows build logs

![](windows-exit-code.svg)

```
system        : Windows
version       : 10.0.26120
platform      : Windows-11-10.0.26120-SP0
processor     : AMD64 Family 23 Model 17 Stepping 0, AuthenticAMD
min frequency : 0.0 MHz
max frequency : 3500.0 MHz
cores         : 4
```

build ran for `123.38 s` and terminated with exit code `0`

---

```bash
> .shci/windows/prerequisites-output.txt (
    choco install -y mingw cmake
    mingw-get install gcc mingw32-make gdb
)
```

```bash
Chocolatey v2.2.2
3 validations performed. 2 success(es), 1 warning(s), and 0 error(s).

Validation Warnings:
 - A pending system reboot request has been detected, however, this is
   being ignored due to the current Chocolatey configuration.  If you
   want to halt when this occurs, then either set the global feature
   using:
     choco feature enable --name="exitOnRebootDetected"
   or pass the option --exit-when-reboot-detected.

Chocolatey detected you are not running from an elevated command shell
 (cmd/powershell).

 You may experience errors - many functions/packages
 require admin rights. Only advanced users should run choco w/out an
 elevated shell. When you open the command shell, you should ensure
 that you do so with "Run as Administrator" selected. If you are
 attempting to use Chocolatey in a non-administrator setting, you
 must select a different location other than the default install
 location. See
 https://docs.chocolatey.org/en-us/choco/setup#non-administrative-install
 for details.

For the question below, you have 20 seconds to make a selection.

 Do you want to continue?([Y]es/[N]o): 
Timeout or your choice of '' is not a valid selection.
Installing the following packages:
mingw;cmake
By installing, you accept licenses for the packages.

Progress: Downloading mingw 13.2.0... 10%
Progress: Downloading mingw 13.2.0... 31%
Progress: Downloading mingw 13.2.0... 52%
Progress: Downloading mingw 13.2.0... 73%
Progress: Downloading mingw 13.2.0... 94%
Progress: Downloading mingw 13.2.0... 100%

mingw v13.2.0 [Approved]
mingw package files install completed. Performing other installation steps.
Downloading mingw 64 bit
  from 'https://github.com/niXman/mingw-builds-binaries/releases/download/13.2.0-rt_v11-rev0/x86_64-13.2.0-release-posix-seh-ucrt-rt_v11-rev0.7z'

Progress: 0% - Saving 13.46 KB of 69.57 MB
Progress: 0% - Saving 26.91 KB of 69.57 MB
Progress: 0% - Saving 83.69 KB of 69.57 MB
Progress: 0% - Saving 243.69 KB of 69.57 MB
Progress: 0% - Saving 397.5 KB of 69.57 MB
Progress: 0% - Saving 535.31 KB of 69.57 MB
Progress: 0% - Saving 695.31 KB of 69.57 MB
Progress: 1% - Saving 849.13 KB of 69.57 MB
Progress: 1% - Saving 1009.13 KB of 69.57 MB
Progress: 1% - Saving 1.13 MB of 69.57 MB
Progress: 1% - Saving 1.28 MB of 69.57 MB
Progress: 2% - Saving 1.44 MB of 69.57 MB
Progress: 2% - Saving 1.6 MB of 69.57 MB
Progress: 2% - Saving 1.75 MB of 69.57 MB
Progress: 2% - Saving 1.9 MB of 69.57 MB
Progress: 2% - Saving 2.06 MB of 69.57 MB
Progress: 3% - Saving 2.22 MB of 69.57 MB
Progress: 3% - Saving 2.37 MB of 69.57 MB
Progress: 3% - Saving 2.52 MB of 69.57 MB
Progress: 3% - Saving 2.68 MB of 69.57 MB
Progress: 4% - Saving 2.84 MB of 69.57 MB
Progress: 4% - Saving 2.99 MB of 69.57 MB
Progress: 4% - Saving 3.14 MB of 69.57 MB
Progress: 4% - Saving 3.3 MB of 69.57 MB
Progress: 4% - Saving 3.46 MB of 69.57 MB
Progress: 5% - Saving 3.61 MB of 69.57 MB
Progress: 5% - Saving 3.76 MB of 69.57 MB
Progress: 5% - Saving 3.91 MB of 69.57 MB
Progress: 5% - Saving 4.07 MB of 69.57 MB
Progress: 6% - Saving 4.22 MB of 69.57 MB
Progress: 6% - Saving 4.37 MB of 69.57 MB
Progress: 6% - Saving 4.53 MB of 69.57 MB
Progress: 6% - Saving 4.69 MB of 69.57 MB
Progress: 6% - Saving 4.84 MB of 69.57 MB
Progress: 7% - Saving 4.99 MB of 69.57 MB
Progress: 7% - Saving 5.15 MB of 69.57 MB
Progress: 7% - Saving 5.3 MB of 69.57 MB
Progress: 7% - Saving 5.46 MB of 69.57 MB
Progress: 8% - Saving 5.61 MB of 69.57 MB
Progress: 8% - Saving 5.77 MB of 69.57 MB
Progress: 8% - Saving 5.92 MB of 69.57 MB
Progress: 8% - Saving 6.07 MB of 69.57 MB
Progress: 8% - Saving 6.22 MB of 69.57 MB
Progress: 9% - Saving 6.37 MB of 69.57 MB
Progress: 9% - Saving 6.52 MB of 69.57 MB
Progress: 9% - Saving 6.68 MB of 69.57 MB
Progress: 9% - Saving 6.83 MB of 69.57 MB
Progress: 10% - Saving 6.99 MB of 69.57 MB
Progress: 10% - Saving 7.14 MB of 69.57 MB
Progress: 10% - Saving 7.3 MB of 69.57 MB
Progress: 10% - Saving 7.45 MB of 69.57 MB
Progress: 10% - Saving 7.61 MB of 69.57 MB
Progress: 11% - Saving 7.76 MB of 69.57 MB
Progress: 11% - Saving 7.91 MB of 69.57 MB
Progress: 11% - Saving 8.07 MB of 69.57 MB
Progress: 11% - Saving 8.23 MB of 69.57 MB
Progress: 12% - Saving 8.38 MB of 69.57 MB
Progress: 12% - Saving 8.54 MB of 69.57 MB
Progress: 12% - Saving 8.68 MB of 69.57 MB
Progress: 12% - Saving 8.84 MB of 69.57 MB
Progress: 12% - Saving 9 MB of 69.57 MB
Progress: 13% - Saving 9.15 MB of 69.57 MB
Progress: 13% - Saving 9.3 MB of 69.57 MB
Progress: 13% - Saving 9.46 MB of 69.57 MB
Progress: 13% - Saving 9.62 MB of 69.57 MB
Progress: 14% - Saving 9.77 MB of 69.57 MB
Progress: 14% - Saving 9.92 MB of 69.57 MB
Progress: 14% - Saving 10.08 MB of 69.57 MB
Progress: 14% - Saving 10.22 MB of 69.57 MB
Progress: 14% - Saving 10.37 MB of 69.57 MB
Progress: 15% - Saving 10.52 MB of 69.57 MB
Progress: 15% - Saving 10.68 MB of 69.57 MB
Progress: 15% - Saving 10.84 MB of 69.57 MB
Progress: 15% - Saving 10.99 MB of 69.57 MB
Progress: 16% - Saving 11.15 MB of 69.57 MB
Progress: 16% - Saving 11.31 MB of 69.57 MB
Progress: 16% - Saving 11.46 MB of 69.57 MB
Progress: 16% - Saving 11.62 MB of 69.57 MB
Progress: 16% - Saving 11.77 MB of 69.57 MB
Progress: 17% - Saving 11.93 MB of 69.57 MB
Progress: 17% - Saving 12.09 MB of 69.57 MB
Progress: 17% - Saving 12.24 MB of 69.57 MB
Progress: 17% - Saving 12.4 MB of 69.57 MB
Progress: 18% - Saving 12.56 MB of 69.57 MB
Progress: 18% - Saving 12.71 MB of 69.57 MB
Progress: 18% - Saving 12.87 MB of 69.57 MB
Progress: 18% - Saving 13.02 MB of 69.57 MB
Progress: 18% - Saving 13.18 MB of 69.57 MB
Progress: 19% - Saving 13.34 MB of 69.57 MB
Progress: 19% - Saving 13.49 MB of 69.57 MB
Progress: 19% - Saving 13.65 MB of 69.57 MB
Progress: 19% - Saving 13.81 MB of 69.57 MB
Progress: 20% - Saving 13.96 MB of 69.57 MB
Progress: 20% - Saving 14.12 MB of 69.57 MB
Progress: 20% - Saving 14.27 MB of 69.57 MB
Progress: 20% - Saving 14.43 MB of 69.57 MB
Progress: 20% - Saving 14.59 MB of 69.57 MB
Progress: 21% - Saving 14.74 MB of 69.57 MB
Progress: 21% - Saving 14.9 MB of 69.57 MB
Progress: 21% - Saving 15.06 MB of 69.57 MB
Progress: 21% - Saving 15.21 MB of 69.57 MB
Progress: 22% - Saving 15.37 MB of 69.57 MB
Progress: 22% - Saving 15.52 MB of 69.57 MB
Progress: 22% - Saving 15.68 MB of 69.57 MB
Progress: 22% - Saving 15.84 MB of 69.57 MB
Progress: 22% - Saving 15.99 MB of 69.57 MB
Progress: 23% - Saving 16.15 MB of 69.57 MB
Progress: 23% - Saving 16.31 MB of 69.57 MB
Progress: 23% - Saving 16.46 MB of 69.57 MB
Progress: 23% - Saving 16.62 MB of 69.57 MB
Progress: 24% - Saving 16.77 MB of 69.57 MB
Progress: 24% - Saving 16.93 MB of 69.57 MB
Progress: 24% - Saving 17.09 MB of 69.57 MB
Progress: 24% - Saving 17.24 MB of 69.57 MB
Progress: 25% - Saving 17.4 MB of 69.57 MB
Progress: 25% - Saving 17.56 MB of 69.57 MB
Progress: 25% - Saving 17.71 MB of 69.57 MB
Progress: 25% - Saving 17.87 MB of 69.57 MB
Progress: 25% - Saving 18.02 MB of 69.57 MB
Progress: 26% - Saving 18.18 MB of 69.57 MB
Progress: 26% - Saving 18.34 MB of 69.57 MB
Progress: 26% - Saving 18.49 MB of 69.57 MB
Progress: 26% - Saving 18.65 MB of 69.57 MB
Progress: 27% - Saving 18.81 MB of 69.57 MB
Progress: 27% - Saving 18.96 MB of 69.57 MB
Progress: 27% - Saving 19.12 MB of 69.57 MB
Progress: 27% - Saving 19.27 MB of 69.57 MB
Progress: 27% - Saving 19.43 MB of 69.57 MB
Progress: 28% - Saving 19.59 MB of 69.57 MB
Progress: 28% - Saving 19.74 MB of 69.57 MB
Progress: 28% - Saving 19.9 MB of 69.57 MB
Progress: 28% - Saving 20.05 MB of 69.57 MB
Progress: 29% - Saving 20.2 MB of 69.57 MB
Progress: 29% - Saving 20.34 MB of 69.57 MB
Progress: 29% - Saving 20.5 MB of 69.57 MB
Progress: 29% - Saving 20.65 MB of 69.57 MB
Progress: 29% - Saving 20.81 MB of 69.57 MB
Progress: 30% - Saving 20.97 MB of 69.57 MB
Progress: 30% - Saving 21.12 MB of 69.57 MB
Progress: 30% - Saving 21.28 MB of 69.57 MB
Progress: 30% - Saving 21.44 MB of 69.57 MB
Progress: 31% - Saving 21.59 MB of 69.57 MB
Progress: 31% - Saving 21.75 MB of 69.57 MB
Progress: 31% - Saving 21.9 MB of 69.57 MB
Progress: 31% - Saving 22.06 MB of 69.57 MB
Progress: 31% - Saving 22.22 MB of 69.57 MB
Progress: 32% - Saving 22.37 MB of 69.57 MB
Progress: 32% - Saving 22.53 MB of 69.57 MB
Progress: 32% - Saving 22.69 MB of 69.57 MB
Progress: 32% - Saving 22.84 MB of 69.57 MB
Progress: 33% - Saving 23 MB of 69.57 MB
Progress: 33% - Saving 23.15 MB of 69.57 MB
Progress: 33% - Saving 23.31 MB of 69.57 MB
Progress: 33% - Saving 23.47 MB of 69.57 MB
Progress: 33% - Saving 23.62 MB of 69.57 MB
Progress: 34% - Saving 23.78 MB of 69.57 MB
Progress: 34% - Saving 23.94 MB of 69.57 MB
Progress: 34% - Saving 24.09 MB of 69.57 MB
Progress: 34% - Saving 24.25 MB of 69.57 MB
Progress: 35% - Saving 24.4 MB of 69.57 MB
Progress: 35% - Saving 24.56 MB of 69.57 MB
Progress: 35% - Saving 24.72 MB of 69.57 MB
Progress: 35% - Saving 24.87 MB of 69.57 MB
Progress: 35% - Saving 25.03 MB of 69.57 MB
Progress: 36% - Saving 25.19 MB of 69.57 MB
Progress: 36% - Saving 25.34 MB of 69.57 MB
Progress: 36% - Saving 25.5 MB of 69.57 MB
Progress: 36% - Saving 25.65 MB of 69.57 MB
Progress: 37% - Saving 25.81 MB of 69.57 MB
Progress: 37% - Saving 25.97 MB of 69.57 MB
Progress: 37% - Saving 26.12 MB of 69.57 MB
Progress: 37% - Saving 26.28 MB of 69.57 MB
Progress: 38% - Saving 26.44 MB of 69.57 MB
Progress: 38% - Saving 26.59 MB of 69.57 MB
Progress: 38% - Saving 26.75 MB of 69.57 MB
Progress: 38% - Saving 26.9 MB of 69.57 MB
Progress: 38% - Saving 27.06 MB of 69.57 MB
Progress: 39% - Saving 27.22 MB of 69.57 MB
Progress: 39% - Saving 27.37 MB of 69.57 MB
Progress: 39% - Saving 27.53 MB of 69.57 MB
Progress: 39% - Saving 27.69 MB of 69.57 MB
Progress: 40% - Saving 27.84 MB of 69.57 MB
Progress: 40% - Saving 28 MB of 69.57 MB
Progress: 40% - Saving 28.15 MB of 69.57 MB
Progress: 40% - Saving 28.31 MB of 69.57 MB
Progress: 40% - Saving 28.47 MB of 69.57 MB
Progress: 41% - Saving 28.62 MB of 69.57 MB
Progress: 41% - Saving 28.78 MB of 69.57 MB
Progress: 41% - Saving 28.94 MB of 69.57 MB
Progress: 41% - Saving 29.09 MB of 69.57 MB
Progress: 42% - Saving 29.25 MB of 69.57 MB
Progress: 42% - Saving 29.4 MB of 69.57 MB
Progress: 42% - Saving 29.56 MB of 69.57 MB
Progress: 42% - Saving 29.72 MB of 69.57 MB
Progress: 42% - Saving 29.87 MB of 69.57 MB
Progress: 43% - Saving 30.02 MB of 69.57 MB
Progress: 43% - Saving 30.17 MB of 69.57 MB
Progress: 43% - Saving 30.31 MB of 69.57 MB
Progress: 43% - Saving 30.47 MB of 69.57 MB
Progress: 44% - Saving 30.62 MB of 69.57 MB
Progress: 44% - Saving 30.78 MB of 69.57 MB
Progress: 44% - Saving 30.94 MB of 69.57 MB
Progress: 44% - Saving 31.09 MB of 69.57 MB
Progress: 44% - Saving 31.25 MB of 69.57 MB
Progress: 45% - Saving 31.41 MB of 69.57 MB
Progress: 45% - Saving 31.56 MB of 69.57 MB
Progress: 45% - Saving 31.72 MB of 69.57 MB
Progress: 45% - Saving 31.87 MB of 69.57 MB
Progress: 46% - Saving 32.03 MB of 69.57 MB
Progress: 46% - Saving 32.19 MB of 69.57 MB
Progress: 46% - Saving 32.34 MB of 69.57 MB
Progress: 46% - Saving 32.5 MB of 69.57 MB
Progress: 46% - Saving 32.66 MB of 69.57 MB
Progress: 47% - Saving 32.81 MB of 69.57 MB
Progress: 47% - Saving 32.97 MB of 69.57 MB
Progress: 47% - Saving 33.12 MB of 69.57 MB
Progress: 47% - Saving 33.28 MB of 69.57 MB
Progress: 48% - Saving 33.44 MB of 69.57 MB
Progress: 48% - Saving 33.59 MB of 69.57 MB
Progress: 48% - Saving 33.75 MB of 69.57 MB
Progress: 48% - Saving 33.91 MB of 69.57 MB
Progress: 48% - Saving 34.06 MB of 69.57 MB
Progress: 49% - Saving 34.22 MB of 69.57 MB
Progress: 49% - Saving 34.37 MB of 69.57 MB
Progress: 49% - Saving 34.53 MB of 69.57 MB
Progress: 49% - Saving 34.69 MB of 69.57 MB
Progress: 50% - Saving 34.84 MB of 69.57 MB
Progress: 50% - Saving 35 MB of 69.57 MB
Progress: 50% - Saving 35.16 MB of 69.57 MB
Progress: 50% - Saving 35.31 MB of 69.57 MB
Progress: 50% - Saving 35.47 MB of 69.57 MB
Progress: 51% - Saving 35.62 MB of 69.57 MB
Progress: 51% - Saving 35.78 MB of 69.57 MB
Progress: 51% - Saving 35.94 MB of 69.57 MB
Progress: 51% - Saving 36.09 MB of 69.57 MB
Progress: 52% - Saving 36.25 MB of 69.57 MB
Progress: 52% - Saving 36.41 MB of 69.57 MB
Progress: 52% - Saving 36.56 MB of 69.57 MB
Progress: 52% - Saving 36.72 MB of 69.57 MB
Progress: 53% - Saving 36.87 MB of 69.57 MB
Progress: 53% - Saving 37.03 MB of 69.57 MB
Progress: 53% - Saving 37.19 MB of 69.57 MB
Progress: 53% - Saving 37.34 MB of 69.57 MB
Progress: 53% - Saving 37.5 MB of 69.57 MB
Progress: 54% - Saving 37.66 MB of 69.57 MB
Progress: 54% - Saving 37.81 MB of 69.57 MB
Progress: 54% - Saving 37.97 MB of 69.57 MB
Progress: 54% - Saving 38.12 MB of 69.57 MB
Progress: 55% - Saving 38.28 MB of 69.57 MB
Progress: 55% - Saving 38.44 MB of 69.57 MB
Progress: 55% - Saving 38.59 MB of 69.57 MB
Progress: 55% - Saving 38.75 MB of 69.57 MB
Progress: 55% - Saving 38.91 MB of 69.57 MB
Progress: 56% - Saving 39.06 MB of 69.57 MB
Progress: 56% - Saving 39.22 MB of 69.57 MB
Progress: 56% - Saving 39.37 MB of 69.57 MB
Progress: 56% - Saving 39.53 MB of 69.57 MB
Progress: 57% - Saving 39.69 MB of 69.57 MB
Progress: 57% - Saving 39.84 MB of 69.57 MB
Progress: 57% - Saving 40 MB of 69.57 MB
Progress: 57% - Saving 40.14 MB of 69.57 MB
Progress: 57% - Saving 40.3 MB of 69.57 MB
Progress: 58% - Saving 40.44 MB of 69.57 MB
Progress: 58% - Saving 40.6 MB of 69.57 MB
Progress: 58% - Saving 40.75 MB of 69.57 MB
Progress: 58% - Saving 40.91 MB of 69.57 MB
Progress: 59% - Saving 41.07 MB of 69.57 MB
Progress: 59% - Saving 41.22 MB of 69.57 MB
Progress: 59% - Saving 41.38 MB of 69.57 MB
Progress: 59% - Saving 41.54 MB of 69.57 MB
Progress: 59% - Saving 41.69 MB of 69.57 MB
Progress: 60% - Saving 41.85 MB of 69.57 MB
Progress: 60% - Saving 42 MB of 69.57 MB
Progress: 60% - Saving 42.16 MB of 69.57 MB
Progress: 60% - Saving 42.32 MB of 69.57 MB
Progress: 61% - Saving 42.47 MB of 69.57 MB
Progress: 61% - Saving 42.63 MB of 69.57 MB
Progress: 61% - Saving 42.79 MB of 69.57 MB
Progress: 61% - Saving 42.94 MB of 69.57 MB
Progress: 61% - Saving 43.1 MB of 69.57 MB
Progress: 62% - Saving 43.25 MB of 69.57 MB
Progress: 62% - Saving 43.41 MB of 69.57 MB
Progress: 62% - Saving 43.57 MB of 69.57 MB
Progress: 62% - Saving 43.72 MB of 69.57 MB
Progress: 63% - Saving 43.88 MB of 69.57 MB
Progress: 63% - Saving 44.04 MB of 69.57 MB
Progress: 63% - Saving 44.19 MB of 69.57 MB
Progress: 63% - Saving 44.35 MB of 69.57 MB
Progress: 63% - Saving 44.5 MB of 69.57 MB
Progress: 64% - Saving 44.66 MB of 69.57 MB
Progress: 64% - Saving 44.82 MB of 69.57 MB
Progress: 64% - Saving 44.97 MB of 69.57 MB
Progress: 64% - Saving 45.13 MB of 69.57 MB
Progress: 65% - Saving 45.29 MB of 69.57 MB
Progress: 65% - Saving 45.44 MB of 69.57 MB
Progress: 65% - Saving 45.6 MB of 69.57 MB
Progress: 65% - Saving 45.75 MB of 69.57 MB
Progress: 65% - Saving 45.91 MB of 69.57 MB
Progress: 66% - Saving 46.07 MB of 69.57 MB
Progress: 66% - Saving 46.22 MB of 69.57 MB
Progress: 66% - Saving 46.38 MB of 69.57 MB
Progress: 66% - Saving 46.54 MB of 69.57 MB
Progress: 67% - Saving 46.69 MB of 69.57 MB
Progress: 67% - Saving 46.85 MB of 69.57 MB
Progress: 67% - Saving 47 MB of 69.57 MB
Progress: 67% - Saving 47.16 MB of 69.57 MB
Progress: 68% - Saving 47.32 MB of 69.57 MB
Progress: 68% - Saving 47.47 MB of 69.57 MB
Progress: 68% - Saving 47.63 MB of 69.57 MB
Progress: 68% - Saving 47.79 MB of 69.57 MB
Progress: 68% - Saving 47.94 MB of 69.57 MB
Progress: 69% - Saving 48.1 MB of 69.57 MB
Progress: 69% - Saving 48.25 MB of 69.57 MB
Progress: 69% - Saving 48.41 MB of 69.57 MB
Progress: 69% - Saving 48.57 MB of 69.57 MB
Progress: 70% - Saving 48.72 MB of 69.57 MB
Progress: 70% - Saving 48.88 MB of 69.57 MB
Progress: 70% - Saving 49.04 MB of 69.57 MB
Progress: 70% - Saving 49.19 MB of 69.57 MB
Progress: 70% - Saving 49.35 MB of 69.57 MB
Progress: 71% - Saving 49.5 MB of 69.57 MB
Progress: 71% - Saving 49.66 MB of 69.57 MB
Progress: 71% - Saving 49.82 MB of 69.57 MB
Progress: 71% - Saving 49.97 MB of 69.57 MB
Progress: 72% - Saving 50.12 MB of 69.57 MB
Progress: 72% - Saving 50.28 MB of 69.57 MB
Progress: 72% - Saving 50.42 MB of 69.57 MB
Progress: 72% - Saving 50.56 MB of 69.57 MB
Progress: 72% - Saving 50.68 MB of 69.57 MB
Progress: 73% - Saving 50.84 MB of 69.57 MB
Progress: 73% - Saving 51 MB of 69.57 MB
Progress: 73% - Saving 51.15 MB of 69.57 MB
Progress: 73% - Saving 51.31 MB of 69.57 MB
Progress: 73% - Saving 51.47 MB of 69.57 MB
Progress: 74% - Saving 51.61 MB of 69.57 MB
Progress: 74% - Saving 51.76 MB of 69.57 MB
Progress: 74% - Saving 51.9 MB of 69.57 MB
Progress: 74% - Saving 52.05 MB of 69.57 MB
Progress: 75% - Saving 52.21 MB of 69.57 MB
Progress: 75% - Saving 52.36 MB of 69.57 MB
Progress: 75% - Saving 52.51 MB of 69.57 MB
Progress: 75% - Saving 52.67 MB of 69.57 MB
Progress: 75% - Saving 52.82 MB of 69.57 MB
Progress: 76% - Saving 52.98 MB of 69.57 MB
Progress: 76% - Saving 53.14 MB of 69.57 MB
Progress: 76% - Saving 53.29 MB of 69.57 MB
Progress: 76% - Saving 53.45 MB of 69.57 MB
Progress: 77% - Saving 53.61 MB of 69.57 MB
Progress: 77% - Saving 53.76 MB of 69.57 MB
Progress: 77% - Saving 53.92 MB of 69.57 MB
Progress: 77% - Saving 54.07 MB of 69.57 MB
Progress: 77% - Saving 54.23 MB of 69.57 MB
Progress: 78% - Saving 54.39 MB of 69.57 MB
Progress: 78% - Saving 54.54 MB of 69.57 MB
Progress: 78% - Saving 54.7 MB of 69.57 MB
Progress: 78% - Saving 54.86 MB of 69.57 MB
Progress: 79% - Saving 55.01 MB of 69.57 MB
Progress: 79% - Saving 55.17 MB of 69.57 MB
Progress: 79% - Saving 55.32 MB of 69.57 MB
Progress: 79% - Saving 55.48 MB of 69.57 MB
Progress: 79% - Saving 55.64 MB of 69.57 MB
Progress: 80% - Saving 55.79 MB of 69.57 MB
Progress: 80% - Saving 55.95 MB of 69.57 MB
Progress: 80% - Saving 56.11 MB of 69.57 MB
Progress: 80% - Saving 56.26 MB of 69.57 MB
Progress: 81% - Saving 56.42 MB of 69.57 MB
Progress: 81% - Saving 56.57 MB of 69.57 MB
Progress: 81% - Saving 56.73 MB of 69.57 MB
Progress: 81% - Saving 56.89 MB of 69.57 MB
Progress: 81% - Saving 57.04 MB of 69.57 MB
Progress: 82% - Saving 57.2 MB of 69.57 MB
Progress: 82% - Saving 57.36 MB of 69.57 MB
Progress: 82% - Saving 57.51 MB of 69.57 MB
Progress: 82% - Saving 57.67 MB of 69.57 MB
Progress: 83% - Saving 57.82 MB of 69.57 MB
Progress: 83% - Saving 57.98 MB of 69.57 MB
Progress: 83% - Saving 58.14 MB of 69.57 MB
Progress: 83% - Saving 58.29 MB of 69.57 MB
Progress: 84% - Saving 58.45 MB of 69.57 MB
Progress: 84% - Saving 58.61 MB of 69.57 MB
Progress: 84% - Saving 58.76 MB of 69.57 MB
Progress: 84% - Saving 58.92 MB of 69.57 MB
Progress: 84% - Saving 59.07 MB of 69.57 MB
Progress: 85% - Saving 59.23 MB of 69.57 MB
Progress: 85% - Saving 59.39 MB of 69.57 MB
Progress: 85% - Saving 59.54 MB of 69.57 MB
Progress: 85% - Saving 59.7 MB of 69.57 MB
Progress: 86% - Saving 59.86 MB of 69.57 MB
Progress: 86% - Saving 60 MB of 69.57 MB
Progress: 86% - Saving 60.15 MB of 69.57 MB
Progress: 86% - Saving 60.31 MB of 69.57 MB
Progress: 86% - Saving 60.46 MB of 69.57 MB
Progress: 87% - Saving 60.62 MB of 69.57 MB
Progress: 87% - Saving 60.76 MB of 69.57 MB
Progress: 87% - Saving 60.89 MB of 69.57 MB
Progress: 87% - Saving 61.04 MB of 69.57 MB
Progress: 87% - Saving 61.2 MB of 69.57 MB
Progress: 88% - Saving 61.36 MB of 69.57 MB
Progress: 88% - Saving 61.51 MB of 69.57 MB
Progress: 88% - Saving 61.67 MB of 69.57 MB
Progress: 88% - Saving 61.82 MB of 69.57 MB
Progress: 89% - Saving 61.98 MB of 69.57 MB
Progress: 89% - Saving 62.14 MB of 69.57 MB
Progress: 89% - Saving 62.29 MB of 69.57 MB
Progress: 89% - Saving 62.45 MB of 69.57 MB
Progress: 89% - Saving 62.61 MB of 69.57 MB
Progress: 90% - Saving 62.76 MB of 69.57 MB
Progress: 90% - Saving 62.92 MB of 69.57 MB
Progress: 90% - Saving 63.07 MB of 69.57 MB
Progress: 90% - Saving 63.23 MB of 69.57 MB
Progress: 91% - Saving 63.39 MB of 69.57 MB
Progress: 91% - Saving 63.54 MB of 69.57 MB
Progress: 91% - Saving 63.7 MB of 69.57 MB
Progress: 91% - Saving 63.86 MB of 69.57 MB
Progress: 92% - Saving 64.01 MB of 69.57 MB
Progress: 92% - Saving 64.17 MB of 69.57 MB
Progress: 92% - Saving 64.32 MB of 69.57 MB
Progress: 92% - Saving 64.48 MB of 69.57 MB
Progress: 92% - Saving 64.64 MB of 69.57 MB
Progress: 93% - Saving 64.79 MB of 69.57 MB
Progress: 93% - Saving 64.95 MB of 69.57 MB
Progress: 93% - Saving 65.11 MB of 69.57 MB
Progress: 93% - Saving 65.26 MB of 69.57 MB
Progress: 94% - Saving 65.42 MB of 69.57 MB
Progress: 94% - Saving 65.57 MB of 69.57 MB
Progress: 94% - Saving 65.73 MB of 69.57 MB
Progress: 94% - Saving 65.89 MB of 69.57 MB
Progress: 94% - Saving 66.04 MB of 69.57 MB
Progress: 95% - Saving 66.2 MB of 69.57 MB
Progress: 95% - Saving 66.36 MB of 69.57 MB
Progress: 95% - Saving 66.51 MB of 69.57 MB
Progress: 95% - Saving 66.67 MB of 69.57 MB
Progress: 96% - Saving 66.82 MB of 69.57 MB
Progress: 96% - Saving 66.98 MB of 69.57 MB
Progress: 96% - Saving 67.14 MB of 69.57 MB
Progress: 96% - Saving 67.29 MB of 69.57 MB
Progress: 96% - Saving 67.45 MB of 69.57 MB
Progress: 97% - Saving 67.61 MB of 69.57 MB
Progress: 97% - Saving 67.76 MB of 69.57 MB
Progress: 97% - Saving 67.92 MB of 69.57 MB
Progress: 97% - Saving 68.07 MB of 69.57 MB
Progress: 98% - Saving 68.23 MB of 69.57 MB
Progress: 98% - Saving 68.39 MB of 69.57 MB
Progress: 98% - Saving 68.54 MB of 69.57 MB
Progress: 98% - Saving 68.7 MB of 69.57 MB
Progress: 98% - Saving 68.86 MB of 69.57 MB
Progress: 99% - Saving 69.01 MB of 69.57 MB
Progress: 99% - Saving 69.17 MB of 69.57 MB
Progress: 99% - Saving 69.32 MB of 69.57 MB
Progress: 99% - Saving 69.48 MB of 69.57 MB
Progress: 100% - Completed download of C:\Users\sino3\AppData\Local\Temp\chocolatey\mingw\13.2.0\x86_64-13.2.0-release-posix-seh-ucrt-rt_v11-rev0.7z (69.57 MB).
Download of x86_64-13.2.0-release-posix-seh-ucrt-rt_v11-rev0.7z (69.57 MB) completed.
Hashes match.
Extracting C:\Users\sino3\AppData\Local\Temp\chocolatey\mingw\13.2.0\x86_64-13.2.0-release-posix-seh-ucrt-rt_v11-rev0.7z to C:\Users\sino3\AppData\Local\UniGetUI\Chocolatey\lib\mingw\tools\install...
C:\Users\sino3\AppData\Local\UniGetUI\Chocolatey\lib\mingw\tools\install
Testing path: C:\ProgramData\mingw64\mingw32\bin
Testing path: C:\ProgramData\mingw64\mingw64\bin
PATH environment variable does not have C:\ProgramData\mingw64\mingw64\bin in it. Adding...
Environment Vars (like PATH) have changed. Close/reopen your shell to
 see the changes (or in powershell/cmd.exe just type `refreshenv`).
 The install of mingw was successful.
  Software installed to 'C:\Users\sino3\AppData\Local\UniGetUI\Chocolatey\lib\mingw\tools\install'
[NuGet] One or more unresolved package dependency constraints detected in the Chocolatey lib folder. All dependency constraints must be resolved to add or update packages. If these packages are being updated this message may be ignored, if not the following error(s) may be blocking the current package operation: 'nodejs 22.6.0 constraint: nodejs.install (= 22.6.0)'
Unable to resolve dependency: Unable to resolve dependencies. 'nodejs.install 22.5.1' is not compatible with 'nodejs 22.6.0 constraint: nodejs.install (= 22.6.0)'.

Chocolatey installed 1/2 packages. 1 packages failed.
 See the log for details (C:\Users\sino3\AppData\Local\UniGetUI\Chocolatey\logs\chocolatey.log).

Failures
 - cmake - Unable to resolve dependency: Unable to resolve dependencies. 'nodejs.install 22.5.1' is not compatible with 'nodejs 22.6.0 constraint: nodejs.install (= 22.6.0)'.

```

---
    
```bash
> .shci/windows/build-output.txt (
    mkdir windows
    cd windows
    cmake -DSH_VULKAN_BUILD_EXAMPLES=ON ..
    cmake --build .
)

```

```bash
-- Selecting Windows SDK version 10.0.26100.0 to target Windows 10.0.26120.
-- shvulkan message: found Vulkan
-- shvulkan message: Vulkan_INCLUDE_DIR: C:/_lib/vulkan-sdk/1.3.283.0/Include
-- shvulkan message: Vulkan_LIBRARY:     C:/_lib/vulkan-sdk/1.3.283.0/Lib/vulkan-1.lib
-- Found Doxygen: C:/Program Files/doxygen/bin/doxygen.exe (found version "1.12.0") found components: doxygen missing components: dot
-- Configuring C:/_home/desktop/github/shvulkan/docs/Doxyfile from C:/_home/desktop/github/shvulkan/docs/Doxyfile.in
-- Creating custom docs command: C:/Program Files/doxygen/bin/doxygen.exe C:/_home/desktop/github/shvulkan/docs/Doxyfile
-- Including Win32 support
-- Found Doxygen: C:/Program Files/doxygen/bin/doxygen.exe (found version "1.12.0") found components: doxygen 
-- Configuring done (1.0s)
-- Generating done (1.4s)
-- Build files have been written to: C:/_home/desktop/github/shvulkan/windows
Versione di MSBuild 17.11.2+c078802d4 per .NET Framework

  glfw.vcxproj -> C:\_home\desktop\github\shvulkan\windows\examples\external\glfw\src\Debug\glfw3.lib
  shvulkan.vcxproj -> C:\_home\desktop\github\shvulkan\bin\windows\Debug\shvulkan.lib
  shvulkan-clear-color.vcxproj -> C:\_home\desktop\github\shvulkan\bin\windows\Debug\shvulkan-clear-color.exe
  shvulkan-compute-power-numbers.vcxproj -> C:\_home\desktop\github\shvulkan\bin\windows\Debug\shvulkan-compute-power-numbers.exe
  scene.c
  shvulkan-scene.vcxproj -> C:\_home\desktop\github\shvulkan\bin\windows\Debug\shvulkan-scene.exe

```

---
    
build ran for `123.38 s` and terminated with exit code `0`

---

