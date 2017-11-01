cd /d "%~dp0"
7z a -y "..\RemoteWebcam_Zip.zip" "..\RemoteWebcam" "..\OSUtils" "..\Extensions" -xr!"Debug*" -xr!"Release*" -xr!".vs" -xr!"*.ncb" -xr!"*.sdf" -xr!"*.opensdf" -xr!"*.VC.db" -xr!"*.wmv" -xr!"*.mp4" -xr!"*.mpg" -xr!"*.mpeg" -xr!"*.mov" -xr!"*.webm" -xr!"*.flv" -xr!"..\OSUtils\Debug" -xr!"..\OSUtils\Release" -xr!"..\OSUtils\.vs" -xr!"..\OSUtils\OSUtils.ncb" -xr!"..\OSUtils\OSUtils.sdf" -xr!"..\OSUtils\OSUtils.opensdf" -xr!"..\OSUtils\OSUtils.VC.db" -xr!"*.git*"
pause
exit
