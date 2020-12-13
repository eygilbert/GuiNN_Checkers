# GuiNN_Checkers
This is a fork of Jonathan Kreuzer's checkers program. The purpose of the fork is to add the capability for it to use any of the larger endgame databases that can be accessed using the Kingsrow egdb driver. These include the Kingsrow 10-piece WLD db, the Cake 8-piece WLD db, and the Chinook 8-piece WLD db. See http://www.edgilbert.org/Checkers/KingsRow.htm. This additional capability is only added to the DLL version of GuiNN_Checkers that runs in the CheckerBoard user interface. The user interface in the standalone EXE version does not have controls for configuring endgame databases, so its operation is unmodified.

To make debugging convenient, the project includes a barebones CheckerBoard installation. When the DLL is compiled, the file goes directly into the CheckerBoard\engines directory. Running the debugger in Visual Studio launches CheckerBoard.

See http://www.github.com/jonkr2/GuiNN_Checkers
