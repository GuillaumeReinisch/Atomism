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
#define LOGGER_WRITECOLUMNS(p)  			Logger::writeMultiColumns(p);

#define ATOMISM_LOG()            			ScopLog scoplog(__PRETTY_FUNCTION__);

using namespace std;

namespace atomism
{
    
   class Exception;
   class Logger;


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
            
            std::string    getFunctionName()	const { 
	      
	        string tmp0=_Name.substr(0,_Name.find("["));
	        if(tmp0.find("<")==-1) return tmp0;
	        string classname = tmp0.substr(0,tmp0.find("<"));
	        string method =    tmp0.substr(tmp0.find(">")+1);
	        if ( size_t(method.find(")")-method.find("(")) > 5 )
		   return classname+method.substr(0,method.find("("))+"(...)";
	      
	        return classname + method.substr(0,method.find("("));   	      
	    }
	    
            VectorMessages getMessages()        const { return _Messages;}
	
            double duration() const { return double(_TimeEnd - _TimeBegin)*1000 / CLOCKS_PER_SEC; };
        
	    double totalTime() const {
	      
	      if (_TotalTime==0) _TotalTime = CLOCKS_PER_SEC/1000;
	      return _TotalTime * 1000/ CLOCKS_PER_SEC; }
	    
            LogElement* closeFunction(std::string) { _TimeEnd = clock();
					             _TotalTime += double(_TimeEnd - _TimeBegin);
						      return _Parent; }
        
        private:
           
            std::string _Name;
            LogElement* _Parent;
            std::vector<std::shared_ptr<LogElement> > _Children;
            std::vector<std::pair<Priority,string> >  _Messages;
	     
            clock_t _TimeBegin;
            clock_t _TimeEnd;
	    
