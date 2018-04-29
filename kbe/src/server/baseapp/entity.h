/*
This source file is part of KBEngine
For the latest info, see http://www.kbengine.org/

Copyright (c) 2008-2018 KBEngine.

KBEngine is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

KBEngine is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.
 
You should have received a copy of the GNU Lesser General Public License
along with KBEngine.  If not, see <http://www.gnu.org/licenses/>.
*/



#ifndef KBE_BASE_ENTITY_H
#define KBE_BASE_ENTITY_H
	
#include "profile.h"
#include "common/common.h"
#include "helper/debug_helper.h"
#include "pyscript/math.h"
#include "pyscript/scriptobject.h"
#include "entitydef/datatypes.h"	
#include "entitydef/entitydef.h"	
#include "entitydef/scriptdef_module.h"
#include "entitydef/entity_macro.h"	
#include "entitydef/entity_component.h"
#include "server/script_timers.h"		
	
namespace KBEngine{

class EntityCall;
class EntityComponent;
class EntityMessagesForwardCellappHandler;
class BaseMessagesForwardClientHandler;

namespace Network
{
class Channel;
}


class Entity : public script::ScriptObject
{
	/** Populate derived class with some py operations */
	BASE_SCRIPT_HREADER(Entity, ScriptObject)
	ENTITY_HEADER(Entity)
public:
	Entity(ENTITY_ID id, const ScriptDefModule* pScriptModule,
		PyTypeObject* pyType = getScriptType(), bool isInitialised = true);
	~Entity();

	/** 
		Whether is stored in that database 
	*/
	INLINE bool hasDB() const;
	INLINE void hasDB(bool has);

	/** 
		Database Association ID
	*/
	INLINE DBID dbid() const;
	INLINE void dbid(uint16 dbInterfaceIndex, DBID id);
	DECLARE_PY_GET_MOTHOD(pyGetDBID);

	/**
		Database Association Name
	*/
	INLINE uint16 dbInterfaceIndex() const;
	DECLARE_PY_GET_MOTHOD(pyGetDBInterfaceName);

	/** 
		Destroy entity's cell part
	*/
	bool destroyCellEntity(void);

	DECLARE_PY_MOTHOD_ARG0(pyDestroyCellEntity);
	
	/** 
		script gets cell EntityCall
	*/
	DECLARE_PY_GET_MOTHOD(pyGetCellEntityCall);

	EntityCall* cellEntityCall(void) const;

	void cellEntityCall(EntityCall* entityCall);
	
	/** 
		script gets client EntityCall
	*/
	DECLARE_PY_GET_MOTHOD(pyGetClientEntityCall);

	EntityCall* clientEntityCall() const;

	void clientEntityCall(EntityCall* entityCall);

	/**
		Whether space was created
	*/
	INLINE bool isCreatedSpace();

	/** 
		cellData section
	*/
	bool installCellDataAttr(PyObject* dictData = NULL, bool installpy = true);

	void createCellData(void);

	void destroyCellData(void);

	void addPersistentsDataToStream(uint32 flags, MemoryStream* s);

	PyObject* createCellDataDict(uint32 flags);

	INLINE PyObject* getCellData(void) const;
	
	INLINE bool creatingCell(void) const;

	/**
		The request cell part to update the entity's celldata.
	*/
	void reqBackupCellData();
	
	/** 
		Write backup information to the stream
	*/
	void writeBackupData(MemoryStream* s);
	void onBackup();

	/** 
		Write archive information to the stream
	*/
	void writeArchiveData(MemoryStream* s);

	/** 
		Notifications before you saved to the database 
	*/
	void onWriteToDB();
	void onCellWriteToDBCompleted(CALLBACK_ID callbackID, int8 shouldAutoLoad, int dbInterfaceIndex);
	void onWriteToDBCallback(ENTITY_ID eid, DBID entityDBID, uint16 dbInterfaceIndex,
		CALLBACK_ID callbackID, int8 shouldAutoLoad, bool success);

	/** Network interface
		The first time the entity writes the dbid returned by the database from dbmgr
	*/
	void onGetDBID(Network::Channel* pChannel, DBID dbid);

	/** 
		Create cell failure callback
	*/
	void onCreateCellFailure(void);

	/** 
		Create cell success callback
	*/
	void onGetCell(Network::Channel* pChannel, COMPONENT_ID componentID);

	/** 
		Lost cell notification
	*/
	void onLoseCell(Network::Channel* pChannel, MemoryStream& s);

	/** 
		When cellapp unexpectedly terminates, baseapp can recover if it finds a suitable cellapp.
		Will call this method
	*/
	void onRestore();

	/** 
		Backup cell data
	*/
	void onBackupCellData(Network::Channel* pChannel, MemoryStream& s);

	/** 
		Client lost
	*/
	void onClientDeath();

	/** Network interface
		Method for remotely calling this entity
	*/
	void onRemoteMethodCall(Network::Channel* pChannel, MemoryStream& s);

	/** 
		Destroy this entity
	*/
	void onDestroy(bool callScript);

	/**
		Destroy the base internal notification
	*/
	void onDestroyEntity(bool deleteFromDB, bool writeToDB);

	/** 
		create the cell section of this base entity on the specified cell
	*/
	DECLARE_PY_MOTHOD_ARG1(createCellEntity, PyObject_ptr);
	
	/** 
		Restore a baseEntity's cell part on the specified cell
	*/
	void restoreCell(EntityCallAbstract* cellEntityCall);
	INLINE bool inRestore();

