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
#define ATOMISM_RETURN(r)        Logger::exitFunction(__PRETTY_FUNCTION__);  return r;

using namespace std;

namespace impact
{
    
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
    template<typename ScalarType=double>
    class Logger : boost::noncopyable {
      
      friend class Exception;
      friend void throwAny(Exception);
      
          
        class LogElement {
	  
            friend class Exception;
	    friend class Logger;
	    friend void throwAny(Exception);
		
            struct LogMessage{ std::string MessagePriority; 
	                       std::string Message; };
	      
        public:
         
            LogElement();
        
            LogElement* addFunctionCall(std::string fctpriority, 
				    std::string function);
        
	    void addMessage( std::string messagepriority , 
	  		     std::string message );
		
            std::vector< boost::shared_ptr<LogElement> > getFunctionsCalled();
        
            LogElement* getParent();
        
        
            std::string getFunctionName();
	
            std::vector<std::string> getMessages();
	
            std::string getPriority();
	
            std::string getTime();
	
            std::string getTimeElapsed();
        
            void closeFunction(std::string);
        
        private:
       
            double TimeBegin;
	
            double TimeEnd;
        
            std::vector<boost::shared_ptr<LogElement> > Children;
        
            std::string FunctionPriority;
        
            std::string FunctionCalled;
        
            std::vector<std::string> Messages;
        
            std::string TimeStamp;
        
            LogElement* Parent;
        };
    
    public:
      
        // log priorities
        enum Priority {
	  
	    TRACK,
            DEBUG,
            INFO,
            WARNING,
            ERROR
        };
        
    public:
        
        
        // start/stop logging
        // - messages with priority >= minPriority will be written in log
        
        static void start(Priority minPriority, int llm);
        static void restart();
        static void stop();
        
        static void enterFunction(const std::string& fct,
				  const std::string& objId);
	 
	static void exitFunction( const std::string& fct);
	       
	static void write(Priority priority, 
			  const string& message,
		          const string& fct);
	
        static void write(Priority priority,
			  stringstream& message,
			  const string& fct);
	
        
	static void header2Columns(Priority priority, 
				   std::string c0, 
                                   std::string c1);
	
        static void write2Columns( Priority priority,
				   ScalarType a, 
				   ScalarType b);
	
        static void header3Columns(Priority priority,
				   std::string c0, 
                                   std::string c1,
                                   std::string c2);
	
        static void write3Columns(Priority priority, 
				  ScalarType a, 
				  ScalarType b, 
				  ScalarType c );
        
        static void header4Columns(Priority priority, 
				   std::string c0, 
                                   std::string c1, 
                                   std::string c2, 
                                   std::string c3 );
	
        static void write4Columns(Priority priority, 
				  ScalarType a, 
				  ScalarType b,
				  ScalarType c, 
				  ScalarType d );
        	
    private:
        
        // Logger adheres to the singleton design pattern, hence the private
        // constructor, copy constructor and assignment operator.
	      
        Logger& operator = (const Logger& logger) {return *this;}
        
        Logger();
	
        Logger(const Logger& logger) {}
        

        static bool        active;
	
        ofstream           OutStream;
	
        static Priority    minPriority;
        
        static bool        MethodCallsTracked;
        
        static LogElement* CurrentElement;
        
        // names describing the items in enum Priority
        static const string PRIORITY_NAMES[];
        
        // the sole Logger instance (singleton)
        // static Logger instance;
	static void exitAllFunctions();
    };
    
    // --------------------------------------
    // static members initialization
    // --------------------------------------
       
    int Logger::InfoLevel = 0;
    
    int Logger::InfoLevelMax = 3;
    
    bool Logger::active =0;
    
    Logger::Priority Logger::minPriority = Logger::DEBUG;
    
    const string Logger::PRIORITY_NAMES[] =
    {   "TRACK",
        "DEBUG",
        "INFO",
        "WARNING"
    };
    
    
    //Logger Logger::instance;
    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------
    
    template<typename ScalarType>
    inline
    Logger::Logger() {}
        
    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------
    
    template<typename ScalarType>
    inline
    void Logger::start(Priority min, int llm) {
      
        CurrentElement = new LogElement();
        InfoLevel = 0;
        InfoLevelMax = llm ;
        active = true;
        min = minPriority;
    }
    
    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------
    
    template<typename ScalarType>
    inline        
    void Logger::stop()
    {
        active = false;
        delete CurrentElement;
    }
    
    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------

    template<typename ScalarType>
    inline        
    void Logger::enterFunction(std::string fct,std::string objId) {
        
        if  (!active)  return;
        NoOfFunctionOpen++;
        if ( minPriority > DEBUG)  return;
	if( (minPriority == INFO ) && (NoOfFunctionOpen > InfoLevelMax ) ) return;
	
        for(int i=0;i<NoOfFunctionOpen-1;i++) std::cout<<"\t";
        
        std::cout<<output::printGreen(objId)
        <<" "<<fct
        <<std::endl;
        
        if(areMethodCallsTracked())
            CurrentElement = CurrentElement->addFunctionCall( PRIORITY_NAMES[INFO], fct );
        
    }
    
    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------
    
