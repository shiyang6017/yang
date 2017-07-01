#include <iostream>
#include <memory>
#include <unordered_map>

class Observer;

class Subject {
public:
    void registerObserver(const std::weak_ptr<Observer> obs);
    void unregisterObserver();
    bool notify(const std::string& observerName);
    void notifyall();
private:
    std::unordered_map<std::string, std::weak_ptr<Observer>> observers_;
};


class Observer : public std::enable_shared_from_this<Observer> {
public:
    void update() {
        /**/
    }
    const std::string& getName() const {
        return name_;
    }
    
private:
    std::string name_;
};
// know when to use by-ref and by-value
void Subject::registerObserver(const std::weak_ptr<Observer> weakObs) {
    auto shdObs = weakObs.lock();
   
    if (shdObs) {
        auto name = shdObs->getName();
        {
            // lock
            observers_.emplace(name, weakObs); 
            // unlock;
        }
    }
}

bool Subject::notify(const std::string& observerName) {
    //lock;
    auto iter = observers_.find(observerName); 
    if (iter == observers_.end()) {
        //unlock;
        return false;
    }
    auto weakObs = iter->second; 
    auto shdObs = weakObs.lock();
    if (shdObs) {
        shdObs->update(); 
    } else {
        observers_.erase(iter); 
    }
    // unlock;
    return true;
}



int main() {

}