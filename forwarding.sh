sudo sudo systemctl restart isc-dhcp-server.service
sudo iptables -t nat -A POSTROUTING -j MASQUERADE -s 192.168.0.1/24
sudo iptables -I FORWARD 1 -i eno2 -j ACCEPT
sudo iptables -I FORWARD 1 -o eno2 -m state --state RELATED,ESTABLISHED -j ACCEPT
sudo sysctl -w net.ipv4.ip_forward=1