    template<typename ScalarType>
    inline    
    void Logger::enterFunctionDbg( std::string fct, std::string objId) {
        
        if  (!active)  return;
	if (NoOfFunctionOpen<0) NoOfFunctionOpen=1;
        NoOfFunctionOpen++;
        if ( minPriority > DEBUG)  return;
               
        for(int i=0;i<NoOfFunctionOpen;i++) std::cout<<"\t";
        
        std::cout<<output::printYellow(objId)
        <<" "<<fct<<". Function open N°"<<NoOfFunctionOpen<<endl;
	
        if(areMethodCallsTracked())
            CurrentElement = CurrentElement->addFunctionCall( PRIORITY_NAMES[DEBUG], fct );
    }
    
    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------
    
    template<typename ScalarType>
    inline        
    void Logger::exitFunction() {
      
        if  (!active)  return;
        NoOfFunctionOpen--;
        if ( minPriority > DEBUG)  return;
        
        if(areMethodCallsTracked()){
            
            CurrentElement->closeFunction("");
            if( CurrentElement->getParent() != 0 )
               CurrentElement = CurrentElement->getParent();
        }
    }
   
    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------

    template<typename ScalarType>
    inline    
    void Logger::exitFunction(std::string fct) {
        
        if  (!active)  return;
        NoOfFunctionOpen--;
        if ( minPriority > DEBUG)  return;
	
        LOGGER_WRITE(Logger::DEBUG,"exit function");
                
        if(areMethodCallsTracked()) {
            try {
                CurrentElement->closeFunction(fct);
            }
            catch (msException& e) {
	        stringstream out;
		out<<"void Logger::exitFunction(std::string fct), method still open "<<endl;
		while ( CurrentElement->getParent() != 0 ) {
	  
	            out<<"Function called:"<<CurrentElement->getFunctionName()<<" "<<endl;
	            CurrentElement = CurrentElement->getParent();
		}
                e.addContext(out.str());
	        IMPACT_THROW_EXCEPTION(e);	    
	    }
            if( CurrentElement->getParent() != 0 )
                CurrentElement = CurrentElement->getParent();
        }
    }
    
    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------

    template<typename ScalarType>
    inline        
    void Logger::exitAllFunctions() {
        
        if  (!active)  return;
	if (CurrentElement==0) return;
	cout<<"an exception occured, exit all function"<<endl;
	LogElement* elem = Logger::CurrentElement;
        while(elem->Parent != 0 ) {
	    cout<<"exit Function:"<<elem->getFunctionName()<<endl;
	    elem->closeFunction(elem->getFunctionName());
            NoOfFunctionOpen--;
	    elem = elem->Parent;
	}
	NoOfFunctionOpen=0; 
	CurrentElement = elem;
	cout<<"done exiting all functions"<<endl;
    }
    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------
    
    template<typename ScalarType>
    inline        
    void Logger::write(Priority priority, const string& message , const string& fct) {
        
        if( (!active) || (priority<minPriority) ) return;
        if( (minPriority == INFO ) && (NoOfFunctionOpen > InfoLevelMax ) ) return;
        
        if(priority==ERROR){
            
            std::cout<<PRIORITY_NAMES[priority]<<" : "
            <<output::printRed(message)<<std::endl;
            char a[10];std::cin>>a;return;
        }
        if(priority==WARNING){
            
            std::cout<<PRIORITY_NAMES[priority]<<" : "
            <<output::printYellow(message)<<std::endl;
        }
        if(priority==INFO){
            
            std::cout<<PRIORITY_NAMES[priority]<<" : "
            <<output::printGreen(message)<<std::endl;
        }
        if(priority==DEBUG){
            
            std::cout<<PRIORITY_NAMES[priority]<<" : "
            <<message<<std::endl;
        }
        if(areMethodCallsTracked())
            CurrentElement->addMessage(PRIORITY_NAMES[priority], message );
    }
   
    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------

    template<typename ScalarType>
    inline    
    void Logger::write( Priority priority , const string& message )
    {
        if( (!active) || (priority<minPriority) ) return;
        if( (minPriority == INFO ) && (NoOfFunctionOpen > InfoLevelMax ) ) return;
        
        for(int i=0;i<NoOfFunctionOpen;i++) std::cout<<"\t";
        
        if(priority==ERROR){
            
            std::cout<<PRIORITY_NAMES[priority]<<" : "
            <<output::printRed(message)<<std::endl;
            char a[10];std::cin>>a;
        }
        if(priority==WARNING){
            
            std::cout<<PRIORITY_NAMES[priority]<<" : "
            <<output::printYellow(message)<<std::endl;
        }
        if(priority==INFO){
            
            std::cout<<PRIORITY_NAMES[priority]<<" : "
            <<output::printGreen(message)<<std::endl;
        }
        if(priority==DEBUG){
            
            std::cout<<PRIORITY_NAMES[priority]<<" : "
            <<message<<std::endl;
        }
        if(areMethodCallsTracked())
            CurrentElement->addMessage(PRIORITY_NAMES[priority], message );
    }
    
    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------

