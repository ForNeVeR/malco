param (
    $SourceRoot = "$PSScriptRoot/../source"
)

$fileNames = Get-ChildItem $SourceRoot -Recurse -Include *.cpp, *.h

clang-format -i $fileNames