	    static double _TotalTime;
        };
        
    public:
        
        // start/stop logging
        // - messages with priority >= minPriority will be written in log
        
        static void start(Priority minPriority, int llm);
	static void record();
        static void clear();
	
        static void enterFunction(const std::string& fct);
	static void exitFunction(const std::string& fct);
	static void write(Priority priority, const string& message);
	static void write(Priority priority, const ostream& message);	
	static void toHtml(string filename);
	
	/*template<typename T>
	static void operator<<(T text);
	*/
	
        template<typename T,size_t N>
        static void writeMultiColumns(std::array<T,N>& heads);
        	
	static const LogElement* getTreeElement() {return _CurrentElement;}
	
    private:
        
        // Logger adheres to the singleton design pattern, hence the private
        // constructor, copy constructor and assignment operator.
	      
        Logger& operator = (const Logger& logger) {return *this;}
        
        Logger();
	
        Logger(const Logger& logger) {}
        
        static void insertChildrenInHtml(LogElement* element,std::ostream& outfile,int& ivar);
	    
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
    
      
   class ScopLog {
	
    public:
          ScopLog(const string& method): _Method(method){  
	             Logger::enterFunction( _Method );
	  };
	  ~ScopLog(){Logger::exitFunction( _Method );}
	  
    private:
          string _Method;
    };
    
    // --------------------------------------
    // static members initialization
    // --------------------------------------
       
    double Logger::LogElement::_TotalTime =0;
    
    bool Logger::_FunctionCalls2Tree = 0;
    
    bool Logger::_Active = 0;   
    
    size_t Logger::_CurrentDepth = 0;
    
    size_t Logger::_MaxFunctionCallDepth = 3;
            
    Logger::Priority Logger::_MinPriority = Logger::DEBUG;
    
    Logger::LogElement* Logger::_CurrentElement = 0;
    
    const std::string Logger::PRIORITY_NAMES[] =
    {   "DEBUG",
        "INFO",
        "WARNING",
	"ERROR"
    };
    
    ostream*    Logger::_OutStream = 0;
    
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
    void Logger::record() { _FunctionCalls2Tree=1;}
    
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
	LOGGER_WRITE(Logger::DEBUG,"enter function: "+fct);
    }
    
    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------
    
    inline        
    void Logger::exitFunction( const std::string& fct ) {
      
        if  (!_Active)  return;
	
	if( _FunctionCalls2Tree )
	    _CurrentElement = _CurrentElement->closeFunction(fct);
         
	stringstream out;
        LOGGER_WRITE(Logger::DEBUG,"exit function");
	if(_CurrentDepth>0) _CurrentDepth--;
	
	if( _CurrentDepth ==0 ){
	    if( _FunctionCalls2Tree ) Logger::toHtml("./debug.html");
	     clear();
	    _Active =0;
	}
    }
    
    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------
    
    inline        
    void Logger::write(Priority priority, const string& message ) {
        
        if( !_Active ) return;
	if( _FunctionCalls2Tree ) _CurrentElement->addMessage(priority,message);
	if(priority<_MinPriority) return;
	
        if(_CurrentDepth > _MaxFunctionCallDepth ) return;
        
	for(size_t i=0; i<_CurrentDepth;i++) (*_OutStream)<<"  ";
	  
	(*_OutStream)<<PRIORITY_NAMES[priority]<<" : "<<message<<std::endl;
    }
    
    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------
    
    inline        
    void Logger::write(Priority priority, const ostream& message ) {
        
        if( !_Active ) return;
	if( _FunctionCalls2Tree ) { 
	     const stringstream& out = dynamic_cast<const stringstream&>(message);
	    _CurrentElement->addMessage(priority,out.str());
	}
	if(priority<_MinPriority) return;
	
        if(_CurrentDepth > _MaxFunctionCallDepth ) return;
        
	for(size_t i=0; i<_CurrentDepth;i++) (*_OutStream)<<"  ";
	const stringstream& out = dynamic_cast<const stringstream&>(message);
	(*_OutStream)<<PRIORITY_NAMES[priority]<<" : "<<out.str()<<std::endl;
    }
    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------

    template<typename T,size_t N>
    inline    
    void Logger::writeMultiColumns(std::array<T,N>& heads) {
      
        if( !_Active ) return;
	
	int witdh=25;
	stringstream out;
	for(size_t i=0; i<N;i++)  out<<std::setw(witdh)<<heads[i];
	
	if( _FunctionCalls2Tree ) _CurrentElement->addMessage(INFO,out.str());
	
	if( INFO>=_MinPriority) (*_OutStream)<<out<<endl;
	
    }
 
    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------

    inline    
    void Logger::insertChildrenInHtml(LogElement* element,std::ostream& outfile,int& ivar) {
    
         	 
	 std::vector<std::shared_ptr<LogElement> > children=element->getFunctionsCalled();
	 string lmarge="";
	// for( size_t i = 0 ;i<ivar;i++) lmarge+="\t";
	 for( auto child: children) {

	     outfile<< lmarge <<"d = (data["<<ivar<<"] = {});"<<endl;
	     outfile<< lmarge << "indent++;"<<endl;
             outfile<< lmarge << "parents.push("<<ivar<<" - 1);"<<endl;
	     outfile<< lmarge << "if (parents.length > 0) {"<<endl;
             outfile<< lmarge << "parent = parents[parents.length - 1];"<<endl;
             outfile<< lmarge << "} else {"<<endl;
             outfile<< lmarge << "parent = null;"<<endl;
             outfile<< lmarge << "}"<<endl;
	     outfile<< lmarge << "d[\"id\"] = \"id_"<<ivar<<"\""<<endl;
             outfile<< lmarge << "d[\"indent\"] = indent;"<<endl;
             outfile<< lmarge << "d[\"parent\"] = parent;"<<endl;
             outfile<< lmarge << "d[\"title\"] = \""<<child->getFunctionName()<<"\""<<endl;
             outfile<< lmarge << "d[\"duration\"] = \""<<child->duration()<<"\";"<<endl;
	     
             outfile<< "d[\"percentComplete\"]="<<child->duration()/child->totalTime()*100<<";"<<endl;
	     ivar++;
	     Logger::insertChildrenInHtml(child.get(),outfile,ivar);
	     outfile << "indent--;"<<endl;
             outfile << "parents.pop();"<<endl;
	 }
    }
    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------

    inline    
    void Logger::toHtml(string filename) {
    
      
	 std::string line ;
	 int ivar = 0;
	 
         std::ifstream infile( "./src/Utilities/debugTemplate.html" ) ;
	 std::ofstream outfile( filename ) ;
         if ( infile ) {
            while ( getline( infile , line ) ) {
	      
	        outfile << line << '\n' ;//supposing '\n' to be line end
	        if(line == "//treeview"){
		  
                    outfile << "var indent = 0;"<<endl;
                    outfile << "var parents = [];"<<endl;
		    Logger::insertChildrenInHtml(_CurrentElement,outfile,ivar);
		    outfile << "indent--;"<<endl;
		    outfile << "parents.pop();"<<endl;
	     
                    //Logger::insertChildrenInHtml(_CurrentElement,outfile,ivar);
		 }
             }
         }
         
         infile.close();
	 outfile.close();
    }
   
    
}
#endif // MSLOGGER_H
