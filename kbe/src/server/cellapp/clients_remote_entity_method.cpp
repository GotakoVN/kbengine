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

#include "witness.h"
#include "cellapp.h"
#include "entitydef/method.h"
#include "clients_remote_entity_method.h"
#include "network/bundle.h"
#include "network/network_stats.h"
#include "helper/eventhistory_stats.h"

#include "client_lib/client_interface.h"
#include "../../server/baseapp/baseapp_interface.h"

namespace KBEngine{

SCRIPT_METHOD_DECLARE_BEGIN(ClientsRemoteEntityMethod)
SCRIPT_METHOD_DECLARE_END()

SCRIPT_MEMBER_DECLARE_BEGIN(ClientsRemoteEntityMethod)
SCRIPT_MEMBER_DECLARE_END()

SCRIPT_GETSET_DECLARE_BEGIN(ClientsRemoteEntityMethod)
SCRIPT_GETSET_DECLARE_END()
SCRIPT_INIT(ClientsRemoteEntityMethod, tp_call, 0, 0, 0, 0)	

//-------------------------------------------------------------------------------------
ClientsRemoteEntityMethod::ClientsRemoteEntityMethod(PropertyDescription* pComponentPropertyDescription, 
													const ScriptDefModule* pScriptModule,
													MethodDescription* methodDescription,
													 bool otherClients,
													 ENTITY_ID id):
script::ScriptObject(getScriptType(), false),
pComponentPropertyDescription_(pComponentPropertyDescription),
pScriptModule_(pScriptModule),
methodDescription_(methodDescription),
otherClients_(otherClients),
id_(id)
{
}

//-------------------------------------------------------------------------------------
ClientsRemoteEntityMethod::~ClientsRemoteEntityMethod()
{
}

//-------------------------------------------------------------------------------------
PyObject* ClientsRemoteEntityMethod::tp_call(PyObject* self, PyObject* args, 
	PyObject* kwds)	
{
	ClientsRemoteEntityMethod* rmethod = static_cast<ClientsRemoteEntityMethod*>(self);
	return rmethod->callmethod(args, kwds);	
}		

//-------------------------------------------------------------------------------------
PyObject* ClientsRemoteEntityMethod::callmethod(PyObject* args, PyObject* kwds)
{
	// Get other entities in entityView range
	// Push this method call to the clients of these entities
	MethodDescription* methodDescription = getDescription();

	Entity* pEntity = Cellapp::getSingleton().findEntity(id_);
	if(pEntity == NULL || /*pEntity->pWitness() == NULL ||*/
		pEntity->isDestroyed() /*|| pEntity->clientEntityCall() == NULL*/)
	{
		//WARNING_MSG(fmt::format("EntityRemoteMethod::callClientMethod: not found entity({}).\n", 
		//	entityCall->id()));

		S_Return;
	}
	
	const std::list<ENTITY_ID>& entities = pEntity->witnesses();

	if(otherClients_)
	{
		if(pEntity->witnessesSize() == 0)
			S_Return;
	}
	
	// Send it to yourself
	if(methodDescription->checkArgs(args))
	{
		MemoryStream* mstream = MemoryStream::createPoolObject();

		// If it is a message broadcast to the component
		if (pComponentPropertyDescription_)
		{
			if (pScriptModule_->usePropertyDescrAlias())
				(*mstream) << pComponentPropertyDescription_->aliasIDAsUint8();
			else
				(*mstream) << pComponentPropertyDescription_->getUType();
		}
		else
		{
			if (pScriptModule_->usePropertyDescrAlias())
				(*mstream) << (uint8)0;
			else
				(*mstream) << (ENTITY_PROPERTY_UID)0;
		}

		methodDescription->addToStream(mstream, args);

		if((!otherClients_ && (pEntity->pWitness() && (pEntity->clientEntityCall()))))
		{
			Network::Bundle* pSendBundle = NULL;
			Network::Channel* pChannel = pEntity->clientEntityCall()->getChannel();

			if (!pChannel)
				pSendBundle = Network::Bundle::createPoolObject();
			else
				pSendBundle = pChannel->createSendBundle();

			pEntity->clientEntityCall()->newCall_((*pSendBundle));

			if(mstream->wpos() > 0)
				(*pSendBundle).append(mstream->data(), (int)mstream->wpos());

			if(Network::g_trace_packet > 0)
			{
				if(Network::g_trace_packet_use_logfile)
					DebugHelper::getSingleton().changeLogger("packetlogs");

				DEBUG_MSG(fmt::format("ClientsRemoteEntityMethod::callmethod: pushUpdateData: ClientInterface::onRemoteMethodCall({}::{})\n", 
					pEntity->scriptName(), methodDescription->getName()));

				switch(Network::g_trace_packet)
				{
				case 1:
					mstream->hexlike();
					break;
				case 2:
					mstream->textlike();
					break;
				default:
					mstream->print_storage();
					break;
				};

				if(Network::g_trace_packet_use_logfile)	
					DebugHelper::getSingleton().changeLogger(COMPONENT_NAME_EX(g_componentType));
			}

			// Record the amount of data generated by this event
			g_publicClientEventHistoryStats.trackEvent(pEntity->scriptName(),
				methodDescription->getName(),
				pSendBundle->currMsgLength(),
				"::");

			//entityCall->sendCall((*pBundle));
			pEntity->pWitness()->sendToClient(ClientInterface::onRemoteMethodCall, pSendBundle);
		}

		// Broadcast to others
		std::list<ENTITY_ID>::const_iterator iter = entities.begin();
		for(; iter != entities.end(); ++iter)
		{
			Entity* pViewEntity = Cellapp::getSingleton().findEntity((*iter));
			if(pViewEntity == NULL || pViewEntity->pWitness() == NULL || pViewEntity->isDestroyed())
				continue;
			
			EntityCall* entityCall = pViewEntity->clientEntityCall();
			if(entityCall == NULL)
				continue;

			Network::Channel* pChannel = entityCall->getChannel();
			if(pChannel == NULL)
				continue;

			// This possibility exists, for example, the data comes from createWitnessFromStream()
			// Or if their own entity is not yet created on the target client
			if (!pViewEntity->pWitness()->entityInView(pEntity->id()))
				continue;
			
			Network::Bundle* pSendBundle = pChannel->createSendBundle();
			NETWORK_ENTITY_MESSAGE_FORWARD_CLIENT_BEGIN(pViewEntity->id(), (*pSendBundle));
			
			int ialiasID = -1;
			const Network::MessageHandler& msgHandler = 
			pViewEntity->pWitness()->getViewEntityMessageHandler(ClientInterface::onRemoteMethodCall, 
					ClientInterface::onRemoteMethodCallOptimized, pEntity->id(), ialiasID);

			ENTITY_MESSAGE_FORWARD_CLIENT_BEGIN(pSendBundle, msgHandler, viewEntityMessage);

			if(ialiasID != -1)
			{
				KBE_ASSERT(msgHandler.msgID == ClientInterface::onRemoteMethodCallOptimized.msgID);
				(*pSendBundle)  << (uint8)ialiasID;
			}
			else
			{
				KBE_ASSERT(msgHandler.msgID == ClientInterface::onRemoteMethodCall.msgID);
				(*pSendBundle)  << pEntity->id();
			}

			if(mstream->wpos() > 0)
				(*pSendBundle).append(mstream->data(), (int)mstream->wpos());

			if(Network::g_trace_packet > 0)
			{
				if(Network::g_trace_packet_use_logfile)
					DebugHelper::getSingleton().changeLogger("packetlogs");

				DEBUG_MSG(fmt::format("ClientsRemoteEntityMethod::callmethod: pushUpdateData: ClientInterface::onRemoteOtherEntityMethodCall({}::{})\n", 
					pViewEntity->scriptName(), methodDescription->getName()));

				switch(Network::g_trace_packet)	
				{
				case 1:
					mstream->hexlike();
					break;
				case 2:
					mstream->textlike();
					break;
				default:
					mstream->print_storage();
					break;
				};

				if(Network::g_trace_packet_use_logfile)
					DebugHelper::getSingleton().changeLogger(COMPONENT_NAME_EX(g_componentType));
			}

			ENTITY_MESSAGE_FORWARD_CLIENT_END(pSendBundle, msgHandler, viewEntityMessage);

			// Record the amount of data generated by this even
			g_publicClientEventHistoryStats.trackEvent(pViewEntity->scriptName(), 
				methodDescription->getName(), 
				pSendBundle->currMsgLength(), 
				"::");

			pViewEntity->pWitness()->sendToClient(ClientInterface::onRemoteMethodCallOptimized, pSendBundle);
		}

		MemoryStream::reclaimPoolObject(mstream);
	}

	S_Return;
}

//-------------------------------------------------------------------------------------

}
