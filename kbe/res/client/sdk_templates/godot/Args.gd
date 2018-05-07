var ip = "127.0.0.1"
var port = 20013

#Persistent plug-in information, For example: The protocol
# imported from the server can be persistent, if the 
# protocol version does not change on the next login.
#Can be loaded from the local cache to improve login speed.
var persistentDataPath = ""

#Whether to enable automatic synchronization of player
# information to the server. Information includes
# location and direction
#Non highly real-time games do not need to enable this option
var syncPlayer = true

#Whether to use the alias mechanism
#This parameter must be consistent with the parameters
# of kbengine_defs.xml::cellapp/aliasEntityID
var useAliasEntityID = true

#Whether to trigger the set_* event of the property when
# the Entity is initialized (callPropertysSetMethods)
var callPropertySetMethodsOnInit = true

#Start KBEngine client app on another thread
#Note: when debugging, always use single threading.
#Currently, godot does not have support for debugging/
# errors on multiple threads
var isMultiThreads = false

#Only enabled in multithreaded mode
#Thread main loop processing frequency
var threadUpdateHZ = 100

#Frequency to send server heartbeat tick
var serverHeartbeatTick = 15