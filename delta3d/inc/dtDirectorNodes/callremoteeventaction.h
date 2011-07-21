/*
 * Delta3D Open Source Game and Simulation Engine
 * Copyright (C) 2008 MOVES Institute
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * Author: Jeff P. Houde
 */

#ifndef CALL_REMOTE_EVENT_ACTION_NODE
#define CALL_REMOTE_EVENT_ACTION_NODE

////////////////////////////////////////////////////////////////////////////////

#include <dtDirector/latentactionnode.h>
#include <dtDirectorNodes/nodelibraryexport.h>
#include <dtDAL/actorproperty.h>

namespace dtDirector
{
   ////////////////////////////////////////////////////////////////////////////////
   class NODE_LIBRARY_EXPORT CallRemoteEventAction: public LatentActionNode
   {
   public:
      enum EventScopeType
      {
         LOCAL_SCOPE,
         SCRIPT_SCOPE,
         GLOBAL_SCOPE,
      };

      /**
       * Constructor.
       */
      CallRemoteEventAction();

      /**
       * Initializes the Node.
       *
       * @param[in]  nodeType  The node type.
       * @param[in]  graph     The graph that owns this node.
       */
      virtual void Init(const NodeType& nodeType, DirectorGraph* graph);

      /**
       * This method is called in init, which instructs the node
       * to create its properties.  Methods implementing this should
       * be sure to call their parent class's buildPropertyMap method to
       * ensure all properties in the proxy inheritance hierarchy are
       * correctly added to the property map.
       *
       * @see GetDeprecatedProperty to handle old properties that need
       *       to be removed.
       */
      virtual void BuildPropertyMap();

      /**
       * This function queries the proxy with any properties not
       * found in the property list. If a property was previously
       * removed from the proxy, but is still important to load,
       * then this function should return a temporary property of
       * the appropriate type to be used when loading the map.
       *
       * @param[in]  name  The name of the property queried for.
       *
       * @return           A temporary property, or NULL if
       *                   none is needed.
       */
      virtual dtCore::RefPtr<dtDAL::ActorProperty> GetDeprecatedProperty(const std::string& name);

      /**
       * Updates the node.
       * @note  Parent implementation will auto activate any trigger
       *        with the "Out" label by default.
       *
       * @param[in]  simDelta     The simulation time step.
       * @param[in]  delta        The real time step.
       * @param[in]  input        The index to the input that is active.
       * @param[in]  firstUpdate  True if this input was just activated,
       * @param[in]  data         A reference to a custom data pointer for the
       *                           currently active thread stack being executed.
       *                           If this parameter is used, the memory allocated
       *                           for this data should be released prior to
       *                           returning a false result.
       *
       * @return     True if the current node should remain active.
       */
      virtual bool Update(float simDelta, float delta, int input, bool firstUpdate, void*& data);

      /**
       * This event is called by value nodes that are linked via
       * value links when that value has changed.
       *
       * @param[in]  linkName  The name of the value link that is changing.
       */
      virtual void OnLinkValueChanged(const std::string& linkName);

      /**
       * Updates the name of the node.
       */
      void UpdateName();

      /**
       * Accessors for property values.
       */
      void SetEventName(const std::string& value);
      const std::string& GetEventName();
      std::vector<std::string> GetEventList();

      /**
       * Accessors for the local event property flag.
       */
      void SetLocalEvent(bool value);
      bool IsLocalEvent() const;

      /**
       * Accessors for the event scope property.
       */
      void SetEventScope(const std::string& value);
      std::string GetEventScope() const;
      std::vector<std::string> GetEventScopeList();

      /**
       * Accessors for the instigator property.
       */
      void SetInstigator(const dtCore::UniqueId& value);
      const dtCore::UniqueId& GetInstigator() const;

   protected:
      /**
       * Destructor.
       */
      ~CallRemoteEventAction();

   private:

      struct TrackingData
      {
         dtCore::ObserverPtr<Director> script;
         int id;
      };

      std::string      mEventName;
      int              mEventScope;
      dtCore::UniqueId mInstigator;
   };
}

////////////////////////////////////////////////////////////////////////////////

#endif // CALL_REMOTE_EVENT_ACTION_NODE
