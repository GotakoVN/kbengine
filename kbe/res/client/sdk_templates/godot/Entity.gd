#KBEngine logic layer entity base class
#All game entities should extend this class

#The current player's last sync to the server's position and orientation
#These two properties are for the engine, do not modify elsewhere
var _entityLastLocalPos = Vector3(0,0,0)
var _entityLastLocalDir = Vector3(0,0,0)

var id = 0
var className = ""
var position = Vector3(0,0,0)
var direction = Vector3(0,0,0)
var velocity = 0

var isOnGround = true

var renderObj = null setget _setRenderObj,_getRenderObj
var _renderObjWeakref = weakref(Reference.new())
func _setRenderObj(val):
	if val == null:
		_renderObjWeakref = weakref(Reference.new())
	else:
		_renderObjWeakref = weakref(val)
func _getRenderObj():
	return _renderObjWeakref.get_ref()

#var baseEntityCall = null
#var cellEntityCall = null

var inWorld = false

#For the local player, it indicates whether he is controlled by other players;
#For other entities, indicates whether this machine controls the entity
var isControlled = false

#Set to true after __init__() call
var inited = false

func onDestroy():
	pass

func isPlayer():
	return id == KBEngine.app.entity_id
	
func onRemoteMethodCall(stream):
	pass # Dynamically generated

func onUpdatePropertys(stream):
	pass # Dynamically generated

func onGetBase():
	pass # Dynamically generated

func onGetCell():
	pass # Dynamically generated

func onLoseCell():
	pass # Dynamically generated

func getBaseEntityCall():
	return null # Dynamically generated

func getCellEntityCall():
	return null # Dynamically generated

#The Kbengine entity constructor that corresponds to the
# server script.
#This constructor exists because KBE needs to create a good
# entity and populate data such as attributes to tell the
# script layer to initialize
func __init__():
	pass

func callPropertysSetMethods():
	pass # Dynamically generated

func baseCall(methodname, arguments=[]):
	if KBEngine.app.currserver == "loginapp":
		KBEngine.Dbg.ERROR_MSG(className + "::baseCall(" + methodname + "), currserver=!" + KBEngine.app.currserver)
		return
	
	if not KBEngine.EntityDef.moduledefs.has(className):
		KBEngine.Dbg.ERROR_MSG("entity::baseCall:  entity-module(" + className + ") error, can not find in EntityDef.moduledefs")
		return
	var module = KBEngine.EntityDef.moduledefs[className]

	if not module.base_methods.has(methodname):
		KBEngine.Dbg.ERROR_MSG(className + "::baseCall(" + methodname + "), method not found")
		return
	var method = module.base_methods[methodname]
	
	var methodID = method.methodUtype
	
	if len(arguments) != len(method.args):
		KBEngine.Dbg.ERROR_MSG(className + "::baseCall(" + methodname + "): args(" + str(len(arguments)) + " != " + str(len(method.args)) + ") error, wrong number of arguments!")
		return
		
	var baseEntityCall = getBaseEntityCall()
	
	baseEntityCall.newCall()
	baseEntityCall.bundle.writeUint16(0)
	baseEntityCall.bundle.writeUint16(methodID)
	
	for i in range(len(arguments)):
		if method.args[i].isSameType(arguments[i]):
			method.args[i].addToStream(baseEntityCall.bundle, arguments[i])
		else:
			var err_text = "arg" + str(i) + " ("+str(arguments[i])+") is not " + method.args[i].TYPENAME()
			KBEngine.Dbg.ERROR_MSG(className + "::baseCall(method=" + methodname + "): args type error(" + err_text + ")!")
			baseEntityCall.bundle = null
			return
	
	baseEntityCall.sendCall(null)

func cellCall(methodname, arguments=[]):
	if KBEngine.app.currserver == "loginapp":
		KBEngine.Dbg.ERROR_MSG(className + "::cellCall(" + methodname + "), currserver=!" + KBEngine.app.currserver)
		return
	
	if not KBEngine.EntityDef.moduledefs.has(className):
		KBEngine.Dbg.ERROR_MSG("entity::cellCall:  entity-module(" + className + ") error, can not find in EntityDef.moduledefs")
		return
	var module = KBEngine.EntityDef.moduledefs[className]

	if not module.cell_methods.has(methodname):
		KBEngine.Dbg.ERROR_MSG(className + "::cellCall(" + methodname + "), method not found")
		return
	var method = module.cell_methods[methodname]
	
	var methodID = method.methodUtype
	
	if len(arguments) != len(method.args):
		KBEngine.Dbg.ERROR_MSG(className + "::cellCall(" + methodname + "): args(" + str(len(arguments)) + " != " + str(len(method.args)) + ") error, wrong number of arguments!")
		return
	
	var cellEntityCall = getCellEntityCall()
	
	if cellEntityCall == null:
		KBEngine.Dbg.ERROR_MSG(className + "::cellCall(" + methodname + "): no cell!")
		return
	
	cellEntityCall.newCall()
	cellEntityCall.bundle.writeUint16(0)
	cellEntityCall.bundle.writeUint16(methodID)
	
	for i in range(len(arguments)):
		if method.args[i].isSameType(arguments[i]):
			method.args[i].addToStream(cellEntityCall.bundle, arguments[i])
		else:
			var err_text = "arg" + str(i) + ": " + str(method.args[i])
			KBEngine.Dbg.ERROR_MSG(className + "::cellCall(method=" + methodname + "): args type error(" + err_text + ")!")
			cellEntityCall.bundle = null
			return
	
	cellEntityCall.sendCall(null)

func enterWorld():
	inWorld = true
	onEnterWorld()
	KBEngine.Event.fireOut("onEnterWorld", [self])

func onEnterWorld():
	pass

func leaveWorld():
	inWorld = false
	onLeaveWorld()
	KBEngine.Event.fireOut("onLeaveWorld", [self])

func onLeaveWorld():
	pass

func enterSpace():
	inWorld = true
	onEnterSpace()
	KBEngine.Event.fireOut("onEnterSpace", [self])
	
	#To immediately refresh the position of the 
	# render layer object
	KBEngine.Event.fireOut("set_position", [self])
	KBEngine.Event.fireOut("set_direction", [self])

func onEnterSpace():
	pass

func leaveSpace():
	inWorld = false
	onLeaveSpace()
	KBEngine.Event.fireOut("onLeaveSpace", [self])

func onLeaveSpace():
	pass

func onPositionChanged(old):
	if isPlayer():
		KBEngine.app.entityServerPos(position)
	
	if inWorld:
		KBEngine.Event.fireOut("set_position", [self])

func onUpdateVolatileData():
	pass

func onDirectionChanged(old):
	if inWorld:
		KBEngine.Event.fireOut("set_direction", [self])

#This callback method is called when the local entity
# control by the client has been enabled or disabled. 
#See the Entity.controlledBy() method in the CellApp
# server code for more infomation.
#param "isControlled_":
#For the player himself, it indicates whether he is controlled by other players;
#For other entities, indicates whether my machine controls the entity
func onControlled(isControlled_):
	pass