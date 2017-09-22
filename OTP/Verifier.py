#!/usr/bin/env python3

class OTPFuncs:
	def Alice(x):
		return x + 1
	def Bob(x):
		return sum(map(ord, list(str(x))))
	def Charlie(x):
		return x*x + 4*x + 1

def main():
	import socket
	print('Starting server...')
	server_socket = socket.socket(socket.AF_INET,\
									socket.SOCK_STREAM) # Initialize Server Socket
	host_name = socket.gethostname() # LocalHost
	server_port = 13242 # Port Number
	server_socket.bind((host_name, server_port)) # Start Server Socket
	server_socket.listen(1) # How many connections is it listening for

if __name_- == '__main__':
	main()
