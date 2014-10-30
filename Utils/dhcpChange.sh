
echo -e "\n"
while getopts "hc" opt; do
	case "$opt" in
	h)	echo -e "\tSetting 'wlan0' interface to HOST config..."
		sudo ifdown wlan0
		sudo cp /etc/network/interfaces.HOST /etc/network/interfaces
		sudo cp /etc/dhcp/dhcpd.conf.HOST /etc/dhcp/dhcpd.conf
		sudo cp /etc/sysctl.conf.HOST /etc/sysctl.conf
		sudo service isc-dhcp-server start
		sudo ifup wlan0
	break
	;;
	c)	echo -e "\tSetting 'wlan0' to interface to CLIENT config..."
		sudo ifdown wlan0
		sudo cp /etc/network/interfaces.CLIENT /etc/network/interfaces
		sudo cp /etc/dhcp/dhcpd.conf.CLIENT /etc/dhcp/dhcpd.conf
		sudo cp /etc/sysctl.conf.CLIENT /etc/sysctl.conf
		sudo service isc-dhcp-server stop
		sudo ifup wlan0
	break
	;;
	*)
	esac
done
if [ $# -eq 0  ]; then
	echo -e "\tMissing arguments..."
	echo -e "\t-c\t- Client mode"
	echo -e "\t-h\t- Host mode"
fi

echo -e "\n\n"


