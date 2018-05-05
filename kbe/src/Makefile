##############
# setup vars #
##############

export KBE_SRC_ROOT=$(shell cd ../.. && pwd)

KBE_SERVER_BINARIES =	../bin/server/baseapp ../bin/server/baseappmgr \
						../bin/server/cellapp ../bin/server/cellappmgr \
						../bin/server/dbmgr ../bin/server/loginapp \
						../bin/server/machine ../bin/server/kbcmd
				
KBE_LIBS =	libs/libclient_lib.a libs/libcommon.a libs/libdb_redis.a \
			libs/libdb_mysql.a libs/libdb_interface.a libs/libentitydef.a \
			libs/libmath.a libs/libresmgr.a libs/libpyscript.a libs/libserver.a \
			libs/libnavigation.a libs/libnetwork.a libs/libhelper.a \
			libs/libthread.a libs/libxml.a

KBE_DEPENDS = libs/libg3dlite.a libs/libsigar.a libs/libtmxparser.a libs/libjwsmtp.a

##################
# default target #
##################

all: $(KBE_SERVER_BINARIES)

###################
# server/ targets #
###################

../bin/server/baseapp: $(KBE_LIBS)
	$(MAKE) -C server/baseapp

../bin/server/baseappmgr: $(KBE_LIBS)
	$(MAKE) -C server/baseappmgr

../bin/server/cellapp: $(KBE_LIBS)
	$(MAKE) -C server/cellapp

../bin/server/cellappmgr: $(KBE_LIBS)
	$(MAKE) -C server/cellappmgr

../bin/server/dbmgr: $(KBE_LIBS)
	$(MAKE) -C server/dbmgr

../bin/server/loginapp: $(KBE_LIBS)
	$(MAKE) -C server/loginapp

../bin/server/machine: $(KBE_LIBS)
	$(MAKE) -C server/machine

../bin/server/kbcmd: $(KBE_LIBS)
	$(MAKE) -C server/tools

################
# lib/ targets #
################

#ls | egrep "\.cpp|\.h" | sed "s/^/$(pwd | sed 's/\/home\/computer\/src\/kbeprojs\/kbengine\/kbe\/src\///g' | sed 's/\//\\\//g')\//g" | tr "\n\r" " " | xsel -ib
#KBE libs

libs/libclient_lib.a: $(KBE_DEPENDS) lib/client_lib/clientapp.cpp lib/client_lib/clientapp.h lib/client_lib/client_interface.cpp lib/client_lib/client_interface.h lib/client_lib/client_interface_macros.h lib/client_lib/clientobjectbase.cpp lib/client_lib/clientobjectbase.h lib/client_lib/common.h lib/client_lib/config.cpp lib/client_lib/config.h lib/client_lib/entity_aspect.cpp lib/client_lib/entity_aspect.h lib/client_lib/entity_component.cpp lib/client_lib/entity.cpp lib/client_lib/entity.h lib/client_lib/event.cpp lib/client_lib/event.h lib/client_lib/kbemain.h lib/client_lib/moveto_point_handler.cpp lib/client_lib/moveto_point_handler.h lib/client_lib/profile.cpp lib/client_lib/profile.h lib/client_lib/script_callbacks.cpp lib/client_lib/script_callbacks.h 
	$(MAKE) -C lib/client_lib

libs/libcommon.a: $(KBE_DEPENDS) lib/common/base64.cpp lib/common/base64.h lib/common/blowfish.cpp lib/common/blowfish.h lib/common/common.cpp lib/common/common.h lib/common/deadline.h lib/common/format.cpp lib/common/format.h lib/common/kbekey.cpp lib/common/kbekey.h lib/common/kbemalloc.h lib/common/kbeversion.cpp lib/common/kbeversion.h lib/common/md5.cpp lib/common/md5.h lib/common/memorystream_converter.h lib/common/memorystream.cpp lib/common/memorystream.h lib/common/objectpool.h lib/common/platform.h lib/common/refcountable.h lib/common/rsa.cpp lib/common/rsa.h lib/common/sha1.cpp lib/common/sha1.h lib/common/singleton.h lib/common/smartpointer.h lib/common/stdfindif_handers.h lib/common/stringconv.h lib/common/strutil.cpp lib/common/strutil.h lib/common/task.h lib/common/tasks.cpp lib/common/tasks.h lib/common/timer.cpp lib/common/timer.h lib/common/timestamp.cpp lib/common/timestamp.h 
	$(MAKE) -C lib/common

