#ifndef LOGGER_LOGGER_H
#define LOGGER_LOGGER_H

#include <ostream>
#include <concepts>

namespace logger_details {
  template <class T>
  using rmcvref_t = std::remove_cv_t<std::remove_reference_t<T>>;
  
  template <class T>
  using rmcvptr_t = std::remove_cv_t<std::remove_pointer_t<T>>;
  
  template<class T>
  concept HasStrMethod = requires { T().str(); };

  template<class T>
  concept IsContainer = !std::is_same_v<T, std::string> && requires { T().begin(); T().begin()++; T().end(); };
  
  template<class T>
  concept IsChar = std::is_same_v<T, char> || std::is_same_v<T, wchar_t>;

  template<class T>
  concept IsString = std::is_same_v<T, std::string> || std::is_pointer_v<std::decay_t<T>> && std::is_same_v<rmcvptr_t<std::decay_t<T>>, char>;
  
  template<class T>
  concept IsInteger = !IsChar<T> && std::integral<T>;

  template<class T>
  concept IsFloat = std::floating_point<T>;

  template<class T>
  concept Printable = HasStrMethod<T> || IsString<T> || IsInteger<T> || IsFloat<T> || IsChar<T> || IsContainer<T>;
  
  // is pointer and the object is printable
  // is container and the content is printable
  // is array
  // is map
  
  using std::operator""s;
  
  class Logger {
    class LoggerImpl {
    public:
      explicit LoggerImpl(std::ostream* os) : os_(os) {}

      template<IsChar T>
      LoggerImpl& operator<<(const T& c) {
        T s[2] = {c, T('\0')};
        *os_ << std::quoted(s, T('\''));
        return *this;
      }

      template<IsInteger T>
      LoggerImpl& operator<<(const T& i) {
        *os_ << std::to_string(i);
        return *this;
      }

      template<IsFloat T>
      LoggerImpl& operator<<(const T& f) {
        *os_ << std::to_string(f);
        return *this;
      }
      
      template<IsString T>
      LoggerImpl& operator<<(const T& str) {
        *os_ << str;
        return *this;
      }

      template<IsContainer T>
      LoggerImpl& operator<<(const T& container) {
        int flg = 0;
        *this << "[";
        for (const auto& item : container) {
          *this << (flg++ ? "," : "") << item;
        }
        *this << "]";
        return *this;
      }
      
      template<HasStrMethod T>
      LoggerImpl& operator<<(const T& obj) {
        *this << obj.str();
        return *this;
      }
      
    private:
      std::ostream* os_;
    };
    
  public:
    explicit Logger(std::ostream* os, int float_precision = 4) : impl(os) {
//      os->setf(std::ios_base::precision( float_precision));
    }

    template <Printable ...Args>
    Logger& operator()(const Args &...args) {
      ((impl << args << " "), ...);
      impl << "\n";
      return *this;
    }
    
    template <Printable T>
    Logger& operator<<(const T &t) {
      impl << t;
      return *this;
    }
    
  private:
    LoggerImpl impl;
  };
}

using Logger = logger_details::Logger;

#endif //LOGGER_LOGGER_H