	/** 
		Create a cellEntity on a new space
	*/
	DECLARE_PY_MOTHOD_ARG1(createCellEntityInNewSpace, PyObject_ptr);

	/** Network interface
		The client sends a message directly to the cell entity
	*/
	void forwardEntityMessageToCellappFromClient(Network::Channel* pChannel, MemoryStream& s);
	
	/**
		Send message to cellapp
	*/
	void sendToCellapp(Network::Bundle* pBundle);
	void sendToCellapp(Network::Channel* pChannel, Network::Bundle* pBundle);

	/** 
		teleport
	*/
	DECLARE_PY_MOTHOD_ARG1(pyTeleport, PyObject_ptr);

	/**
		Send callback
	*/
	void onTeleportCB(Network::Channel* pChannel, SPACE_ID spaceID, bool fromCellTeleport);  
	void onTeleportFailure();  
	void onTeleportSuccess(SPACE_ID spaceID);

	/** Network interface
		An entity requests teleport to the space of this entity.
	*/
	void reqTeleportOther(Network::Channel* pChannel, ENTITY_ID reqTeleportEntityID, 
		COMPONENT_ID reqTeleportEntityCellAppID, COMPONENT_ID reqTeleportEntityBaseAppID);

	/** Network interface
		The process of the entity request to migrate to another cellapp begins and ends.
	*/
	void onMigrationCellappStart(Network::Channel* pChannel, COMPONENT_ID sourceCellAppID, COMPONENT_ID targetCellAppID);
	void onMigrationCellappEnd(Network::Channel* pChannel, COMPONENT_ID sourceCellAppID, COMPONENT_ID targetCellAppID);
	void onMigrationCellappOver(COMPONENT_ID targetCellAppID);
	
	/**
		Set whether to automatically archive
	*/
	INLINE int8 shouldAutoArchive() const;
	INLINE void shouldAutoArchive(int8 v);
	DECLARE_PY_GETSET_MOTHOD(pyGetShouldAutoArchive, pySetShouldAutoArchive);

	/**
		Set whether to automatically backup
	*/
	INLINE int8 shouldAutoBackup() const;
	INLINE void shouldAutoBackup(int8 v);
	DECLARE_PY_GETSET_MOTHOD(pyGetShouldAutoBackup, pySetShouldAutoBackup);

	/**
		cellap died
	*/
	void onCellAppDeath();

	/** 
		Forwarding message completed
	*/
	void onBufferedForwardToCellappMessagesOver();
	void onBufferedForwardToClientMessagesOver();
	
	INLINE BaseMessagesForwardClientHandler* pBufferedSendToClientMessages();
	
	/** 
		Sets whether the entity persisted data is dirty and will be automatically archived 
	*/
	INLINE void setDirty(bool dirty = true);
	INLINE bool isDirty() const;
	
protected:
	/** 
		Defined attribute data was changed
	*/
	void onDefDataChanged(EntityComponent* pEntityComponent, const PropertyDescription* propertyDescription,
			PyObject* pyData);

	/**
		Erase online log from db
	*/
	void eraseEntityLog();

protected:
	// The entity's client & cell EntityCall
	EntityCall*								clientEntityCall_;
	EntityCall*								cellEntityCall_;

	// After entity is created, some cell property data is saved here before the cell section is created
	PyObject*								cellDataDict_;

	// Whether it is stored in the database
	bool									hasDB_;
	DBID									DBID_;

	// is getting Celldata
	bool									isGetingCellData_;

	// Whether it is archiving
	bool									isArchiving_;

	// Whether to AutoArchive: <= 0 is False, 1 is True, KBE_NEXT_ONLY automatically set to false after execution
	int8									shouldAutoArchive_;
	
	// Whether to backup: <= 0 is False, 1 is True, KBE_NEXT_ONLY automatically set to false after execution
	int8									shouldAutoBackup_;

	// Whether you are creating a cell
	bool									creatingCell_;

	// Have you created a space
	bool									createdSpace_;

	// Whether it is restoring
	bool									inRestore_;
	
	// 如果此时实体还没有被设置为ENTITY_FLAGS_TELEPORT_START,  说明onMigrationCellappArrived包优先于
	// onMigrationCellappStart到达(某些压力所致的情况下会导致实体跨进程跳转时（由cell1跳转到cell2），
	// 跳转前所产生的包会比cell2的enterSpace包慢到达)，因此发生这种情况时需要将cell2的包先缓存
	// 等cell1的包到达后执行完毕再执行cell2的包
	// Google translate...:
	// If the entity has not been set to ENTITY_FLAGS_TELEPORT_START at this time, it means that the 
	//  onMigrationCellappArrived packet is prior to the arrival of onMigrationCellappStart
	//  (a condition caused by some pressure will cause the entity to jump across processes (jump from cell1 to cell2) 
	//  before the jump. The resulting packet arrives slower than cell2's enterSpace packet, so when this happens, 
	//  cell2's packet needs to be cached first.
	// Wait until cell1's packet arrives and then execute cell2's packet
	BaseMessagesForwardClientHandler*		pBufferedSendToClientMessages_;
	
	// Whether the data that needs to be archived. If it is dirty it needs to be re archived
	bool									isDirty_;

	// If this entity has been written to the database, this attribute is the index of the corresponding database interface
	uint16									dbInterfaceIndex_;
};

}


#ifdef CODE_INLINE
#include "entity.inl"
#endif

#endif // KBE_BASE_ENTITY_H