libs/libdb_redis.a: $(KBE_DEPENDS) lib/db_redis/common.cpp lib/db_redis/common.h lib/db_redis/db_context.h lib/db_redis/db_exception.cpp lib/db_redis/db_exception.h lib/db_redis/db_interface_redis.cpp lib/db_redis/db_interface_redis.h lib/db_redis/db_transaction.cpp lib/db_redis/db_transaction.h lib/db_redis/entity_table_redis.cpp lib/db_redis/entity_table_redis.h lib/db_redis/kbe_table_redis.cpp lib/db_redis/kbe_table_redis.h lib/db_redis/redis_helper.h lib/db_redis/redis_watcher.cpp lib/db_redis/redis_watcher.h 
	$(MAKE) -C lib/db_redis

libs/libdb_mysql.a: $(KBE_DEPENDS) lib/db_mysql/common.cpp lib/db_mysql/common.h lib/db_mysql/db_context.h lib/db_mysql/db_exception.cpp lib/db_mysql/db_exception.h lib/db_mysql/db_interface_mysql.cpp lib/db_mysql/db_interface_mysql.h lib/db_mysql/db_transaction.cpp lib/db_mysql/db_transaction.h lib/db_mysql/entity_sqlstatement_mapping.cpp lib/db_mysql/entity_sqlstatement_mapping.h lib/db_mysql/entity_table_mysql.cpp lib/db_mysql/entity_table_mysql.h lib/db_mysql/kbe_table_mysql.cpp lib/db_mysql/kbe_table_mysql.h lib/db_mysql/read_entity_helper.h lib/db_mysql/remove_entity_helper.h lib/db_mysql/sqlstatement.h lib/db_mysql/write_entity_helper.h 
	$(MAKE) -C lib/db_mysql

libs/libdb_interface.a: $(KBE_DEPENDS) lib/db_interface/db_interface.cpp lib/db_interface/db_interface.h lib/db_interface/db_tasks.cpp lib/db_interface/db_tasks.h lib/db_interface/db_threadpool.cpp lib/db_interface/db_threadpool.h lib/db_interface/entity_table.cpp lib/db_interface/entity_table.h lib/db_interface/kbe_tables.cpp lib/db_interface/kbe_tables.h 
	$(MAKE) -C lib/db_interface

libs/libentitydef.a: $(KBE_DEPENDS) lib/entitydef/common.cpp lib/entitydef/common.h lib/entitydef/datatype.cpp lib/entitydef/datatype.h lib/entitydef/datatypes.cpp lib/entitydef/datatypes.h lib/entitydef/detaillevel.cpp lib/entitydef/detaillevel.h lib/entitydef/entities.h lib/entitydef/entitycallabstract.cpp lib/entitydef/entitycallabstract.h lib/entitydef/entity_call.cpp lib/entitydef/entity_call.h lib/entitydef/entity_component_call.cpp lib/entitydef/entity_component_call.h lib/entitydef/entity_component.cpp lib/entitydef/entity_component.h lib/entitydef/entitydef.cpp lib/entitydef/entitydef.h lib/entitydef/entity_garbages.h lib/entitydef/entity_macro.h lib/entitydef/fixedarray.cpp lib/entitydef/fixedarray.h lib/entitydef/fixeddict.cpp lib/entitydef/fixeddict.h lib/entitydef/method.cpp lib/entitydef/method.h lib/entitydef/property.cpp lib/entitydef/property.h lib/entitydef/remote_entity_method.cpp lib/entitydef/remote_entity_method.h lib/entitydef/scriptdef_module.cpp lib/entitydef/scriptdef_module.h lib/entitydef/volatileinfo.cpp lib/entitydef/volatileinfo.h 
	$(MAKE) -C lib/entitydef

