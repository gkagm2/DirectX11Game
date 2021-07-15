xcopy /s /y /exclude:Exclude_list.txt ".\Project\Script\*.h" ".\External\Include\Script"

xcopy /s /y /exclude:Exclude_list_ScriptCopy.txt ".\x64\Debug\*.exe" ".\OutputFile\bin_debug"
xcopy /s /y /exclude:Exclude_list.txt ".\x64\Debug\*.txt" ".\OutputFile\bin_debug"
