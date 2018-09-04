# RPSGame
Simple rock-paper-scissors game which uses GOG SDK to provide P2P communication between players.

# Build
*IMPORTANT:* Visual Studio 14 2015 is a must!
*IMPORTANT2:* VS140COMNTOOLS must be included in system environment variables. Actually, they are included there
by default after VS intallation. So, this item is just to make sure that everything is specified.

To build x64 version of RPSGame just run *build_win_x64.bat* file.
To build x86 version of RPSGame just run *build_win_x86.bat* file.

Both these files:
1) Create separate path for binaries(e.g. _bin\win\x64_) and project files (e.g. _build\win\x64_);
2) Run cmake with _install_ target.