libs/libmath.a: $(KBE_DEPENDS) lib/math/math.cpp lib/math/math.h 
	$(MAKE) -C lib/math

libs/libresmgr.a: $(KBE_DEPENDS) lib/resmgr/resmgr.cpp lib/resmgr/resmgr.h lib/resmgr/resourceobject.cpp lib/resmgr/resourceobject.h 
	$(MAKE) -C lib/resmgr

libs/libpyscript.a: $(KBE_DEPENDS) lib/pyscript/copy.cpp lib/pyscript/copy.h lib/pyscript/install_py_dlls.cpp lib/pyscript/install_py_dlls.h lib/pyscript/map.cpp lib/pyscript/map.h lib/pyscript/math.cpp lib/pyscript/math.h lib/pyscript/pickler.cpp lib/pyscript/pickler.h lib/pyscript/py_gc.cpp lib/pyscript/py_gc.h lib/pyscript/py_macros.h lib/pyscript/py_memorystream.cpp lib/pyscript/py_memorystream.h lib/pyscript/pyobject_pointer.h lib/pyscript/pyprofile.cpp lib/pyscript/pyprofile.h lib/pyscript/pyprofile_handler.cpp lib/pyscript/pyprofile_handler.h lib/pyscript/pystruct.cpp lib/pyscript/pystruct.h lib/pyscript/pythread_lock.h lib/pyscript/pywatcher.cpp lib/pyscript/pywatcher.h lib/pyscript/script.cpp lib/pyscript/script.h lib/pyscript/scriptobject.cpp lib/pyscript/scriptobject.h lib/pyscript/scriptstderr.cpp lib/pyscript/scriptstderr.h lib/pyscript/scriptstdout.cpp lib/pyscript/scriptstdouterr.cpp lib/pyscript/scriptstdouterr.h lib/pyscript/scriptstdouterrhook.cpp lib/pyscript/scriptstdouterrhook.h lib/pyscript/scriptstdout.h lib/pyscript/sequence.cpp lib/pyscript/sequence.h lib/pyscript/vector2.cpp lib/pyscript/vector2.h lib/pyscript/vector3.cpp lib/pyscript/vector3.h lib/pyscript/vector4.cpp lib/pyscript/vector4.h 
	$(MAKE) -C lib/pyscript

libs/libserver.a: $(KBE_DEPENDS) lib/server/callbackmgr.h lib/server/common.cpp lib/server/common.h lib/server/component_active_report_handler.cpp lib/server/component_active_report_handler.h lib/server/components.cpp lib/server/components.h lib/server/entity_app.cpp lib/server/entity_app.h lib/server/forward_messagebuffer.cpp lib/server/forward_messagebuffer.h lib/server/globaldata_client.cpp lib/server/globaldata_client.h lib/server/globaldata_server.cpp lib/server/globaldata_server.h lib/server/idallocate.cpp lib/server/idallocate.h lib/server/kbemain.h lib/server/machine_infos.cpp lib/server/machine_infos.h lib/server/pendingLoginmgr.cpp lib/server/pendingLoginmgr.h lib/server/py_file_descriptor.cpp lib/server/py_file_descriptor.h lib/server/python_app.cpp lib/server/python_app.h lib/server/script_timers.cpp lib/server/script_timers.h lib/server/sendmail_threadtasks.cpp lib/server/sendmail_threadtasks.h lib/server/serverapp.cpp lib/server/serverapp.h lib/server/serverconfig.cpp lib/server/serverconfig.h lib/server/server_errors.h lib/server/shutdowner.cpp lib/server/shutdowner.h lib/server/shutdown_handler.h lib/server/signal_handler.cpp lib/server/signal_handler.h lib/server/telnet_handler.cpp lib/server/telnet_handler.h lib/server/telnet_server.cpp lib/server/telnet_server.h 
	$(MAKE) -C lib/server

