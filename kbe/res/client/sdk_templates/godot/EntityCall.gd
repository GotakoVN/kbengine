enum ENTITYCALL_TYPE {
	ENTITYCALL_TYPE_CELL = 0,
	ENTITYCALL_TYPE_BASE = 1,
}

var id = 0
var className = ""
var type = ENTITYCALL_TYPE.ENTITYCALL_TYPE_CELL

var bundle = null

func _init(eid, ename):
	id = eid
	className = ename
	
func __init__():
	pass
	
func isBase():
	return type == ENTITYCALL_TYPE.ENTITYCALL_TYPE_BASE

func isCell():
	return type == ENTITYCALL_TYPE.ENTITYCALL_TYPE_CELL

func _noargs_newCall():
	if bundle == null:
		bundle = KBEngine.Bundle.createObject()
	
	if isCell():
		bundle.newMessage(KBEngine.Messages.messages["Baseapp_onRemoteCallCellMethodFromClient"])
	else:
		bundle.newMessage(KBEngine.Messages.messages["Entity_onRemoteMethodCall"])
	
	bundle.writeInt32(self.id)
	
	return bundle

func newCall(methodName=null, entitycomponentPropertyID=null):
	if methodName == null and entitycomponentPropertyID == null:
		_noargs_newCall()
		return
	
	if KBEngine.app.currserver == "loginapp":
		KBEngine.Dbg.ERROR_MSG(className + "::newCall(" + methodName + "), currserver=" + KBEngineApp.app.currserver)
		return null
	
	if not KBEngine.EntityDef.moduledefs.has(className):
		KBEngine.Dbg.ERROR_MSG(className + "::newCall: entity-module(" + className + ") error, can not find from EntityDef.moduledefs")
		return null
	var module = KBEngine.EntityDef.moduledefs[className]
	
	var method = null
	if isCell():
		method = module.cell_methods[methodName]
	else:
		method = module.base_methods[methodName]
	
	var methodID = method.methodUtype
	
	_noargs_newCall()
	
	bundle.writeUint16(entitycomponentPropertyID)
	bundle.writeUint16(methodID)
	return bundle
	
func sendCall(inbundle):
	if inbundle == null:
		inbundle = bundle
	
	inbundle.send(KBEngine.app.networkInterface())
	
	if inbundle == bundle:
		bundle = null