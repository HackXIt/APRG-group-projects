# PowerShell script to run all test variations for the convex hull executable with a timeout
# Save this script as run_tests.ps1

Param (
    [Parameter(Mandatory = $true)]
    [string]$BinaryPath,
    [Parameter(Mandatory = $false)]
    [string]$LogFile
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
if (-not $LogFile) {
    $LogFile = Join-Path -Path $BinaryDirectory -ChildPath "test.log"
} else {
    $LogFile = Resolve-Path -Path $LogFile | Select-Object -ExpandProperty Path
}

# Delete the existing log file if it exists
if (Test-Path $LogFile) {
    Remove-Item $LogFile
}

# Define the maximum execution time per test (in seconds)
$MaxExecutionTime = 300  # 5 minutes

# Configuration: Define dictionaries for test cases and algorithms with descriptive names
$TestCases = @{
    0 = "random"
    1 = "line"
    2 = "circle"
    3 = "square"
}

$Algorithms = @{
    0 = "quick-hull"
    1 = "jarvis-march"
}

# Configuration: Adjust the number of data points per test case
# Each test case can have different data point values
$DataPoints = @{
    0 = 10, 50, 10000, 100000, 1000000, 10000000, 100000000                 # Data points for test case 0 ("random")
    1 = 10, 50, 10000, 50000, 100000, 1000000             # Data points for test case 1 ("line")
    2 = 10, 50, 10000, 50000, 100000     # Data points for test case 2 ("circle")
    3 = 10, 50, 10000, 50000, 100000, 1000000         # Data points for test case 3 ("square")
}

# Function to escape arguments if needed
function Escape-Argument($arg) {
    if ($arg -match '[\s"]') {
        return '"' + ($arg -replace '"', '""') + '"'
    } else {
        return $arg
    }
}

# Loop over all combinations of test cases, algorithms, and data points
foreach ($TestCase in $TestCases.Keys) {
    if ($DataPoints.ContainsKey($TestCase)) {
        foreach ($NumPoints in $DataPoints[$TestCase]) {
            foreach ($Algorithm in $Algorithms.Keys) {

                # Get descriptive names for the test case and algorithm
                $TestCaseName = $TestCases[$TestCase]
                $AlgorithmName = $Algorithms[$Algorithm]

                # Prepare the arguments for each test run
                $ArgumentList = @("-a", "$Algorithm", "-t", "$TestCase", "-n", "$NumPoints")
                $EscapedArguments = $ArgumentList | ForEach-Object { Escape-Argument $_ }
                $ArgumentsString = $EscapedArguments -join ' '

                # Write header to the log file
                Add-Content -Path $LogFile -Value "========================================================================="
                Add-Content -Path $LogFile -Value "Running test case: $TestCase ($TestCaseName), algorithm: $Algorithm ($AlgorithmName), data points: $NumPoints"
                Add-Content -Path $LogFile -Value "========================================================================="

                # Start the process for each test configuration
                Write-Host "Running test case: $TestCase ($TestCaseName), algorithm: $Algorithm ($AlgorithmName), data points: $NumPoints"

                $StartInfo = New-Object System.Diagnostics.ProcessStartInfo
                $StartInfo.FileName = $BinaryPath
                $StartInfo.Arguments = $ArgumentsString
                $StartInfo.WorkingDirectory = $BinaryDirectory
                $StartInfo.UseShellExecute = $false
                $StartInfo.RedirectStandardOutput = $true
                $StartInfo.RedirectStandardError = $true

                $Process = New-Object System.Diagnostics.Process
                $Process.StartInfo = $StartInfo

                # Start the timer
                $Timer = [System.Diagnostics.Stopwatch]::StartNew()
                $Process.Start() | Out-Null

                # Track process execution time and forcefully terminate if it exceeds the limit
                $Exited = $Process.WaitForExit($MaxExecutionTime * 1000)

                if (-not $Exited) {
                    # Process exceeded the time limit, kill it and log a warning
                    $Process.Kill()
                    Write-Warning "Test case $TestCase ($TestCaseName), algorithm $Algorithm ($AlgorithmName), data points $NumPoints exceeded the time limit and was terminated."
                    Add-Content -Path $LogFile -Value "WARNING: Test case $TestCase ($TestCaseName), algorithm $Algorithm ($AlgorithmName), data points $NumPoints exceeded the time limit and was terminated."
                } else {
                    # Read output
                    $StdOut = $Process.StandardOutput.ReadToEnd()
                    $StdErr = $Process.StandardError.ReadToEnd()
                    $Process.WaitForExit()

                    # Append output to log file
                    Add-Content -Path $LogFile -Value $StdOut
                    Add-Content -Path $LogFile -Value $StdErr
                }

                # Log the execution time
                Add-Content -Path $LogFile -Value "Execution time: $($Timer.Elapsed.TotalSeconds) seconds."

                # Add a blank line to the log file
                Add-Content -Path $LogFile -Value ""
            }
        }
    } else {
        Write-Warning "No data points defined for test case $TestCase ($TestCaseName). Skipping..."
    }
}

Write-Host "All tests executed. Check the log file at: $LogFile"
