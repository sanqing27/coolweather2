# Combine split chunks of study_all.7z
$dir = Split-Path -Parent $MyInvocation.MyCommand.Path
$parts = Get-ChildItem -LiteralPath $dir -File | Where-Object { $_.Name -match '^study_all\.7z\.part\d{4}$' } | Sort-Object Name
$outFile = Join-Path $dir 'study_restored.7z'
if (Test-Path -LiteralPath $outFile) { Remove-Item -LiteralPath $outFile -Force }
$fs = [System.IO.File]::Create($outFile)
try {
    foreach ($p in $parts) {
        $bytes = [System.IO.File]::ReadAllBytes($p.FullName)
        $fs.Write($bytes, 0, $bytes.Length)
    }
} finally {
    $fs.Close()
}
Write-Host "Combined -> $outFile ($($parts.Count) parts)"
Write-Host "Verify sha1: $((Get-FileHash -LiteralPath $outFile -Algorithm SHA1).Hash)"
