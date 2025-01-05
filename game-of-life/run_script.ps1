# Define the path to the executable and the subfolder
$executable = ".\game_of_life.exe" # Path to your executable
$subfolder = ".\input"      # Path to the subfolder containing the files

# Check if the executable exists
if (!(Test-Path $executable)) {
    Write-Host "Executable not found: $executable" -ForegroundColor Red
    exit 1
}

# Check if the subfolder exists
if (!(Test-Path $subfolder)) {
    Write-Host "Subfolder not found: $subfolder" -ForegroundColor Red
    exit 1
}

# Iterate through all files in the subfolder
Get-ChildItem -Path $subfolder -File -Filter '*.gol' | ForEach-Object {
    $filePath = $_.FullName
    Write-Host "Launching $executable with file: $($_.BaseName)$($_.Extension)" -ForegroundColor Green

    # Launch the executable with arguments
    & $executable -l $filePath -g 250 -m
}
