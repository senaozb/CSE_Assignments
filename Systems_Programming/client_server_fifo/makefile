target:	MybiboBox

MybiboBox: 
		gcc fileop.c server.c -o biboServer -lpthread
		gcc client.c -o biboClient -lpthread

clean:
		rm biboServer
		rm biboClient
