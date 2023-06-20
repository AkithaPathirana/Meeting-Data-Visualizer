import socket


serverAddressPort1=("127.0.0.1",20001)
serverAddressPort2=("127.0.0.1",20002)
serverAddressPort3=("127.0.0.1",20003)
bufferSize=1024

#Create a UDP socket at the client side

UDPClientSocket=socket.socket(family=socket.AF_INET,type=socket.SOCK_DGRAM)

#Send to server using created UDP socket

UDPClientSocket.sendto("Hi".encode(),serverAddressPort1)
UDPClientSocket.sendto("Co223".encode(),serverAddressPort2)
UDPClientSocket.sendto("bye".encode(),serverAddressPort3)

msgFromServer=UDPClientSocket.recvfrom(bufferSize)
msg="Message from Server {}".format(msgFromServer)
print(msg)

msgFromServer=UDPClientSocket.recvfrom(bufferSize)
msg="Message from Server {}".format(msgFromServer)
print(msg)

msgFromServer=UDPClientSocket.recvfrom(bufferSize)
msg="Message from Server {}".format(msgFromServer)
print(msg)