libs/libnavigation.a: $(KBE_DEPENDS) lib/navigation/DetourAlloc.cpp lib/navigation/DetourAlloc.h lib/navigation/DetourAssert.h lib/navigation/DetourCommon.cpp lib/navigation/DetourCommon.h lib/navigation/DetourCrowd.cpp lib/navigation/DetourCrowd.h lib/navigation/DetourLocalBoundary.cpp lib/navigation/DetourLocalBoundary.h lib/navigation/DetourMath.h lib/navigation/DetourNavMeshBuilder.cpp lib/navigation/DetourNavMeshBuilder.h lib/navigation/DetourNavMesh.cpp lib/navigation/DetourNavMesh.h lib/navigation/DetourNavMeshQuery.cpp lib/navigation/DetourNavMeshQuery.h lib/navigation/DetourNode.cpp lib/navigation/DetourNode.h lib/navigation/DetourObstacleAvoidance.cpp lib/navigation/DetourObstacleAvoidance.h lib/navigation/DetourPathCorridor.cpp lib/navigation/DetourPathCorridor.h lib/navigation/DetourPathQueue.cpp lib/navigation/DetourPathQueue.h lib/navigation/DetourProximityGrid.cpp lib/navigation/DetourProximityGrid.h lib/navigation/DetourStatus.h lib/navigation/DetourTileCacheBuilder.cpp lib/navigation/DetourTileCacheBuilder.h lib/navigation/DetourTileCache.cpp lib/navigation/DetourTileCache.h lib/navigation/findpath_9tile_bak.cpp lib/navigation/navigation.cpp lib/navigation/navigation.h lib/navigation/navigation_handle.cpp lib/navigation/navigation_handle.h lib/navigation/navigation_mesh_handle.cpp lib/navigation/navigation_mesh_handle.h lib/navigation/navigation_tile_handle.cpp lib/navigation/navigation_tile_handle.h lib/navigation/RecastAlloc.cpp lib/navigation/RecastAlloc.h lib/navigation/RecastArea.cpp lib/navigation/RecastAssert.h lib/navigation/RecastContour.cpp lib/navigation/Recast.cpp lib/navigation/RecastFilter.cpp lib/navigation/Recast.h lib/navigation/RecastLayers.cpp lib/navigation/RecastMesh.cpp lib/navigation/RecastMeshDetail.cpp lib/navigation/RecastRasterization.cpp lib/navigation/RecastRegion.cpp lib/navigation/stlastar.h lib/navigation/stlstarfsa.h 
	$(MAKE) -C lib/navigation

libs/libnetwork.a: $(KBE_DEPENDS) lib/network/address.cpp lib/network/address.h lib/network/bundle_broadcast.cpp lib/network/bundle_broadcast.h lib/network/bundle.cpp lib/network/bundle.h lib/network/channel.cpp lib/network/channel.h lib/network/common.cpp lib/network/common.h lib/network/delayed_channels.cpp lib/network/delayed_channels.h lib/network/encryption_filter.cpp lib/network/encryption_filter.h lib/network/endpoint.cpp lib/network/endpoint.h lib/network/error_reporter.cpp lib/network/error_reporter.h lib/network/event_dispatcher.cpp lib/network/event_dispatcher.h lib/network/event_poller.cpp lib/network/event_poller.h lib/network/fixed_messages.cpp lib/network/fixed_messages.h lib/network/http_utility.h lib/network/interface_defs.cpp lib/network/interface_defs.h lib/network/interfaces.h lib/network/listener_receiver.cpp lib/network/listener_receiver.h lib/network/message_handler.cpp lib/network/message_handler.h lib/network/network_exception.h lib/network/network_interface.cpp lib/network/network_interface.h lib/network/network_stats.cpp lib/network/network_stats.h lib/network/packet_filter.cpp lib/network/packet_filter.h lib/network/packet.h lib/network/packet_reader.cpp lib/network/packet_reader.h lib/network/packet_receiver.cpp lib/network/packet_receiver.h lib/network/packet_sender.cpp lib/network/packet_sender.h lib/network/poller_epoll.cpp lib/network/poller_epoll.h lib/network/poller_select.cpp lib/network/poller_select.h lib/network/tcp_packet.cpp lib/network/tcp_packet.h lib/network/tcp_packet_receiver.cpp lib/network/tcp_packet_receiver.h lib/network/tcp_packet_sender.cpp lib/network/tcp_packet_sender.h lib/network/udp_packet.cpp lib/network/udp_packet.h lib/network/udp_packet_receiver.cpp lib/network/udp_packet_receiver.h lib/network/websocket_packet_filter.cpp lib/network/websocket_packet_filter.h lib/network/websocket_packet_reader.cpp lib/network/websocket_packet_reader.h lib/network/websocket_protocol.cpp lib/network/websocket_protocol.h 
	$(MAKE) -C lib/network

