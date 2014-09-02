/*********************************************************************
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2013, Institute for Artificial Intelligence,
 *  Universität Bremen.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of the Institute for Artificial Intelligence,
 *     Universität Bremen, nor the names of its contributors may be
 *     used to endorse or promote products derived from this software
 *     without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************/

/** \author Jan Winkler */


#ifndef __PLUGIN_SYMBOLIC_LOG_H__
#define __PLUGIN_SYMBOLIC_LOG_H__


#define PLUGIN_CLASS PluginSymbolicLog


// System
#include <cstdlib>
#include <iostream>

// ROS
#include <ros/ros.h>

// Designators
#include <designators/CDesignator.h>
#include <designator_integration_msgs/DesignatorCommunication.h>

// Private
#include <Types.h>
#include <ForwardDeclarations.h>
#include <Plugin.h>
#include <Node.h>


namespace beliefstate {
  namespace plugins {
    class PLUGIN_CLASS : public Plugin {
    private:
      std::list<Node*> m_lstNodes;
      std::list<Node*> m_lstRootNodes;
      Node* m_ndActive;
      std::list< std::pair<std::string, std::string> > m_lstDesignatorIDs;
      std::list< std::pair<std::string, std::string> > m_lstDesignatorEquations;
      std::list< std::pair<std::string, std::string> > m_lstDesignatorEquationTimes;
      std::pair<std::string, Node*> m_prLastFailure;
      std::map<std::string, Node*> m_mapFailureCatchers;
      
    public:
      PLUGIN_CLASS();
      ~PLUGIN_CLASS();
      
      virtual Result init(int argc, char** argv);
      virtual Result deinit();
      
      virtual Result cycle();
      
      virtual void consumeEvent(Event evEvent);
      virtual Event consumeServiceEvent(ServiceEvent seServiceEvent);
      
      Node* addNode(std::string strName, int nContextID);
      void setNodeAsActive(Node* ndActive);
      Node* activeNode();
      
      std::string getDesignatorID(std::string strMemoryAddress);
      std::string getUniqueDesignatorID(std::string strMemoryAddress, CKeyValuePair* ckvpDescription);
      std::string generateRandomIdentifier(std::string strPrefix, unsigned int unLength);
      std::string equateDesignators(std::string strMAChild, CKeyValuePair* ckvpDescriptionChild, std::string strMAParent, CKeyValuePair* ckvpDescriptionParent);
      
      bool ensureDesignatorPublished(std::list<CKeyValuePair*> lstDescription, std::string strMemoryAddress, std::string strType, std::string strAnnotation = "", bool bAdd = false);
      void setNestedDesignatorUniqueIDs(CKeyValuePair* ckvpParent);
    };
  }
  
  extern "C" plugins::PLUGIN_CLASS* createInstance();
  extern "C" void destroyInstance(plugins::PLUGIN_CLASS* icDestroy);
}


#endif /* __PLUGIN_SYMBOLIC_LOG_H__ */
