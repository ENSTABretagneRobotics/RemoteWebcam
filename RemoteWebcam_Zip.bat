cd /d "%~dp0"
7z a -y "..\RemoteWebcam_Zip.zip" "..\RemoteWebcam" "..\OSUtils" "..\Extensions" -xr!"Debug*" -xr!"Release*" -xr!"*.ncb" -xr!"*.sdf" -xr!"*.wmv" -xr!"*.mp4" -xr!"*.flv" -xr!"..\OSUtils\Debug" -xr!"..\OSUtils\Release" -xr!"..\OSUtils\OSUtils.ncb" -xr!"..\OSUtils\OSUtils.sdf" -xr!"*.git*"
pause
exit
