# me - this DAT
#
# dat - the DAT that received the data
# rowIndex - is the row number the data was placed into
# message - an ascii representation of the data
#           Unprintable characters and unicode characters will
#           not be preserved. Use the 'byteData' parameter to get
#           the raw bytes that were sent.
# byteData - a byte array of the data received
# peer - a Peer object describing the originating message
#   peer.close()    #close the connection
#   peer.owner  #the operator to whom the peer belongs
#   peer.address    #network address associated with the peer
#   peer.port       #network port associated with the peer
#

def onReceive(dat, rowIndex, message, byteData, peer):
	coord_table = op("coord_table")
	values = message.split(',')

	for index, value in enumerate(values):
		coord_table[(index + 1), 1] = float(value) 
		
	

	