libs/libhelper.a: $(KBE_DEPENDS) lib/helper/console_helper.h lib/helper/crashhandler.cpp lib/helper/crashhandler.h lib/helper/debug_helper.cpp lib/helper/debug_helper.h lib/helper/debug_option.cpp lib/helper/debug_option.h lib/helper/eventhistory_stats.cpp lib/helper/eventhistory_stats.h lib/helper/memory_helper.h lib/helper/profile.cpp lib/helper/profile.h lib/helper/profile_handler.cpp lib/helper/profile_handler.h lib/helper/profiler.cpp lib/helper/profiler.h lib/helper/script_loglevel.cpp lib/helper/script_loglevel.h lib/helper/sys_info.cpp lib/helper/sys_info.h lib/helper/watcher.cpp lib/helper/watcher.h lib/helper/watch_pools.cpp lib/helper/watch_pools.h 
	$(MAKE) -C lib/helper

libs/libthread.a: $(KBE_DEPENDS) lib/thread/concurrency.cpp lib/thread/concurrency.h lib/thread/threadguard.h lib/thread/threadmutex.h lib/thread/threadpool.cpp lib/thread/threadpool.h lib/thread/threadtask.h 
	$(MAKE) -C lib/thread

libs/libxml.a: $(KBE_DEPENDS) lib/xml/xml.cpp lib/xml/xml.h 
	$(MAKE) -C lib/xml

#Depends

libs/libg3dlite.a: lib/dependencies/g3dlite/AABox.cpp lib/dependencies/g3dlite/Box.cpp lib/dependencies/g3dlite/Crypto.cpp lib/dependencies/g3dlite/format.cpp lib/dependencies/g3dlite/license.html lib/dependencies/g3dlite/Matrix3.cpp lib/dependencies/g3dlite/Plane.cpp lib/dependencies/g3dlite/System.cpp lib/dependencies/g3dlite/Triangle.cpp lib/dependencies/g3dlite/Vector3.cpp lib/dependencies/g3dlite/Vector4.cpp 
	$(MAKE) -C lib/dependencies/g3dlite

libs/libsigar.a: lib/dependencies/sigar/linux/sigar_fileinfo.h lib/dependencies/sigar/linux/sigar_format.h lib/dependencies/sigar/linux/sigar_getline.h lib/dependencies/sigar/linux/sigar.h lib/dependencies/sigar/linux/sigar_log.h lib/dependencies/sigar/linux/sigar_os.h lib/dependencies/sigar/linux/sigar_private.h lib/dependencies/sigar/linux/sigar_ptql.h lib/dependencies/sigar/linux/sigar_util.h 
	$(MAKE) -C lib/dependencies/sigar/linux

