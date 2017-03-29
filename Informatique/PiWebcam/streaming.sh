mjpg_streamer -i "/usr/local/lib/input_uvc.so -d /dev/video0 -y -f 24 -r 640x480" \
	-o "/usr/local/lib/output_http.so -n -w /usr/local/www -p 8001"
