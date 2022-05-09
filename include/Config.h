#include <iostream>
#include <exception>
#include <typeinfo>

#define OGE_DELETE_ASSIGN(Classname)                 \
    Classname& operator=(const Classname&) = delete; \
    Classname(const Classname&) = delete;            


#define OGE_LOG_ERROR(message) \
    std::cerr << "[ERROR]: " << message << std::endl;

 
#define OGE_LOG_INFO(message) \
    std::cerr << "[INFO]: " << message << std::endl;


#define OGE_LOG_WARNING(message) \
    std::cerr << "[WARNING]: " << message << std::endl;


#define OGE_ASSERT(condition, message)                      \
if(!(condition))                                            \
{                                                           \
    std::cerr << "[FATAL ERROR]: " << message << std::endl; \
    throw std::runtime_error(message);                      \
}                                                         


#define OGE_SPECIFY_CLASS_NAME(classname) static inline const char* _NAME_ = classname;


#define OGE_GET_CLASS_NAME(type) \
(type::_NAME_ == nullptr ? typeid(type).name() : type::_NAME_)