    template<typename ScalarType>
    inline    
    void Logger::header2Columns(Priority priority,string c0, string c1)
    {
        if( (!active) || (priority<minPriority) ) return;
        if( (minPriority == INFO ) && (NoOfFunctionOpen > InfoLevelMax ) ) return;
        
        int witdh=25;
        std::string v0=c0+"["+u0+"]";
        std::string v1=c1+"["+u1+"]";
        std::stringstream comment;
        comment<<std::setw(witdh)<<v0<<std::setw(witdh)<<v1;
        std::cout<<comment.str()<<std::endl;
        
        if(areMethodCallsTracked())
            CurrentElement->addMessage(PRIORITY_NAMES[priority], comment.str() );
    }
    
    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------
    
    template<typename ScalarType>
    inline    
    void Logger::write2Columns(Priority priority,double a, double b)
    {
        if( (!active) || (priority<minPriority) ) return;
        if( (minPriority == INFO ) && (NoOfFunctionOpen > InfoLevelMax ) ) return;
        
        int witdh=25;
        std::stringstream comment;
        comment<<std::setw(witdh)<<a<<std::setw(witdh)<<b<<std::endl;
        std::cout<<comment.str();
        
        if(areMethodCallsTracked())
            CurrentElement->addMessage(PRIORITY_NAMES[priority], comment.str() );
    }
    
    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------
    
    template<typename ScalarType>
    inline    
    void Logger::header3Columns(Priority priority,string c0, string c1, string c2) {
        
        if( (!active) || (priority<minPriority) ) return;
        if( (minPriority == INFO ) && (NoOfFunctionOpen > InfoLevelMax ) ) return;
        
        int witdh=25;
        std::string v0=c0+"["+u0+"]";
        std::string v1=c1+"["+u1+"]";
        std::string v2=c2+"["+u2+"]";
        std::stringstream comment;
        comment<<std::setw(witdh)<<v0<<std::setw(witdh)<<v1<<std::setw(witdh)<<v2;
        std::cout<<comment.str()<<std::endl;
        
        if(areMethodCallsTracked())
            CurrentElement->addMessage(PRIORITY_NAMES[priority], comment.str() );
    }
    
    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------
    
    template<typename ScalarType>
    inline    
    void Logger::write3Columns(Priority priority,double a, double b, double c) {
        
        if( (!active) || (priority<minPriority) ) return;
        if( (minPriority == INFO ) && (NoOfFunctionOpen > InfoLevelMax ) ) return;
        
        int witdh=25;
        std::stringstream comment;
        comment<<std::setw(witdh)<<a<<std::setw(witdh)<<b<<std::setw(witdh)<<c;
        std::cout<<comment.str()<<std::endl;
        if(areMethodCallsTracked())
            CurrentElement->addMessage(PRIORITY_NAMES[priority], comment.str() );
    }
    
    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------
    
    template<typename ScalarType>
    inline    
    void Logger::header4Columns(Priority priority,string c0, string c1, string c2, string c3) {
        
        if( (!active) || (priority<minPriority) ) return;
        if( (minPriority == INFO ) && (NoOfFunctionOpen > InfoLevelMax ) ) return;
        
        int witdh=25;
        std::string v0=c0+"["+u0+"]";
        std::string v1=c1+"["+u1+"]";
        std::string v2=c2+"["+u2+"]";
        std::string v3=c3+"["+u3+"]";
        std::stringstream comment;
        comment<<std::setw(witdh)<<v0<<std::setw(witdh)<<v1<<std::setw(witdh)<<v2<<std::setw(witdh)<<v3;
	std::cout<<comment.str()<<std::endl;
	
        if(areMethodCallsTracked())
            CurrentElement->addMessage(PRIORITY_NAMES[priority], comment.str() );
    }
    
    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------
    
    template<typename ScalarType>
    inline    
    void Logger::write4Columns( Priority priority, double a, double b, double c, double d) {
        
        if( (!active) || (priority<minPriority) ) return;
        if( (minPriority == INFO ) && (NoOfFunctionOpen > InfoLevelMax ) ) return;
        
        int witdh=25;
        std::stringstream comment;
        comment<<std::setw(witdh)<<a<<std::setw(witdh)<<b<<std::setw(witdh)<<c<<std::setw(witdh)<<d;
	std::cout<<comment.str()<<std::endl;
	
        if(areMethodCallsTracked())
            CurrentElement->addMessage(PRIORITY_NAMES[priority], comment.str() );
    }
    
    
}
#endif // MSLOGGER_H
