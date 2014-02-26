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


#ifndef MSEXCPETION_H
#define MSEXCPETION_H


#include <boost/exception/all.hpp>


#define ATOMISM_THROW(MESSAGE)     		throwAny( MESSAGE, __PRETTY_FUNCTION__ )
#define ATOMISM_THROW_EXCEPTION(e) 		throwAny(e)
#define ATOMISM_THROW_NOT_IMPLEMENTED()		throwNotImplemented( __PRETTY_FUNCTION__ )
#define ATOMISM_THROW_VIRTUAL() 		throwVirtual( __PRETTY_FUNCTION__ )
#define ATOMISM_EXCEPT_IF(FUNC)			exceptIf( FUNC, #FUNC, __PRETTY_FUNCTION__ )

#define ATOMISM_TRY(FUNC)               	impact_try( FUNC, __PRETTY_FUNCTION__ )

#include<Logger.h>

namespace atomism
{
    
    
    class Exception: public boost::exception, public std::exception  {
        
        
    public:
        
        Exception(const std::string& info, const std::string& function);
                
        void addContext(const std::string& str){ _Contexts.push_back(str); }
        
        char const* what() const throw();
        
        ~Exception() throw () { }
        
        std::string getFunction() const { return _Function; }
        
        std::string getInfo()     const { return _Info;     }
        
        std::vector<std::string> getContexts() const { return _Contexts; }
        
    private:
        
        std::string _Info;
        
        std::string _Function;
        
        std::vector<std::string> _Contexts;
        
        mutable std::string _message;
    };
    
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    class MethodNotDefined : public Exception {
        
    public:
        
        MethodNotDefined(std::string function) :
        Exception("The method not defined",function)
        {  }
    };
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    class MethodVirtual : public Exception {
        
    public:
        
        MethodVirtual(std::string function) :
        Exception("The method is virtual",function)
        {  }
    };
    
    class ExceptionInLogic : public Exception {
        
    public:
        
        ExceptionInLogic(std::string message,std::string function) :
        Exception("Exception in logic: "+message,function)
        {  }
    };
    
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    void throwAny(Exception e);
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    void throwAny(std::string message,std::string function) {
        
        throwAny( Exception(message,function) );
    };
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
	
    template<class Func>
    inline
    void exceptIf(Func func,std::string message,std::string method) {
        
        if( func() ){  ExceptionInLogic e(message,method);
            throwAny(e);
        }
    };
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    template<class Func>
    void atomism_try(Func func,std::string method) {
        
        try{ return func();
        }
        catch(Exception& e0){
            
            e0.addContext(method);
            throwAny(e0);
        }
        catch(std::exception& e0){
            
            Exception e(e0.what(),method,"");
            throwAny(e);
        }
    };
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    void throwNotImplemented(std::string method) {
        MethodNotDefined e(method);
        throwAny(e);
    };
    
    void throwVirtual(std::string method) {
        MethodVirtual e(method);
        throwAny(e);
    };
    
    
    void throwAny(Exception e) {
        Logger::clear();
        throw(e);
    };
    
    Exception::Exception(const std::string& info, const std::string& function) {
        
        _Info=info;
	_Function=function;
        Logger::LogElement* elem = Logger::_CurrentElement;
	
        while(elem->_Parent != 0 ) {
            addContext(elem->getFunctionName());
            elem = elem->_Parent;
        };
    }
    
    char const*  Exception::what() const throw() {
        
        _message = _Function+" : "+_Info+".\n Contexts:\n";
        for(size_t i=0; i<_Contexts.size();i++)  
	    _message+="-"+_Contexts[i]+";\n";
        return  _message.c_str();
    }
}
#endif