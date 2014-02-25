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
#define ATOMISM_EXCEPT_IF(FUNC,MESSAGE)		exceptIf( FUNC, MESSAGE, __PRETTY_FUNCTION__ )
#define ATOMISM_EXCEPT_IF(FUNC)			exceptIf( FUNC, "MESSAGE", __PRETTY_FUNCTION__ )

#define ATOMISM_TRY(FUNC)               impact_try( FUNC, __PRETTY_FUNCTION__ )

#include<Atomism/Logger.h>

namespace atomism
{
    
    
    class Exception: public boost::exception, public std::exception  {
        
        
    public:
        
        Exception(const std::string& info, const std::string& function);
        
        Exception(const std::string& info);
        
        void addContext(const std::string& str){ Contexts.push_back(str); }
        
        char const* what() const throw();
        
        ~Exception() throw () { }
        
        std::string getFunction() const { return Function; }
        
        std::string getInfo()     const { return Info;     }
        
        std::vector<std::string> getContexts() const { return Contexts; }
        
    private:
        
        std::string Info;
        
        std::string Function;
        
        std::vector<std::string> Contexts;
        
        mutable std::string message;
    };
    
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    class MethodNotDefined : public Exception {
        
    public:
        
        MethodNotDefined(std::string function) :
        Exception("The method "+function+" is not defined",function,"")
        {  }
    };
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    class MethodVirtual : public Exception {
        
    public:
        
        atMethodVirtual(std::string function) :
        Exception("The method "+function+" is virtual, overiden method need to be used.",function,"")
        {  }
    };
    
    class ExceptionInLogic : public Exception {
        
    public:
        
        ExceptionInLogic(std::string message,std::string function,std::string id) :
        Exception("exception in logic: "+message,function,id)
        {  }
    };
    
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    void throwAny(Exception e);
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    
    void throwAny(std::string message,std::string function, std::string id) {
        
        throwAny(Exception(message,function,id));
    };
    
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
	
    template<class Func>
    inline
    void exceptIf(Func func,std::string message,std::string method,std::string id) {
        
        if( func() ){  ExceptionInLogic e(message,method,id);
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
        
        msLogElement* elem = msLogger::CurrentElement;
        while(elem->Parent != 0 ) elem = elem->Parent;
        msLogger::exitAllFunctions();
        
        BOOST_THROW_EXCEPTION(e);
    };
    
    Exception::Exception(std::string info, std::string function, std::string id) {
        
        Info=info;Id=id;Function=function;
        msLogElement* elem = msLogger::CurrentElement;
        //Function=elem->getFunctionName();
        while(elem->Parent != 0 ) {
            addContext(elem->getFunctionName());
            elem = elem->Parent;
        };
        
    }
    
    Exception::Exception(string info,string id){
        
        Info=info;Id=id;
        msLogElement* elem = msLogger::CurrentElement;
        Function=elem->getFunctionName();
        cout<<"Function:"<<Function<<endl;
        while(elem->Parent != 0 ) {
            addContext(elem->getFunctionName());
            elem = elem->Parent;
        }
    }
    
    PyObject *myCPPExceptionType = NULL;
    
    void translateExceptionPython(Exception const& e) {
        
        assert(myCPPExceptionType != NULL);
        boost::python::object pythonExceptionInstance(e);
        PyErr_SetObject(myCPPExceptionType, pythonExceptionInstance.ptr());
    }
    
    
    void Exception::registryInPython()  {
        
#if USE_PYTHON > 0
        using namespace boost::python;
        
        class_<Exception, boost::shared_ptr<Exception> >
        pyException("Exception", "base class for exception in impact", no_init );
        
        pyException.def("getId", &Exception::getId)
        .def("getFunction", &Exception::getFunction)
        .def("getInfo", &Exception::getInfo)
        .def("getContexts", &Exception::getContexts);
        
        myCPPExceptionType = pyException.ptr();
        boost::python::register_exception_translator<Exception>
        (&translateExceptionPython);
        
#endif
    }
    
    char const*  Exception::what() const throw() {
        
        message = "\nThrow in function "+Function+".\nThrow by the object "+Id+":\n\t "+output::printRed(Info)+".\n Contexts:\n";
        for(size_t i=0; i<Contexts.size();i++)  message+="-"+Contexts[i]+";\n";
        return  message.c_str();
    }
}
#endif