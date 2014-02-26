/*
 <one line to give the library's name and an idea of what it does.>
 Copyright (C) 2013  Guillaume <email>
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <vector>
#include <memory>

//#include "boost/date_time/posix_time/posix_time.hpp"

#ifndef LOGGER_H
#define LOGGER_H


#define LOGGER_START(MIN_PRIORITY, FILE)		Logger::start(MIN_PRIORITY, FILE);
#define LOGGER_STOP() 					Logger::Stop();
#define LOGGER_WRITE(PRIORITY,MESSAGE) 			Logger::write(PRIORITY,MESSAGE);

#define LOGGER_HEADER4COLUMNS(p,a,au,b,bu,c,cu,d,du)  	Logger::header4Columns(p,a,au,b,bu,c,cu,d,du);
#define LOGGER_WRITE4COLUMNS(p,a,b,c,d)  		Logger::write4Columns(p,a,b,c,d);
#define LOGGER_HEADER3COLUMNS(p,a,au,b,bu,c,cu)  	Logger::header3Columns(p,a,au,b,bu,c,cu);
#define LOGGER_WRITE3COLUMNS(p,a,b,c)  			Logger::write3Columns(p,a,b,c);
#define LOGGER_HEADER2COLUMNS(p,a,au,b,bu)  		Logger::header2Columns(p,a,au,b,bu);
#define LOGGER_WRITE2COLUMNS(p,a,b)  			Logger::write2Columns(p,a,b);

#define ATOMISM_LOGIN()          Logger::enterFunction( __PRETTY_FUNCTION__ );
#define ATOMISM_LOGOUT()         Logger::exitFunction(__PRETTY_FUNCTION__);
#define ATOMISM_RETURN(r)        Logger::exitFunction(__PRETTY_FUNCTION__); return r;

using namespace std;

namespace atomism
{
    
   class Exception;
  
    /** Logger to displat messages in outsream
     * 
     * The logger defines the loggin mechanisms in atomism. The mechanisms
     * are controlled by the Loggin option in the configuration, 
     * and the message priority given when a loggin message is sent to the logger.
     * All methods of the logger are static such that it behaves like a singleton. 
     *
     * The rules are as follow: 
     *     - if the option Loggin is set to 0 in the configuration,
     * all the log mechanisms are ignored (the macro defined remove the associated
     * code in the compiled files)
     *     - if the Priority is set to WARNING, only warnig messages are displayed
     *     - if the Priority is set to INFO, warning and info messages are displayed
     *     - if the Priority is set to DEBUG, all messages are displayed.
     *     - if the Priority is set to TRACK, all the messages are displayed 
     * as well as the history of the methods called.
     * To track a method call ATOMISM_LOGIN() at the beginning of the method, and ATOMISM_LOGOUT
     * at the end. In this case, performance checks in terms of the time spent in each method 
     * are available.
     */   
    class Logger {
      
      friend class Exception;
      friend void throwAny(Exception);
      
      public:
	    
      enum Priority { DEBUG, INFO, WARNING, ERROR };
          
      
        class LogElement {
	  
            friend class Exception;
	    friend class Logger;
	    friend void throwAny(Exception);
		
	    typedef std::vector< std::shared_ptr<LogElement> > VectorElem;
	    typedef std::vector<std::pair<Priority,string> >   VectorMessages;
	    
        public:
         
            LogElement(const std::string& function,LogElement* parent) : 
            _Name(function), _Parent(parent) {
	      
	        _TimeBegin = clock();
	    };
        
            LogElement* addFunctionCall(const std::string& function) { 
	      
                  _Children.push_back(std::shared_ptr<LogElement>( new Logger::LogElement(function,this) ));
		  return _Children.back().get();
	    };
        
	    void addMessage( Priority priority , 
	  		     std::string message ) {
	        _Messages.push_back( pair<Priority,std::string>(priority,message) );	       
	    };
		
            VectorElem 	   getFunctionsCalled() const { return _Children;}
            LogElement*    getParent()       	const { return _Parent;  }
            std::string    getFunctionName()	const { return _Name;    }
            VectorMessages getMessages()        const { return _Messages;}
	
            double duration() { return _TimeEnd - _TimeBegin; };
        
            LogElement* closeFunction(std::string) { _TimeEnd = clock(); }
        
        private:
           
            std::string _Name;
            LogElement* _Parent;
            std::vector<std::shared_ptr<LogElement> > _Children;
            std::vector<std::pair<Priority,string> >  _Messages;
	    
            double _TimeBegin;
            double _TimeEnd;
        };
        
    public:
        
        // start/stop logging
        // - messages with priority >= minPriority will be written in log
        
        static void start(Priority minPriority, int llm);
        static void clear();
	
        static void enterFunction(const std::string& fct);
	static void exitFunction(const std::string& fct);
	static void write(Priority priority, const string& message);
	
        template<int N, typename T>
        static void writeMultiColumns(std::array<T,N>& heads);
        	
    private:
        
        // Logger adheres to the singleton design pattern, hence the private
        // constructor, copy constructor and assignment operator.
	      
        Logger& operator = (const Logger& logger) {return *this;}
        
        Logger();
	
        Logger(const Logger& logger) {}
        
        static bool        _Active;
        static bool        _FunctionCalls2Tree;

        static size_t      _CurrentDepth;
	static size_t      _MaxFunctionCallDepth;
        
	static ostream*    _OutStream;
	
        static Priority    _MinPriority;
        
        
        
        static LogElement* _CurrentElement;
        
        // names describing the items in enum Priority
        static const string PRIORITY_NAMES[];
        
        // the sole Logger instance (singleton)
        // static Logger instance;
	static void exitAllFunctions();
    };
    
    // --------------------------------------
    // static members initialization
    // --------------------------------------
       
    bool Logger::_FunctionCalls2Tree = 0;
    
    bool Logger::_Active = 0;   
    
    size_t Logger::_CurrentDepth = 0;
    
    size_t Logger::_MaxFunctionCallDepth = 3;
            
    Logger::Priority Logger::_MinPriority = Logger::DEBUG;
    
    Logger::LogElement* Logger::_CurrentElement = 0;
    
    const std::string Logger::PRIORITY_NAMES[] =
    {   "TRACK",
        "DEBUG",
        "INFO",
        "WARNING"
    };
    
    
    //Logger Logger::instance;
    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------
    
    inline
    Logger::Logger() {}
        
    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------
    
    inline
    void Logger::start(Priority min, int llm) {
      
        _CurrentElement = new LogElement("Logger",0);
        _CurrentDepth = 0;
        _MaxFunctionCallDepth = llm ;
        _Active = true;
        _MinPriority = min;
	_OutStream = &(std::cout);
    }
    
    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------
    
    inline        
    void Logger::clear()
    {
        if( _CurrentElement !=0 )
            delete _CurrentElement;
	_CurrentDepth = 0;
    }
    
    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------

    inline        
    void Logger::enterFunction( const std::string& fct ) {
        
        if  (!_Active)  return;
	if( _FunctionCalls2Tree )
            _CurrentElement = _CurrentElement->addFunctionCall(fct);
	
        _CurrentDepth++;
	
        if ( _MinPriority > DEBUG)  return;
	if ( _CurrentDepth++ > _MaxFunctionCallDepth ) return;
	
        for(int i=0;i<_CurrentDepth-1;i++) std::cout<<"\t";
        (*_OutStream)<<fct<<std::endl;
    }
    
    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------
    
    inline        
    void Logger::exitFunction( const std::string& fct ) {
      
        if  (!_Active)  return;
	
	if( _FunctionCalls2Tree )
	    _CurrentElement = _CurrentElement->closeFunction(fct);
	
        _CurrentDepth--;
        LOGGER_WRITE(Logger::DEBUG,"exit function");
    }
    
    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------
    
    inline        
    void Logger::write(Priority priority, const string& message ) {
        
        if( !_Active ) return;
	if( _FunctionCalls2Tree ) _CurrentElement->addMessage(priority,message);
	if(priority<_MinPriority) return;
	
        if( (priority==INFO) && 
	    (_CurrentDepth > _MaxFunctionCallDepth )) return;
        
	(*_OutStream)<<PRIORITY_NAMES[priority]<<" : "<<message<<std::endl;
    }
    
    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------

    template<int N,typename T>
    inline    
    void Logger::writeMultiColumns(std::array<T,N>& heads) {
      
        if( !_Active ) return;
	
	int witdh=25;
	stringstream out;
	for(size_t i=0; i<N;i++)  out<<std::setw(witdh)<<heads[i];
	
	if( _FunctionCalls2Tree ) _CurrentElement->addMessage(INFO,out.str());
	
	if( INFO>=_MinPriority) (*_OutStream)<<out<<endl;
	
    }
    
    
}
#endif // MSLOGGER_H
