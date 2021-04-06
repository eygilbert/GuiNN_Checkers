# GuiNN_Checkers
This is a fork of Jonathan Kreuzer's checkers program. The purpose of the fork is to add the capability for it to use any of the larger endgame databases that can be accessed using the Kingsrow egdb driver. These include the Kingsrow 10-piece WLD db, the Cake 8-piece WLD db, and the Chinook 8-piece WLD db. See http://www.edgilbert.org/Checkers/KingsRow.htm. This additional capability is only added to the DLL version of GuiNN_Checkers that runs in the CheckerBoard user interface. The user interface in the standalone EXE version does not have controls for configuring endgame databases, so its operation is unmodified.

To make debugging convenient, the project includes a barebones CheckerBoard installation. When the DLL is compiled, the file goes directly into the CheckerBoard\engines directory. Running the debugger in Visual Studio launches CheckerBoard.

You can find install files for the compiled DLLs and related binaries at the links below. Note that these DLLs are compiled to use AVX2 instructions. If your PC is older than 4 or 5 years, it may not have these and the DLL will not run.

If the install files do not download when you click on the links, it is probably because your browser will not let you download an executable file from a secure page (GitHub is a secure site -- uses https). Either copy/paste the link into the address box of another browser window, or use a different browser.

[GuiNN_Checkers Version 2.06 DLL install for 64-bit Windows (updated 4/6/2021)](http://www.edgilbert.org/EnglishCheckers/downloads/gui_nn64_2.06_setup.exe)

[GuiNN_Checkers Version 2.06 DLL install for 32-bit Windows (updated 4/6/2021)](http://www.edgilbert.org/EnglishCheckers/downloads/gui_nn32_2.06_setup.exe)

