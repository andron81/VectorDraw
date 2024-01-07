#pragma once


namespace cf_ {

template <typename T>
class Configuration
{
public:
    static Configuration& Instance()
    {
        static Configuration theSingleInstance;
        return theSingleInstance;
    }
    T getData() { return data; }
    void setData(T d_) { data = d_; }
private:
    T data;
    Configuration() {}
    Configuration(const Configuration& root) = delete;
    Configuration& operator=(const Configuration&) = delete;
};


};

