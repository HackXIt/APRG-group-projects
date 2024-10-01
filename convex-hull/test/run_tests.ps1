# PowerShell script to run convex hull algorithms on test files
# Save this script as run_tests.ps1

Param (
    [Parameter(Mandatory = $true)]
    [string]$BinaryPath
)

# Resolve the full path of the binary
$BinaryPath = Resolve-Path -Path $BinaryPath | Select-Object -ExpandProperty Path

# Check if the binary exists
if (-Not (Test-Path $BinaryPath)) {
    Write-Error "Error: Binary not found at path '$BinaryPath'."
    exit 1
}

# Get the directory of the binary
$BinaryDirectory = Split-Path -Path $BinaryPath -Parent

# Set the log file name
$LogFile = "test.log"

# Delete the existing log file if it exists
if (Test-Path $LogFile) {
    Remove-Item $LogFile
}

function Escape-Argument($arg) {
    if ($arg -match '[\s"]') {
        return '"' + ($arg -replace '"', '""') + '"'
    } else {
        return $arg
    }
}

# Get all test files matching the pattern TC_*.txt in the current directory
$CurrentDirectory = Get-Location
$TestFiles = Get-ChildItem -Path $CurrentDirectory -Filter "TC_*.txt"

foreach ($TestFile in $TestFiles) {
    # Resolve the full path of the test file
    $TestFileFullPath = Resolve-Path -Path $TestFile.FullName | Select-Object -ExpandProperty Path

    # Extract the test name and number of points from the filename
    # Filename format: TC_<TestName>_<DataPoints>.txt
    $FileName = $TestFile.BaseName
    $FileNameWithoutPrefix = $FileName -replace "^TC_", ""
    $Parts = $FileNameWithoutPrefix -split "_"

    $TestName = $null
    $NumPoints = $null

    if ($Parts.Count -eq 2) {
        $TestName = $Parts[0]
        $NumPoints = [int]$Parts[1]
    } elseif ($Parts.Count -eq 1) {
        $TestName = $Parts[0]
        # Handle special cases like TC_LargeDataset
        if ($TestName -eq "LargeDataset") {
            $NumPoints = 100000000
        } else {
            $NumPoints = 0
        }
    } else {
        Write-Warning "Filename format not recognized: $($TestFile.Name)"
        continue
    }

    # Loop over algorithms (0: QuickHull, 1: Jarvis March, 2: Divide & Conquer)
    foreach ($Algorithm in 0,1,2) {
        # Prepare the arguments
        # Ensure test file path is full path
        $TestFilePath = $TestFileFullPath

        # Prepare the argument list
        $ArgumentList = @("-a", "$Algorithm", "-d", "$TestFilePath")

        # Escape arguments
        $EscapedArguments = $ArgumentList | ForEach-Object { Escape-Argument $_ }
        $ArgumentsString = $EscapedArguments -join ' '

        # Write header to the log file
        Add-Content -Path $LogFile -Value "========================================================================="
        Add-Content -Path $LogFile -Value "Running test file: $($TestFile.Name) with algorithm $Algorithm"
        Add-Content -Path $LogFile -Value "========================================================================="

        # Start the process
        Write-Host "Running test file: $($TestFile.Name) with algorithm $Algorithm"

        $StartInfo = New-Object System.Diagnostics.ProcessStartInfo
        $StartInfo.FileName = $BinaryPath
        $StartInfo.Arguments = $ArgumentsString
        # Set WorkingDirectory to the current directory (where test files are)
        $StartInfo.WorkingDirectory = $CurrentDirectory.Path
        $StartInfo.UseShellExecute = $false
        $StartInfo.RedirectStandardOutput = $true
        $StartInfo.RedirectStandardError = $true

        $Process = New-Object System.Diagnostics.Process
        $Process.StartInfo = $StartInfo
        $Process.Start() | Out-Null

        # Read output
        $StdOut = $Process.StandardOutput.ReadToEnd()
        $StdErr = $Process.StandardError.ReadToEnd()
        $Process.WaitForExit()

        # Append output to log file
        Add-Content -Path $LogFile -Value $StdOut
        Add-Content -Path $LogFile -Value $StdErr

        # Add a blank line to the log file
        Add-Content -Path $LogFile -Value ""
    }
}
