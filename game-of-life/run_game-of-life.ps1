param (
    [string]$Executable = "",
    [string]$Basename = "",
    [int]$Generations = 250,
    [string]$InputDir = (Join-Path -Path $PWD -ChildPath "input"),
    [string]$ResultDir = (Join-Path -Path $PWD -ChildPath "result"),
    [string]$ReferenceDir = (Join-Path -Path $PWD -ChildPath "expected"),
    [int]$Threads = 4,
    [switch]$Verbose
)

function Compile {
	g++ -O3 -march=native -static -fopenmp -o game_of_life.exe main.cpp Timing.cpp game_of_life.cpp
	return ".\game_of_life.exe"
}

# Set logging level
function Log {
    param (
        [string]$Level,
        [string]$Message
    )
    if (($Level -eq "DEBUG" -and $Verbose) -or $Level -in @("INFO", "ERROR", "WARNING")) {
        Write-Host "[$Level] $Message"
    }
}

function Validate-Directory {
    param (
        [string]$Path
    )
    if (-not (Test-Path $Path)) {
        Log "INFO" "Creating directory: $Path"
        New-Item -ItemType Directory -Path $Path | Out-Null
    } elseif (-not (Test-Path $Path -PathType Container)) {
        throw "Path exists but is not a directory: $Path"
    }
    return $Path
}

function Compare-Files {
    param (
        [string]$File1,
        [string]$File2
    )
    $Diff = Compare-Object -ReferenceObject (Get-Content $File1) -DifferenceObject (Get-Content $File2) -PassThru
    if ($Diff) {
        return $Diff
    }
    return $null
}

function Write-Diff {
    param (
        [string]$File,
        [array]$Diff
    )
    $Diff | Out-File -FilePath $File
}

# Validate directories
$InputDir = Validate-Directory -Path $InputDir
$ResultDir = Validate-Directory -Path $ResultDir
$ReferenceDir = Validate-Directory -Path $ReferenceDir

if ($Executable -eq "") {
	$Executable = Compile
}

# Validate executable
if (-not (Test-Path $Executable)) {
    Log "ERROR" "Executable file not found: $Executable"
    exit 1
}

# Clear existing result files
Get-ChildItem -Path $ResultDir | Remove-Item -Force -Recurse

# Gather input and reference files, sorted numerically
$InputFiles = Get-ChildItem -Path $InputDir -File | Sort-Object { [int]($_.Name -replace '\D', '') }
$ReferenceFiles = Get-ChildItem -Path $ReferenceDir -File | Sort-Object { [int]($_.Name -replace '\D', '') }

# Match input and reference files
$InputReferencePairs = @()
foreach ($InputFile in $InputFiles) {
    $ReferenceFile = $InputFile.Name -replace '_in', '_out'
    if ($ReferenceFiles.Name -contains $ReferenceFile) {
        $InputReferencePairs += @([PSCustomObject]@{ Input = $InputFile.FullName; Reference = (Join-Path $ReferenceDir $ReferenceFile) })
    } else {
        Log "WARNING" "No reference file found for input file: $($InputFile.Name)"
    }
}

# Run the executable and compare outputs
for ($i = 0; $i -lt $InputReferencePairs.Count; $i++) {
    $Pair = $InputReferencePairs[$i]
    $OutputFile = Join-Path -Path $ResultDir -ChildPath ($Pair.Reference | Split-Path -Leaf)

    Log "INFO" "Running file $($i + 1) of $($InputReferencePairs.Count): $($Pair.Input)"

    $SeqArgs = @(
        $Executable,
        "-g", $Generations,
        "-l", $Pair.Input,
        "-s", $OutputFile,
        "--measure",
		"--csv",
        "--mode", "seq"
    )
    $ParArgs = @(
        $Executable,
        "-g", $Generations,
        "-l", $Pair.Input,
        "-s", $OutputFile,
        "--measure",
		"--csv",
        "--mode", "par",
        "--threads", $Threads
    )

    # Run sequential mode
    $SeqTempFile = [System.IO.Path]::GetTempFileName()
    Log "DEBUG" "Running (sequential): $SeqArgs"
    Start-Process -FilePath $SeqArgs[0] -ArgumentList $SeqArgs[1..($SeqArgs.Count - 1)] -NoNewWindow -RedirectStandardOutput $SeqTempFile -Wait
    $SeqOutput = (Get-Content $SeqTempFile) -join "" | ForEach-Object { $_.TrimEnd() }

    $Diff = Compare-Files -File1 $OutputFile -File2 $Pair.Reference
    if ($Diff) {
        $DiffFile = Join-Path -Path $ResultDir -ChildPath ($OutputFile -replace '_out\.gol', '_seq_diff.txt')
        Write-Diff -File $DiffFile -Diff $Diff
        Log "ERROR" "Sequential output does not match reference. Diff saved to: $DiffFile"
    }

	Log "INFO" "(sequential):`t$SeqOutput"

    # Run parallel mode
    $ParTempFile = [System.IO.Path]::GetTempFileName()
    Log "DEBUG" "Running (parallel): $ParArgs"
    Start-Process -FilePath $ParArgs[0] -ArgumentList $ParArgs[1..($ParArgs.Count - 1)] -NoNewWindow -RedirectStandardOutput $ParTempFile -Wait
    $ParOutput = (Get-Content $ParTempFile) -join "" | ForEach-Object { $_.TrimEnd() }

    $Diff = Compare-Files -File1 $OutputFile -File2 $Pair.Reference
    if ($Diff) {
        $DiffFile = Join-Path -Path $ResultDir -ChildPath ($OutputFile -replace '_out\.gol', '_omp_diff.txt')
        Write-Diff -File $DiffFile -Diff $Diff
        Log "ERROR" "Parallel output does not match reference. Diff saved to: $DiffFile"
    }

	LOG "INFO" "(parallel):`t$ParOutput"

    Remove-Item -Path $SeqTempFile, $ParTempFile
}
