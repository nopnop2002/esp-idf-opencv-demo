#/bin/bash
set -x
convert -resize 64x64! opencv_logo_icon.png image.png

# Conver to rgb565 format
ffmpeg -vcodec png -i image.png -vcodec rawvideo -f rawvideo -pix_fmt rgb565 image.raw

cp image.raw ../raw_images
