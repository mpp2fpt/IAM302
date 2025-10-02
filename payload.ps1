# payload.ps1
Set-Content -Path "C:\Users\Public\Documents\secret_plan.txt" -Value "My secret data..."
Rename-Item -Path "C:\Users\Public\Documents\secret_plan.txt" -NewName "secret_plan.txt.locked"
Set-Content -Path "C:\Users\Public\Documents\RANSOM_NOTE.txt" -Value "Your files are locked!"