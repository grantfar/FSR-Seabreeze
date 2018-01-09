# FSR-Seabreeze
#set up seabreeze
sudo cp -R Downloads/seabreeze-3.0.11/SeaBreeze/include/ /usr/local/include/
sudo mv /usr/local/include/include/ /usr/local/include/seabreeze

#Setting up -libseabreeze
In the seabreeze-3.0.11/ directory:
	sudo cp SeaBreeze/lib/libseabreeze.so /usr/lib
	sudo ldconfig -n -v /usr/lib