libs/libtmxparser.a: lib/dependencies/tmxparser/base64.cpp lib/dependencies/tmxparser/base64.h lib/dependencies/tmxparser/TmxEllipse.cpp lib/dependencies/tmxparser/TmxEllipse.h lib/dependencies/tmxparser/Tmx.h lib/dependencies/tmxparser/TmxImage.cpp lib/dependencies/tmxparser/TmxImage.h lib/dependencies/tmxparser/TmxImageLayer.cpp lib/dependencies/tmxparser/TmxImageLayer.h lib/dependencies/tmxparser/TmxLayer.cpp lib/dependencies/tmxparser/TmxLayer.h lib/dependencies/tmxparser/TmxMap.cpp lib/dependencies/tmxparser/TmxMap.h lib/dependencies/tmxparser/TmxMapTile.h lib/dependencies/tmxparser/TmxObject.cpp lib/dependencies/tmxparser/TmxObjectGroup.cpp lib/dependencies/tmxparser/TmxObjectGroup.h lib/dependencies/tmxparser/TmxObject.h lib/dependencies/tmxparser/TmxPoint.h lib/dependencies/tmxparser/TmxPolygon.cpp lib/dependencies/tmxparser/TmxPolygon.h lib/dependencies/tmxparser/TmxPolyline.cpp lib/dependencies/tmxparser/TmxPolyline.h lib/dependencies/tmxparser/TmxPropertySet.cpp lib/dependencies/tmxparser/TmxPropertySet.h lib/dependencies/tmxparser/TmxTile.cpp lib/dependencies/tmxparser/TmxTile.h lib/dependencies/tmxparser/TmxTileset.cpp lib/dependencies/tmxparser/TmxTileset.h lib/dependencies/tmxparser/TmxUtil.cpp lib/dependencies/tmxparser/TmxUtil.h 
	$(MAKE) -C lib/dependencies/tmxparser
	
libs/libjwsmtp.a: lib/dependencies/jwsmtp/jwsmtp/jwsmtp/base64.cpp lib/dependencies/jwsmtp/jwsmtp/jwsmtp/base64.h lib/dependencies/jwsmtp/jwsmtp/jwsmtp/compat.cpp lib/dependencies/jwsmtp/jwsmtp/jwsmtp/compat.h lib/dependencies/jwsmtp/jwsmtp/jwsmtp/jwsmtp.h lib/dependencies/jwsmtp/jwsmtp/jwsmtp/mailer.cpp lib/dependencies/jwsmtp/jwsmtp/jwsmtp/mailer.h 
	$(MAKE) -C lib/dependencies/jwsmtp/jwsmtp/jwsmtp

#########
# clean #
#########

clean:
	#lib/
	$(MAKE) -C lib/dependencies/g3dlite clean
	$(MAKE) -C lib/dependencies/sigar/linux clean
	$(MAKE) -C lib/dependencies/tmxparser clean
	$(MAKE) -C lib/dependencies/jwsmtp/jwsmtp/jwsmtp clean
	$(MAKE) -C lib/client_lib clean
	$(MAKE) -C lib/common clean
	$(MAKE) -C lib/db_redis clean
	$(MAKE) -C lib/db_mysql clean
	$(MAKE) -C lib/db_interface clean
	$(MAKE) -C lib/entitydef clean
	$(MAKE) -C lib/math clean
	$(MAKE) -C lib/resmgr clean
	$(MAKE) -C lib/pyscript clean
	$(MAKE) -C lib/server clean
	$(MAKE) -C lib/navigation clean
	$(MAKE) -C lib/network clean
	$(MAKE) -C lib/helper clean
	$(MAKE) -C lib/thread clean
	$(MAKE) -C lib/xml clean
	#server/
	$(MAKE) -C server/baseapp clean
	$(MAKE) -C server/baseappmgr clean
	$(MAKE) -C server/cellapp clean
	$(MAKE) -C server/cellappmgr clean
	$(MAKE) -C server/dbmgr clean
	$(MAKE) -C server/loginapp clean
	$(MAKE) -C server/machine clean
	$(MAKE) -C server/tools clean
